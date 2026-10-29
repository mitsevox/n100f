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
from tools.targets import registry, build_targets, define
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

# Explicit identities; archived revisions are discoverable but cannot build.
TARGET_REGISTRY = registry()
VERSIONS = build_targets(TARGET_REGISTRY)
DEFAULT_VERSION = VERSIONS.index(TARGET_REGISTRY['defaultTarget'])

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
config.project_dir = args.build_dir / config.version
config.project_dir.mkdir(parents=True, exist_ok=True)
# BUILD_VERSION stays defined for dtk-template compatibility and is never read; source tests
# VERSION_<ID> (include/version.h).
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
config.reconfig_deps = [
    Path("config/targets.json"),
    Path("tools/targets.py"),
]

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
    f"-D{define(config.version)}",
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
# (config/GIHE78-rev0/libmap.json "profiles"). They are shown compatible, not established.
_lib_drop = ("-fp_contract on",)

# Runtime profile ("runtime").
cflags_runtime = [
    *[f for f in cflags_base if f not in _lib_drop],
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
]

# MSL C profile ("msl"): the runtime profile plus deferred inlining (functions are emitted
# in reverse source order), signed char and fused multiply-add (the fdlibm kernels).
# libmap's msl profile lacks -fp_contract on; the imports established it. Evidence: compiler
# rows in evidence.tsv.
cflags_msl = [
    *cflags_runtime,
    "-inline auto,deferred",
    "-char signed",
    "-fp_contract on",
    "-i src",
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

# RenderWare profile: CodeWarrior for GameCube 1.3 or 1.3.2 (not separated), -O4, signed char,
# no fused multiply-add and no lmw/stmw. Evidence: RenderWare compiler rows in evidence.tsv and
# docs/compiler_profile/README.md "RenderWare". -lang=c and -common off come from rw.flags and
# are not established (docs/libraries.md).
cflags_rw = [
    *[f for f in cflags_base if f not in _lib_drop],
    "-fp_contract off",
    "-lang=c",
    "-char signed",
    "-common off",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

# Game code profile: CodeWarrior for GameCube 1.3.2 (mwcceppc 2.4.2 build 81).
# Evidence: compiler rows in config/GIHE78-rev0/evidence.tsv and docs/compiler_profile/.
cflags_game = [
    *[f for f in cflags_base if f not in ("-O4,p", "-inline auto", "-str reuse")],
    "-O4,s",
    "-inline auto,deferred",
    "-lang=c++",
    "-char signed",
    "-common off",
    "-str reuse,readonly",
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


# Helper function for the Metrowerks runtime: GC/1.3.2 (compiler row in evidence.tsv).
def RuntimeLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_runtime,
        "progress_category": "runtime",
        "objects": objects,
    }


# Helper function for the MSL C library: GC/1.3 (compiler row in evidence.tsv).
def MSLLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3",
        "cflags": cflags_msl,
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


# Helper function for RenderWare 3.3.0.2. GC/1.3 and GC/1.3.2 produce the same probed bodies;
# GC/1.3.2 is the game's release (compiler rows in evidence.tsv).
def RWLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rw,
        "progress_category": "renderware",
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
# config/GIHE78-rev0/evidence.tsv.
config.libs: List[Dict[str, Any]] = [
    GameLib(
        "game",
        [
            Object(NonMatching, "flat/iAnimSKB.cpp"),
            Object(NonMatching, "flat/iCamera.cpp"),
            Object(NonMatching, "flat/iEnv.cpp"),
            Object(MatchingFor("GIHE78-rev0"), "flat/iLight.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/iMath.cpp"),
            Object(NonMatching, "flat/iMath3.cpp"),
            Object(MatchingFor("GIHE78-rev0"), "flat/iMemMgr.cpp"),
            Object(NonMatching, "flat/iModel.cpp"),
            Object(NonMatching, "flat/isavegame.cpp"),
            Object(NonMatching, "flat/iTime.cpp"),
            Object(NonMatching, "flat/xAnim.cpp"),
            Object(NonMatching, "flat/xBase.cpp"),
            Object(NonMatching, "flat/xbinio.cpp"),
            Object(NonMatching, "flat/xClimate.cpp"),
            Object(NonMatching, "flat/xEnt.cpp"),
            Object(NonMatching, "flat/xEntMotion.cpp"),
            Object(NonMatching, "flat/xEnv.cpp"),
            Object(NonMatching, "flat/xFFX.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/xFilter.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/xhipio.cpp"),
            Object(NonMatching, "flat/xIni.cpp"),
            Object(NonMatching, "flat/xMath3.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/xMorph.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/xMovePoint.cpp"),
            Object(NonMatching, "flat/xordarray.cpp"),
            Object(NonMatching, "flat/xPar.cpp"),
            Object(NonMatching, "flat/xpkrsvc.cpp"),
            Object(NonMatching, "flat/xsavegame.cpp"),
            Object(NonMatching, "flat/xScrFx.cpp"),
            Object(NonMatching, "flat/xserializer.cpp"),
            Object(NonMatching, "flat/xString.cpp"),
            Object(NonMatching, "flat/xSurface.cpp"),
            Object(NonMatching, "flat/xutil.cpp"),
            Object(NonMatching, "flat/zActionLine.cpp"),
            Object(NonMatching, "flat/zCamera.cpp"),
            Object(NonMatching, "flat/zConditional.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zCounter.cpp"),
            Object(NonMatching, "flat/zEnt.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zEntBubble.cpp"),
            Object(NonMatching, "flat/zEntButton.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zEntDestructObj.cpp"),
            Object(NonMatching, "flat/zEntHangable.cpp"),
            Object(NonMatching, "flat/zEntPickup.cpp"),
            Object(NonMatching, "flat/zEntPlayer.cpp"),
            Object(NonMatching, "flat/zEntTrigger.cpp"),
            Object(NonMatching, "flat/zEntProjectile.cpp"),
            Object(NonMatching, "flat/zEntShaggy.cpp"),
            Object(NonMatching, "flat/zEntVillain.cpp"),
            Object(NonMatching, "flat/zFeet.cpp"),
            Object(MatchingFor("GIHE78-rev0"), "flat/zFMV.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zFog.cpp"),
            Object(MatchingFor("GIHE78-rev0"), "flat/zFX.cpp"),
            Object(NonMatching, "flat/zGame.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zGameExtras.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zGroup.cpp"),
            Object(NonMatching, "flat/zGust.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zHologram.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zLaugh.cpp"),
            Object(NonMatching, "flat/zLight.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zLightEffect.cpp"),
            Object(NonMatching, "flat/zLightning.cpp"),
            Object(NonMatching, "flat/zLobMaster.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zMusic.cpp"),
            Object(NonMatching, "flat/zParEmitter.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zParSys.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zPitchChange.cpp"),
            Object(NonMatching, "flat/zPlatform.cpp"),
            Object(NonMatching, "flat/zPortal.cpp"),
            Object(NonMatching, "flat/zRenderState.cpp"),
            Object(NonMatching, "flat/zSaveLoad.cpp"),
            Object(NonMatching, "flat/zScene.cpp"),
            Object(NonMatching, "flat/zScript.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zSFX.cpp"),
            Object(NonMatching, "flat/zSurface.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "flat/zTimer.cpp"),
            Object(NonMatching, "flat/zVar.cpp"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "gc/iAnim.cpp"),
            Object(MatchingFor("GIHE78-rev0"), "gc/iMix.c"),
            Object(NonMatching, "gc/ngcrad3d.c"),
            Object(NonMatching, "gc/iFMV.cpp"),
            Object(NonMatching, "gc/iSnd.cpp"),
            Object(NonMatching, "gc/iFX.cpp"),
        ],
    ),
    # Library units: config/GIHE78-rev0/lib_units.tsv and docs/splits.md "Library units".
    TRKLib(
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/msg.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/msgbuf.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/serpoll.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/usr_put.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/msghndlr.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/support.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/mutex_TRK.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/notify.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/flush_cache.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/targimpl.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/targsupp.s"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/__exception.s"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/main_TRK.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/targcont.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "TRK_MINNOW_DOLPHIN/target_options.c"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/mslsupp.c"),
        ],
    ),
    RuntimeLib(
        "Runtime.PPCEABI.H",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "Runtime.PPCEABI.H/__va_arg.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "Runtime.PPCEABI.H/runtime.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "Runtime.PPCEABI.H/GCN_mem_alloc.c"),
        ],
    ),
    MSLLib(
        "MSL_C.PPCEABI.bare.H",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/abort_exit.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/alloc.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/ansi_files.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/ansi_fp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/arith.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/buffer_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/char_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/ctype.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/direct_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/file_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/FILE_POS.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/mbstring.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/mem.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/mem_funcs.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/misc_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/printf.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/qsort.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/rand.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/scanf.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/string.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/strtold.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/strtoul.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/uart_console_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/wchar_io.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_acos.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_asin.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_atan2.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_exp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_fmod.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_pow.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/e_rem_pio2.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/k_cos.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/k_rem_pio2.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/k_sin.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/k_tan.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_atan.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_copysign.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_cos.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_floor.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_frexp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_ldexp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_modf.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_nextafter.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_sin.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/s_tan.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_acos.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_asin.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_atan2.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_exp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_fmod.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/w_pow.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "MSL_C.PPCEABI.bare.H/math_ppc.c"),
        ],
    ),
    DolphinLib(
        "OdemuExi2",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/OdemuExi2/DebuggerDriver.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/amcstubs/AmcExi2Stubs.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/ar/ar.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "ax",
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AX.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AXAlloc.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AXAux.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AXCL.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AXOut.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/ax/AXSPB.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/ax/AXVPB.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/ax/AXProf.c"),
        ],
    ),
    DolphinLib(
        "base",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/db/db.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dsp/dsp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dsp/dsp_debug.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "exi",
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/exi/EXIBios.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/exi/EXIUart.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXInit.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXFifo.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXAttr.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXMisc.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXGeometry.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXFrameBuf.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXLight.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXTexture.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXBump.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXTev.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXPixel.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXStubs.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/gx/GXDisplayList.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXTransform.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/mtx/mtx.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/mtx/mtx44.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/os/OS.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/os/OSAlarm.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSAlloc.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSArena.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSAudioSystem.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSCache.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/os/OSContext.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSError.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSFont.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/os/OSInterrupt.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSLink.c"),
            Object(NonMatching, "dolphin/os/OSMemory.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSMutex.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSReboot.c"),
            Object(NonMatching, "dolphin/os/OSReset.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSResetSW.c"),
            Object(NonMatching, "dolphin/os/OSRtc.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSSync.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSThread.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/OSTime.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/os/__ppc_eabi_init.c"),
        ],
    ),
    DolphinLib(
        "si",
        [
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/si/SIBios.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/si/SISamplingRate.c"),
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
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/pad/Padclamp.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/dvd/dvdlow.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dvd/dvdfs.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/dvd/dvd.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dvd/dvdqueue.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dvd/dvderror.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/dvd/fstload.c"),
        ],
    ),
    BinkLib(
        [
            Object(NonMatching, "bink/src/sdk/decode/ngc/binkngc.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/src/sdk/decode/ngc/ngcsnd.c"),
            Object(NonMatching, "bink/src/sdk/decode/binkread.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/src/sdk/decode/ngc/ngcfile.c"),
            Object(NonMatching, "bink/src/sdk/decode/yuv.cpp"),
            Object(NonMatching, "bink/src/sdk/decode/binkacd.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/shared/time/radcb.c"),
            Object(NonMatching, "bink/src/sdk/decode/expand.c"),
            Object(MatchingFor("GIHE78-rev0"), "bink/src/sdk/popmal.c"),
            Object(NonMatching, "bink/src/sdk/decode/ngc/ngcrgb.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/src/sdk/decode/ngc/ngcyuy2.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/src/sdk/varbits.c"),
            Object(MatchingFor("GIHE78-rev0"), "bink/src/sdk/fft.c"),
            Object(NonMatching, "bink/src/sdk/dct.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "bink/src/sdk/bitplane.c"),
        ],
    ),
    CardLib(
        [
            Object(MatchingFor("GIHE78-rev0"), "dolphin/card/CARDBios.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/card/CARDUnlock.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDRdwr.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDBlock.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDDir.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDCheck.c"),
            Object(MatchingFor("GIHE78-rev0"), "dolphin/card/CARDMount.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDFormat.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDOpen.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDCreate.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDRead.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDWrite.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDDelete.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDStat.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDStatEx.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "dolphin/card/CARDNet.c"),
        ],
    ),
    # RenderWare units: config/GIHE78-rev0/rw_units.tsv and docs/splits.md "RenderWare units".
    RWLib(
        "rpcollis",
        [
            Object(NonMatching, "rwsdk/plugin/collis/ctgeom.c"),
            Object(NonMatching, "rwsdk/plugin/collis/ctworld.c"),
            Object(NonMatching, "rwsdk/plugin/collis/ctbsp.c"),
            Object(NonMatching, "rwsdk/plugin/collis/rpcollis.c"),
        ],
    ),
    RWLib(
        "rphanim",
        [
            Object(NonMatching, "rwsdk/plugin/hanim/rphanim.c"),
        ],
    ),
    RWLib(
        "rpmatfx",
        [
            Object(NonMatching, "rwsdk/plugin/matfx/rpmatfx.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/effectPipesGcn.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/multiTexGcnData.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/gcn/multiTexGcnPipe.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/multiTex.c"),
            Object(NonMatching, "rwsdk/plugin/matfx/multiTexEffect.c"),
        ],
    ),
    RWLib(
        "rpskin",
        [
            Object(NonMatching, "rwsdk/plugin/skin2/rpskin.c"),
            Object(NonMatching, "rwsdk/plugin/skin2/gcn/skingcn.c"),
        ],
    ),
    RWLib(
        "rpusrdat",
        [
            Object(NonMatching, "rwsdk/plugin/userdata/rpusrdat.c"),
        ],
    ),
    RWLib(
        "rpworld",
        [
            Object(NonMatching, "rwsdk/world/babinwor.c"),
            Object(NonMatching, "rwsdk/world/baclump.c"),
            Object(NonMatching, "rwsdk/world/bageomet.c"),
            Object(NonMatching, "rwsdk/world/balight.c"),
            Object(NonMatching, "rwsdk/world/bamateri.c"),
            Object(NonMatching, "rwsdk/world/bamatlst.c"),
            Object(NonMatching, "rwsdk/world/bamesh.c"),
            Object(NonMatching, "rwsdk/world/bameshop.c"),
            Object(NonMatching, "rwsdk/world/basector.c"),
            Object(NonMatching, "rwsdk/world/baworld.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/world/baworobj.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/world/pipe/p2/bapipew.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeAtomicEnumerateLights.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeAtomicInstance.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeLight.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeMaterialScatter.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodePostLight.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeWorldSectorEnumerateLights.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/nodeWorldSectorInstance.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/gcpipe.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/vtxfmt.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/world/pipe/p2/gcn/wrldpipe.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/nodeGameCubeAtomicAllInOne.c"),
            Object(NonMatching, "rwsdk/world/pipe/p2/gcn/nodeGameCubeWorldSectorAllInOne.c"),
        ],
    ),
    RWLib(
        "rwcore",
        [
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/src/plcore/babinary.c"),
            Object(NonMatching, "rwsdk/src/plcore/baerr.c"),
            Object(NonMatching, "rwsdk/src/plcore/bafsys.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/plcore/baimmedi.c"),
            Object(NonMatching, "rwsdk/src/plcore/bamatrix.c"),
            Object(NonMatching, "rwsdk/src/plcore/baresour.c"),
            Object(NonMatching, "rwsdk/src/plcore/bastream.c"),
            Object(NonMatching, "rwsdk/src/plcore/batkbin.c"),
            Object(NonMatching, "rwsdk/src/plcore/batkreg.c"),
            Object(NonMatching, "rwsdk/src/plcore/bavector.c"),
            Object(NonMatching, "rwsdk/src/plcore/resmem.c"),
            Object(NonMatching, "rwsdk/src/plcore/rwstring.c"),
            Object(NonMatching, "rwsdk/src/babinfrm.c"),
            Object(NonMatching, "rwsdk/src/babintex.c"),
            Object(NonMatching, "rwsdk/src/bacamera.c"),
            Object(NonMatching, "rwsdk/src/badevice.c"),
            Object(NonMatching, "rwsdk/src/baframe.c"),
            Object(NonMatching, "rwsdk/src/baimage.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/baimras.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/src/baraster.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/basync.c"),
            Object(NonMatching, "rwsdk/src/batextur.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/batypehf.c"),
            Object(NonMatching, "rwsdk/driver/common/palquant.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dl2drend.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlconvrt.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dldevice.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlraster.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dlsprite.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dltexdic.c"),
            Object(NonMatching, "rwsdk/driver/gcn/dltextur.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/src/pipe/p2/baim3d.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/bapipe.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeClipLine.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeClipTriangle.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeCullTriangle.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/src/pipe/p2/nodeImmInstance.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeImmMangleLineIndices.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeImmMangleTriangleIndices.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/nodeImmRenderSetup.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/nodeImmStash.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/nodeSubmitLine.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/nodeSubmitTriangle.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/nodeTransform.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2core.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2define.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2dep.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2heap.c"),
            Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), "rwsdk/src/pipe/p2/p2renderstate.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/p2resort.c"),
            Object(MatchingFor("GIHE78-rev0"), "rwsdk/src/pipe/p2/gcn/im3dpipe.c"),
            Object(NonMatching, "rwsdk/src/pipe/p2/gcn/nodeDolphinSubmitNoLight.c"),
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
    ProgressCategory("renderware", "RenderWare Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    "--project", str(config.project_dir),
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
