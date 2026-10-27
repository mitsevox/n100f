"""tools/prodg_cc.py reproduces the ProDG 3.5 driver's stage command lines.

DRIVER holds what `ngccc.exe -v <flags> -c <source> -o x.o` printed for cpp and cc1
(ProDG/3.5 from compilers_20251118, under wibo 1.0.3), without the -v/-version echo and
the input and output files. Set PRODG_DIR (the ProDG/3.5 directory) and WIBO to also
check every case against a live run of the driver:

    PRODG_DIR=build/compilers/ProDG/3.5 WIBO=build/tools/wibo python3 -m unittest tests/test_prodg_cc.py
"""

import contextlib
import io
import os
import shlex
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
import prodg_cc  # noqa: E402

# (source, flags, cpp arguments, cc1 arguments)
DRIVER = [
    ('t.c', '-O2',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2'),
    ('t.c', '-Wall -O2',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -Wall -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -Wall -O2'),
    ('t.c', '-O2 -Wall',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -Wall -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2 -Wall'),
    ('t.c', '-UPPC -O2',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -UPPC -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2'),
    ('t.c', '-U__CHAR_UNSIGNED__',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -U__CHAR_UNSIGNED__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet'),
    ('t.c', '-D__SN__=2 -O2',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__SN__=2 -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2'),
    ('t.c', '-Wall -I inc -DFOO -O2 -UBAR -nostdinc -w',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -Wall -Iinc -DFOO -D__OPTIMIZE__ -UBAR -nostdinc -w -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -Wall -O2 -w'),
    ('t.c', '-DA -O0 -O2 -O3 -Wall -O1',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -DA -D__OPTIMIZE__ -D__OPTIMIZE__ -Wall -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O0 -O2 -O3 -Wall -O1'),
    ('t.c', '-O2 -I inc -include quo.h -isystem inc -DX=1 -g -Wno-inline',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -Iinc -include quo.h -isystem inc -DX=1 -Wno-inline -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2 -g -Wno-inline'),
    ('t.c', '-O2 -G 4 -mcpu=750',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2 -G4 -mcpu=750'),
    ('t.c', '-O2 -mcpu=750 -fno-exceptions -Wno-inline -nostdinc -Iinc -D__GEKKO__ -G4',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -Wno-inline -nostdinc -Iinc -D__GEKKO__ -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -O2 -mcpu=750 -fno-exceptions -Wno-inline -G4'),
    ('t.cpp', '-O2 -fno-exceptions -Werror -DZ',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__OPTIMIZE__ -Werror -DZ -D__NOASMPP__ -D__GNUG__=2 -lang-c++ -D__cplusplus -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C_PLUS_PLUS',
     '-quiet -O2 -fno-exceptions -Werror'),
    ('t.c', '-g -O0 -fsigned-char -funsigned-char',
     '-D__GNUC__=2 -D__GNUC_MINOR__=95 -D__NOASMPP__ -lang-c -DPPC -D__PPC__ -D__PPC -Acpu(powerpc) -Amachine(powerpc) -D__CHAR_UNSIGNED__ -D_BIG_ENDIAN -D__BIG_ENDIAN__ -Amachine(bigendian) -D__PTRDIFF_TYPE__=int -D__SIZE_TYPE__=unsigned -D_CALL_SYSV -DSN_TARGET_NGC -D__SN__ -D_LANGUAGE_C -DLANGUAGE_C',
     '-quiet -g -O0 -fsigned-char -funsigned-char'),
]


def wrapper_arguments(source, flags):
    options, cpp_flags, compiler_flags, language = prodg_cc.parse(
        shlex.split(flags) + ["--dir", ".", "-c", source, "-o", "x.o"])
    return prodg_cc.stage_arguments(cpp_flags, compiler_flags, language)


def driver_arguments(directory, wrapper, source, flags):
    with tempfile.TemporaryDirectory() as temporary:
        (Path(temporary) / "inc").mkdir()
        (Path(temporary) / "quo.h").touch()
        (Path(temporary) / source).write_text("int f(void) { return 1; }\n")
        environment = {**os.environ, "SN_NGC_PATH": directory, "TMP": temporary, "TEMP": temporary}
        result = subprocess.run([wrapper, str(Path(directory) / "ngccc.exe"), "-v", *shlex.split(flags),
                                 "-c", source, "-o", "x.o"], cwd=temporary, env=environment,
                                capture_output=True, text=True, timeout=60)
    lines = [line.split() for line in (result.stdout + result.stderr).splitlines() if line.strip()]
    cpp = next(line for line in lines if line[0].endswith("\\cpp"))
    cc1 = next(line for line in lines if line[0].endswith(("\\cc1", "\\cc1plus")))
    return ([a for a in cpp[1:] if a != "-v"][:-2], [a for a in cc1[1:] if a != "-version"][:-3])


class StageArguments(unittest.TestCase):
    def test_recorded_driver_lines(self):
        for source, flags, cpp, cc1 in DRIVER:
            with self.subTest(flags=flags):
                self.assertEqual(wrapper_arguments(source, flags), (cpp.split(), cc1.split()))

    @unittest.skipUnless(os.environ.get("PRODG_DIR") and os.environ.get("WIBO"), "needs PRODG_DIR and WIBO")
    def test_live_driver(self):
        for source, flags, cpp, cc1 in DRIVER:
            with self.subTest(flags=flags):
                # The driver runs from a temporary directory, so relative paths are resolved here
                driver = driver_arguments(str(Path(os.environ["PRODG_DIR"]).resolve()),
                                          str(Path(os.environ["WIBO"]).resolve()), source, flags)
                self.assertEqual(driver, (cpp.split(), cc1.split()))
                self.assertEqual(wrapper_arguments(source, flags), driver)

    def test_rejected_flags(self):
        for flags in ["-ffast-math", "-Os", "-ansi", "-traditional", "-Wextra", "-Wp,-DX",
                      "-D X", "-U X", "-G x"]:
            with self.subTest(flags=flags), self.assertRaises(ValueError):
                wrapper_arguments("t.c", flags)


# Stand-in for a stage: while the counter file holds fewer than argv[3] runs it writes a
# partial output file and fails (exit 3), then it writes its output. Each run appends to
# the log file whether the output file already existed when it started.
FLAKY_STAGE = """import sys
from pathlib import Path
counter, output, failures, log = Path(sys.argv[1]), Path(sys.argv[2]), int(sys.argv[3]), Path(sys.argv[4])
runs = int(counter.read_text()) if counter.exists() else 0
counter.write_text(str(runs + 1))
with log.open("a") as seen:
    seen.write("existed\\n" if output.exists() else "absent\\n")
if runs < failures:
    output.write_text("partial")
    print("Could not read file x.s Expected 69547 bytes but read 0")
    sys.exit(3)
output.write_text("ok")
"""


class StageRetry(unittest.TestCase):
    def run_flaky(self, failures, github_actions=False):
        """Run the stand-in through stage(); return (succeeded, runs, seen, stdout)."""
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            script, counter, output, log = (directory / "stage.py", directory / "runs",
                                            directory / "out.o", directory / "seen")
            script.write_text(FLAKY_STAGE)
            command = [sys.executable, str(script), str(counter), str(output), str(failures), str(log)]
            environment = {k: v for k, v in os.environ.items() if k != "GITHUB_ACTIONS"}
            if github_actions:
                environment["GITHUB_ACTIONS"] = "true"
            stdout = io.StringIO()
            try:
                with contextlib.redirect_stdout(stdout):
                    prodg_cc.stage("NgcAs", command, output, environment)
                succeeded = output.read_text() == "ok"
            except RuntimeError:
                succeeded = False
            return succeeded, int(counter.read_text()), log.read_text().split(), stdout.getvalue()

    def test_one_transient_failure_is_retried(self):
        self.assertEqual(self.run_flaky(1)[:2], (True, 2))

    def test_persistent_failure_fails_after_one_retry(self):
        self.assertEqual(self.run_flaky(5)[:2], (False, 2))

    def test_success_runs_once(self):
        self.assertEqual(self.run_flaky(0)[:2], (True, 1))

    def test_partial_output_is_removed_before_the_retry(self):
        self.assertEqual(self.run_flaky(1)[2], ["absent", "absent"])

    def test_retry_is_annotated_only_on_github_actions(self):
        self.assertEqual(self.run_flaky(1)[3], "")
        self.assertEqual(self.run_flaky(1, github_actions=True)[3],
                         "::warning title=prodg_cc retry::out.o: retrying NgcAs after exit 3: "
                         "Could not read file x.s Expected 69547 bytes but read 0\n")
        self.assertEqual(self.run_flaky(0, github_actions=True)[3], "")

    def test_annotation_escapes(self):
        self.assertEqual(prodg_cc.annotation("100%\r\nx"), "100%25%0D%0Ax")

if __name__ == "__main__":
    unittest.main()
