#!/usr/bin/env python3
"""Compile a C/C++ translation unit with ProDG (SN Systems GCC) through its separate stages.

The ngccc.exe driver spawns cpp, cc1 and ngcas as child processes. Under wibo that
occasionally hangs when several compiles run at once, and the driver's fixed temporary
file names collide between parallel jobs. This wrapper runs the same three stages
directly, each in its own process with a timeout, and writes the intermediates to a
private directory. The stage arguments are the ones the 3.5 driver prints with -v, in
the same order: the user's preprocessor, -O and warning flags in command-line order
after the driver's version defines and before its target predefines, so the predefines
win over a user -D or -U exactly as they do under ngccc. Flags are limited to the ones
checked against that output; the driver turns others (-Os, -ansi, -ffast-math, ...)
into extra preprocessor arguments this wrapper does not reproduce, so they are
rejected. With the accepted flags the object is byte-identical to ngccc's
(tests/test_prodg_cc.py holds the recorded driver command lines).

    prodg_cc.py [--wrapper wibo] --dir <compiler dir> [--depfile out.d] [-x c|c++]
        <flags> -c <source> -o <object>
"""

import os
import signal
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


CPP_COMMON = ["-D__GNUC__=2", "-D__GNUC_MINOR__=95"]
CPP_TARGET = [
    "-DPPC", "-D__PPC__", "-D__PPC", "-Acpu(powerpc)", "-Amachine(powerpc)",
    "-D__CHAR_UNSIGNED__", "-D_BIG_ENDIAN", "-D__BIG_ENDIAN__", "-Amachine(bigendian)",
    "-D__PTRDIFF_TYPE__=int", "-D__SIZE_TYPE__=unsigned", "-D_CALL_SYSV",
    "-DSN_TARGET_NGC", "-D__SN__",
]
# Per language: flags before the target defines, and language defines after them.
CPP_LANG = {
    "c": (["-lang-c"], ["-D_LANGUAGE_C", "-DLANGUAGE_C"]),
    "c++": (["-D__GNUG__=2", "-lang-c++", "-D__cplusplus"], ["-D_LANGUAGE_C_PLUS_PLUS"]),
}
OPTIONS = {"--wrapper", "--dir", "--depfile", "-c", "-o", "-x"}
STAGE_TIMEOUT = 60

# Flags checked against the 3.5 driver's -v output and its objects. -O levels but -O0
# also put -D__OPTIMIZE__ into the cpp arguments at their position; warning flags go to
# both cpp and cc1; the rest go to cc1 only. Anything else is rejected, since untested
# flags can make the driver change the preprocessor arguments (-ffast-math defines
# __FAST_MATH__, -Os defines __OPTIMIZE_SIZE__, -ansi adds -trigraphs).
OPTIMIZE_FLAGS = {"-O", "-O1", "-O2", "-O3"}
WARNING_FLAGS = {"-w", "-Wall", "-Werror", "-Wno-inline"}
COMPILER_FLAGS = OPTIMIZE_FLAGS | WARNING_FLAGS | {
    "-O0", "-g", "-fno-exceptions", "-fsigned-char", "-funsigned-char", "-mcpu=750"}
# Preprocessor flags the driver forwards unchanged. Its separate "-D X" and "-U X" forms
# drop the value, so only the joined forms are accepted; "-I X" is forwarded joined.
CPP_WITH_ARG = {"-include", "-isystem"}


def parse(args):
    """Split args into options, the cpp flags (in driver order) and the cc1 flags."""
    options, cpp_flags, compiler_flags = {}, [], []
    i = 0
    while i < len(args):
        flag = args[i]
        if flag in OPTIONS | CPP_WITH_ARG | {"-I", "-G"}:
            if i + 1 == len(args) or args[i + 1].startswith("-"):
                raise ValueError(f"{flag} requires a value")
            value = args[i + 1]
            if flag in OPTIONS:
                if flag in options:
                    raise ValueError(f"Duplicate option: {flag}")
                options[flag] = value
            elif flag in CPP_WITH_ARG:
                cpp_flags.extend([flag, value])
            elif flag == "-I":
                cpp_flags.append("-I" + value)
            elif value.isdigit():
                compiler_flags.append("-G" + value)
            else:
                raise ValueError(f"-G requires a number: {value}")
            i += 2
            continue
        if flag in ("-D", "-U"):
            raise ValueError(f"Write {flag} joined to its value ({flag}NAME): ngccc drops a separate value")
        if flag.startswith(("-I", "-D", "-U")) or flag == "-nostdinc":
            cpp_flags.append(flag)
        elif flag in COMPILER_FLAGS or flag[:2] == "-G" and flag[2:].isdigit():
            if flag in OPTIMIZE_FLAGS:
                cpp_flags.append("-D__OPTIMIZE__")
            elif flag in WARNING_FLAGS:
                cpp_flags.append(flag)
            compiler_flags.append(flag)
        elif flag.startswith("-"):
            raise ValueError(f"Unsupported flag (not modelled from the ngccc driver): {flag}")
        else:
            raise ValueError(f"Unexpected argument: {flag}")
        i += 1
    for required in ("--dir", "-c", "-o"):
        if required not in options:
            raise ValueError(f"{required} is required")
    language = options.get("-x")
    if language is None:
        extension = Path(options["-c"]).suffix.lower()
        if extension == ".c":
            language = "c"
        elif extension in {".cpp", ".cc", ".cxx", ".cp"}:
            language = "c++"
        else:
            raise ValueError("Specify -x c or -x c++ for this source extension")
    if language not in CPP_LANG:
        raise ValueError("Language must be c or c++")
    return options, cpp_flags, compiler_flags, language


