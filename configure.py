#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GIHE78",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-2"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.3"
config.objdiff_tag = "v3.6.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# These are dtk-template defaults, not evidence. Each library profile needs evidence at the
# unit or library level (config/README.md); per-object or per-function overrides are not accepted
# without the owner's documented approval.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    "-i include/libc",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Library profiles: the flags tools/splits/libmap.py places the library units with
# (config/GIHE78/libmap.json "profiles"). They are shown compatible, not established.
_lib_drop = ("-fp_contract on",)

# Runtime and MSL profile ("runtime").
cflags_runtime = [
    *[f for f in cflags_base if f not in _lib_drop],
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
]

# Dolphin SDK profile ("sdk").
cflags_sdk = [
    *[f for f in cflags_base if f not in _lib_drop],
    "-fp_contract off",
    "-char signed",
    "-common off",
    "-i src",
]

# MetroTRK profile ("trk").
cflags_trk = [
    *[f for f in cflags_base if f not in (*_lib_drop, "-inline auto", '-pragma "cats off"',
                                          '-pragma "warn_notinlined off"', "-maxerrors 1",
                                          "-DNDEBUG=1")],
    "-use_lmw_stmw on",
    "-common off",
    "-inline auto,deferred",
    "-sdata 0",
    "-sdata2 0",
    "-rostr",
    "-char signed",
    "-i src",
]

# Bink profile: ProDG (SN Systems GCC). The version is not established; ProDG/3.5 is the
# only one tools/prodg_cc.py reproduces (compiler row for Bink in evidence.tsv).
cflags_bink = [
    "-O2",
    "-mcpu=750",
    "-fno-exceptions",
    "-nostdinc",
    "-I include",
    "-I include/libc",
]

# CARD profile ("card_debug"): the GIHE78 CARD library is the SDK's debug build, which is
# compiled at -opt level=0 with symbols and assertions.
cflags_card = [
    *[f for f in cflags_base if f not in (*_lib_drop, "-O4,p", "-inline auto", "-sym on",
                                          "-DDEBUG=1", "-DNDEBUG=1")],
    "-opt level=0",
    "-inline off",
    "-schedule off",
    "-sym on",
    "-DDEBUG=1",
    "-char signed",
    "-i src",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

# Game code profile: CodeWarrior for GameCube 1.3.2 (mwcceppc 2.4.2 build 81).
# Evidence: compiler rows in config/GIHE78/evidence.tsv and docs/compiler_profile/.
cflags_game = [
    *[f for f in cflags_base if f not in ("-O4,p", "-inline auto", "-str reuse")],
    "-O4,s",
    "-inline auto,deferred",
    "-lang=c++",
    "-char signed",
    "-common on",
    "-str reuse,pool,readonly",
    "-use_lmw_stmw on",
    '-pragma "cpp_extensions on"',
    "-gccinc",
]

# Same release as the game compiler. Every mwld from GC/1.0 to 2.6 relinks the baseline
# identically, so the target does not independently establish the linker version.
config.linker_version = "GC/1.3.2"


# Helper function for Heavy Iron game libraries
def GameLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": objects,
    }


# Helper function for Dolphin SDK libraries: GC/1.2.5n, which writes .comment version 8
# (the units carry `comment:8` in splits.txt). Evidence: SDK compiler rows in evidence.tsv.
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for the CARD library: the SDK debug build under GC/1.2.5n, .comment version 8
# (compiler row in evidence.tsv).
def CardLib(objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": "card",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_card,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for the Metrowerks runtime and MSL C library. MSL mixes GC/1.3 and GC/1.3.2
# per object (compiler rows in evidence.tsv); objects that place under only one version set
# it, and the rest take the library default, which their import must confirm.
def RuntimeLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_runtime,
        "progress_category": "runtime",
        "objects": objects,
    }


# Helper function for MetroTRK: GC/1.3 (compiler row in evidence.tsv).
def TRKLib(objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/1.3",
        "cflags": cflags_trk,
        "progress_category": "trk",
        "objects": objects,
    }