def stage_arguments(cpp_flags, compiler_flags, language):
    """The cpp and cc1 arguments before their input and output files."""
    cpp = CPP_COMMON + cpp_flags + ["-D__NOASMPP__"] + CPP_LANG[language][0] + CPP_TARGET \
        + CPP_LANG[language][1]
    return cpp, ["-quiet"] + compiler_flags


def run_stage(command, output, environment):
    """Run one stage; return its exit status (None if it produced no output) and its output."""
    if output.exists():
        output.unlink()
    process = subprocess.Popen(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, env=environment, start_new_session=True)
    try:
        log, _ = process.communicate(timeout=STAGE_TIMEOUT)
    except subprocess.TimeoutExpired:
        os.killpg(process.pid, signal.SIGKILL)
        process.wait()
        raise RuntimeError(f"Compiler stage timed out producing {output.name}")
    except BaseException:
        os.killpg(process.pid, signal.SIGKILL)
        process.wait()
        raise
    if process.returncode == 0 and not output.is_file():
        return None, log
    return process.returncode, log


def stage(name, command, output, environment):
    # Each stage is a pure function of its input file, so a failed run is retried once.
    # On CI the ProDG tools under wibo have failed rarely with no change in their input
    # (NgcAs: "Could not read file ... Expected 69547 bytes but read 0" on a complete .s);
    # a real error fails both attempts and is reported once.
    status, log = run_stage(command, output, environment)
    if status != 0:
        first = log.decode(errors="replace").strip().splitlines()
        print(f"prodg_cc: retrying {name} after exit {status}: {first[-1] if first else 'no output'}",
              file=sys.stderr)
        status, log = run_stage(command, output, environment)
    sys.stderr.buffer.write(log)
    sys.stderr.flush()
    if status is None:
        raise RuntimeError(f"Compiler stage did not produce {output.name}")
    if status != 0:
        raise RuntimeError(f"Compiler stage failed with exit {status}")


def make_depfile(text, target):
    # cpp names the target after the source (unit.obj) and writes the headers as Windows
    # paths; ninja needs the real output and host paths.
    words = text.replace("\\\r\n", " ").replace("\\\n", " ").split()
    if not words or not words[0].endswith(":"):
        raise RuntimeError("Unexpected dependency information from the preprocessor")
    paths = []
    for word in words[1:]:
        if word[1:2] == ":" and word[:1] in "Zz":
            word = word[2:]
        paths.append(word.replace("\\", "/"))
    return target + ": " + " ".join(paths) + "\n"


def compile_unit(options, cpp_flags, compiler_flags, language):
    # A relative source path reaches the preprocessor as given (relative to the working
    # directory), so __FILE__ keeps the build's relative spelling.
    source = Path(options["-c"])
    if source.is_absolute():
        source = source.resolve()
    directory = Path(options["--dir"]).resolve()
    output = Path(options["-o"]).resolve()
    depfile = Path(options["--depfile"]).resolve() if "--depfile" in options else None
    wrapper = [str(Path(options["--wrapper"]).resolve())] if "--wrapper" in options else []
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix=output.stem + "-", dir=output.parent) as temporary:
        temporary = Path(temporary)
        environment = {**os.environ, "SN_NGC_PATH": str(directory),
                       "TMP": str(temporary), "TEMP": str(temporary)}
        preprocessed = temporary / (output.stem + ".i")
        assembly = temporary / (output.stem + ".s")
        compiled = temporary / output.name
        dependencies = temporary / "dependencies.d"
        cpp_arguments, cc1_arguments = stage_arguments(cpp_flags, compiler_flags, language)
        cpp = wrapper + [str(directory / "cpp.exe")] + cpp_arguments
        if depfile:
            # -MD, not -MMD: this cpp leaves <...> includes out of -MMD output.
            cpp += ["-MD", str(dependencies)]
        stage("cpp", cpp + [str(source), "-o", str(preprocessed)], preprocessed, environment)
        compiler = "cc1plus.exe" if language == "c++" else "cc1.exe"
        stage(compiler, wrapper + [str(directory / compiler)] + cc1_arguments
              + [str(preprocessed), "-o", str(assembly)], assembly, environment)
        stage("NgcAs", wrapper + [str(directory / "NgcAs.exe"), str(assembly), "-o", str(compiled)],
              compiled, environment)
        header = compiled.read_bytes()[:20]
        if len(header) != 20 or header[:7] != b"\x7fELF\x01\x02\x01" \
                or struct.unpack_from(">HH", header, 16) != (1, 20):
            raise RuntimeError("Compiler output is not a relocatable PowerPC ELF32 object")
        if depfile:
            if not dependencies.is_file():
                raise RuntimeError("Preprocessor did not produce dependency information")
            depfile.parent.mkdir(parents=True, exist_ok=True)
            pending = depfile.with_name(depfile.name + ".tmp")
            pending.write_text(make_depfile(dependencies.read_text(), options["-o"]))
            os.replace(pending, depfile)
        os.replace(compiled, output)


def main():
    if os.name == "nt":
        raise RuntimeError("This wrapper currently supports POSIX hosts only")
    compile_unit(*parse(sys.argv[1:]))


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, RuntimeError) as error:
        sys.exit(f"prodg_cc: {error}")