# Helper function for RAD Game Tools' Bink: ProDG (compiler row in evidence.tsv).
def BinkLib(objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": "binkngc",
        "mw_version": "ProDG/3.5",
        "cflags": cflags_bink,
        "progress_category": "bink",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
# Add libraries and objects only with boundary and compiler profile evidence recorded in
# config/GIHE78/evidence.tsv.
config.libs: List[Dict[str, Any]] = [
    GameLib(
        "game",
        [
            Object(NonMatching, "flat/iCamera.cpp"),
            Object(NonMatching, "flat/iEnv.cpp"),
            Object(NonMatching, "flat/xAnim.cpp"),
            Object(NonMatching, "flat/xBase.cpp"),
            Object(NonMatching, "flat/xbinio.cpp"),
            Object(NonMatching, "flat/xClimate.cpp"),
            Object(NonMatching, "flat/xEntMotion.cpp"),
            Object(NonMatching, "flat/xFFX.cpp"),
            Object(Matching, "flat/xhipio.cpp"),
            Object(NonMatching, "flat/xIni.cpp"),
            Object(NonMatching, "flat/xMath3.cpp"),
            Object(Matching, "flat/xMorph.cpp"),
            Object(NonMatching, "flat/xMovePoint.cpp"),
            Object(NonMatching, "flat/xordarray.cpp"),
            Object(NonMatching, "flat/xPar.cpp"),
            Object(NonMatching, "flat/xpkrsvc.cpp"),
            Object(NonMatching, "flat/xScrFx.cpp"),
            Object(NonMatching, "flat/xserializer.cpp"),
            Object(NonMatching, "flat/xString.cpp"),
            Object(NonMatching, "flat/xutil.cpp"),
            Object(NonMatching, "flat/zCounter.cpp"),
            Object(NonMatching, "flat/zFog.cpp"),
            Object(NonMatching, "flat/zGust.cpp"),
            Object(Matching, "flat/zLaugh.cpp"),
            Object(NonMatching, "flat/zLight.cpp"),
            Object(NonMatching, "flat/zSurface.cpp"),
        ],
    ),
    # Library units: config/GIHE78/lib_units.tsv and docs/splits.md "Library units".
    TRKLib(
        [
            Object(Matching, "TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/msg.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/msgbuf.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/serpoll.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/usr_put.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/msghndlr.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/support.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mutex_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/notify.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/flush_cache.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targimpl.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targsupp.s"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/main_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targcont.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/target_options.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/mslsupp.c"),
        ],
    ),
    RuntimeLib(
        "Runtime.PPCEABI.H",
        [
            Object(Matching, "Runtime.PPCEABI.H/__va_arg.c"),
            Object(Matching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(Matching, "Runtime.PPCEABI.H/runtime.c"),
        ],
    ),
    RuntimeLib(
        "MSL_C.PPCEABI.bare.H",
        [
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/abort_exit.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/alloc.c", mw_version="GC/1.3"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/ansi_files.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/ansi_fp.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/arith.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/buffer_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/char_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/ctype.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/direct_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/file_io.c", mw_version="GC/1.3"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/FILE_POS.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/mbstring.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/mem.c", mw_version="GC/1.3"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/misc_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/printf.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/rand.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/scanf.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/string.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/strtold.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/strtoul.c", mw_version="GC/1.3"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/uart_console_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/wchar_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/e_atan2.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/e_fmod.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_copysign.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_cos.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_floor.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_frexp.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_ldexp.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_modf.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_nextafter.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_sin.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/s_tan.c"),
        ],
    ),
    DolphinLib(
        "OdemuExi2",
        [
            Object(Matching, "dolphin/OdemuExi2/DebuggerDriver.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(NonMatching, "dolphin/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(NonMatching, "dolphin/ar/ar.c"),
            Object(NonMatching, "dolphin/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "ax",
        [
            Object(NonMatching, "dolphin/ax/AXAlloc.c"),
            Object(NonMatching, "dolphin/ax/AXCL.c"),
            Object(NonMatching, "dolphin/ax/AXOut.c"),
            Object(Matching, "dolphin/ax/AXSPB.c"),
            Object(NonMatching, "dolphin/ax/AXVPB.c"),
            Object(Matching, "dolphin/ax/AXProf.c"),
        ],
    ),
    DolphinLib(
        "base",
        [
            Object(NonMatching, "dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "dolphin/db/db.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(NonMatching, "dolphin/dsp/dsp.c"),
            Object(Matching, "dolphin/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "exi",
        [
            Object(NonMatching, "dolphin/exi/EXIBios.c"),
            Object(NonMatching, "dolphin/exi/EXIUart.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(NonMatching, "dolphin/gx/GXInit.c"),
            Object(NonMatching, "dolphin/gx/GXFifo.c"),
            Object(NonMatching, "dolphin/gx/GXAttr.c"),
            Object(NonMatching, "dolphin/gx/GXMisc.c"),
            Object(NonMatching, "dolphin/gx/GXGeometry.c"),
            Object(NonMatching, "dolphin/gx/GXFrameBuf.c"),
            Object(NonMatching, "dolphin/gx/GXLight.c"),
            Object(NonMatching, "dolphin/gx/GXTexture.c"),
            Object(NonMatching, "dolphin/gx/GXBump.c"),
            Object(NonMatching, "dolphin/gx/GXTev.c"),
            Object(NonMatching, "dolphin/gx/GXPixel.c"),
            Object(NonMatching, "dolphin/gx/GXDisplayList.c"),
            Object(NonMatching, "dolphin/gx/GXTransform.c"),
            Object(NonMatching, "dolphin/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "dolphin/mtx/mtx.c"),
            Object(Matching, "dolphin/mtx/mtx44.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(NonMatching, "dolphin/os/OS.c"),
            Object(NonMatching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSAlloc.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(NonMatching, "dolphin/os/OSCache.c"),
            Object(NonMatching, "dolphin/os/OSContext.c"),
            Object(NonMatching, "dolphin/os/OSError.c"),
            Object(NonMatching, "dolphin/os/OSInterrupt.c"),
            Object(Matching, "dolphin/os/OSLink.c"),
            Object(NonMatching, "dolphin/os/OSMemory.c"),
            Object(NonMatching, "dolphin/os/OSReboot.c"),
            Object(NonMatching, "dolphin/os/OSReset.c"),
            Object(Matching, "dolphin/os/OSResetSW.c"),
            Object(NonMatching, "dolphin/os/OSRtc.c"),
            Object(Matching, "dolphin/os/OSSync.c"),
            Object(NonMatching, "dolphin/os/OSThread.c"),
            Object(NonMatching, "dolphin/os/OSTime.c"),
            Object(Matching, "dolphin/os/__ppc_eabi_init.c"),
        ],
    ),
    DolphinLib(
        "si",
        [
            Object(NonMatching, "dolphin/si/SIBios.c"),
            Object(Matching, "dolphin/si/SISamplingRate.c"),
        ],
    ),
    DolphinLib(
        "vi",
        [
            Object(NonMatching, "dolphin/vi/vi.c"),
        ],
    ),
    DolphinLib(
        "pad",
        [
            Object(NonMatching, "dolphin/pad/Padclamp.c"),
            Object(NonMatching, "dolphin/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(NonMatching, "dolphin/dvd/dvdlow.c"),
            Object(NonMatching, "dolphin/dvd/dvdfs.c"),
            Object(NonMatching, "dolphin/dvd/dvd.c"),
            Object(Matching, "dolphin/dvd/dvdqueue.c"),
            Object(Matching, "dolphin/dvd/dvderror.c"),
            Object(Matching, "dolphin/dvd/fstload.c"),
        ],
    ),
    BinkLib(
        [
            Object(NonMatching, "bink/src/sdk/decode/ngc/binkngc.c"),
            Object(NonMatching, "bink/src/sdk/decode/ngc/ngcsnd.c"),
            Object(NonMatching, "bink/src/sdk/decode/binkread.c"),
            Object(NonMatching, "bink/src/sdk/decode/ngc/ngcfile.c"),
            Object(NonMatching, "bink/src/sdk/decode/yuv.cpp"),
            Object(NonMatching, "bink/src/sdk/decode/binkacd.c"),
            Object(NonMatching, "bink/shared/time/radcb.c"),
            Object(NonMatching, "bink/src/sdk/decode/expand.c"),
            Object(NonMatching, "bink/src/sdk/popmal.c"),
            Object(NonMatching, "bink/src/sdk/decode/ngc/ngcrgb.c"),
            Object(NonMatching, "bink/src/sdk/decode/ngc/ngcyuy2.c"),
            Object(NonMatching, "bink/src/sdk/varbits.c"),
            Object(NonMatching, "bink/src/sdk/fft.c"),
            Object(NonMatching, "bink/src/sdk/dct.c"),
            Object(NonMatching, "bink/src/sdk/bitplane.c"),
        ],
    ),
    CardLib(
        [
            Object(Matching, "dolphin/card/CARDBios.c"),
            Object(Matching, "dolphin/card/CARDUnlock.c"),
            Object(Matching, "dolphin/card/CARDRdwr.c"),
            Object(Matching, "dolphin/card/CARDBlock.c"),
            Object(Matching, "dolphin/card/CARDDir.c"),
            Object(Matching, "dolphin/card/CARDCheck.c"),
            Object(Matching, "dolphin/card/CARDMount.c"),
            Object(Matching, "dolphin/card/CARDFormat.c"),
            Object(Matching, "dolphin/card/CARDOpen.c"),
            Object(Matching, "dolphin/card/CARDCreate.c"),
            Object(Matching, "dolphin/card/CARDRead.c"),
            Object(Matching, "dolphin/card/CARDWrite.c"),
            Object(Matching, "dolphin/card/CARDDelete.c"),
            Object(Matching, "dolphin/card/CARDStat.c"),
            Object(Matching, "dolphin/card/CARDStatEx.c"),
            Object(Matching, "dolphin/card/CARDNet.c"),
        ],
    ),
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects


# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
    ProgressCategory("runtime", "Runtime and MSL Code"),
    ProgressCategory("trk", "MetroTRK Code"),
    ProgressCategory("bink", "Bink Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
