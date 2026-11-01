# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four and CARD were mapped, [Bink units](splits.md#bink-units) how Bink was, and [RenderWare units](splits.md#renderware-units) how RenderWare was. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78-rev0/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H` | GC/1.3.2 | `cflags_runtime` | `runtime` |
| `MSLLib` | `MSL_C.PPCEABI.bare.H` | GC/1.3 | `cflags_msl` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |
| `BinkLib` | `binkngc` | ProDG/3.5 | `cflags_bink` | `bink` |
| `CardLib` | `card` (`dolphin/card/`) | GC/1.2.5n | `cflags_card` | `sdk` |
| `RWLib` | `rpcollis`, `rphanim`, `rpmatfx`, `rpskin`, `rpusrdat`, `rpworld`, `rtintsec`, `rtslerp`, `rwcore` (`rwsdk/`) | GC/1.3.2 | `cflags_rw` | `renderware` |

The 108 RenderWare units, the 89 in `config/GIHE78-rev0/rw_units.tsv` and 19 split by hand (`rwsdk/src/pipe/p2/p2renderstate.c` and the round 24 and 25 [hand splits](splits.md#hand-splits)), are listed in `configure.py`, one `RWLib` per library, NonMatching unless named below. Their compiler is GC/1.3 or GC/1.3.2, with 1.3 and 1.3.2 not separated; `RWLib` uses GC/1.3.2, the game's release. `cflags_rw` sets the established settings: `-O4`, `-char signed`, no contraction (expressed as `-fp_contract off`) and no `-use_lmw_stmw on`. `-lang=c` and `-common off` are carried over from `docs/compiler_profile/rw.flags`; they are not established, because the probe stimuli have no globals and do not test the language mode, and they are to be confirmed when RenderWare data is split. The rest are the base flags, which are not established either ([compiler profile](compiler_profile/README.md#renderware)). The probe covered `rwcore` only; the 23 exact `rpworld` functions below now support the same profile for the world library, and the other plugins take it on the assumption that one build produced the whole SDK. Never use leaked RenderWare source for them.

### RenderWare SDK names and headers

This applies to `src/rwsdk/` and to the RenderWare headers in `include/` (`rwcore.h`, `rpworld.h`, `rphanim.h`, `rpskin.h`, `rpusrdat.h`, `rpmatfx.h`, `rpcollis.h`, `rpmtex.h` and this project's own `rpmtexgcn.h`). Identifier names, and layouts checked against the target, may come from bfbb's `include/rwsdk/` headers, which are derived from the RenderWare SDK headers. By the project owner's decision, SDK macro bodies and enums may also be copied from there (2026-10-28, n100f#74), each credited in [CREDITS.md](../CREDITS.md) with its local changes. Nothing else is copied: no RenderWare function code, comments or other header text. A name or layout no allowed source gives stays opaque or is written from the target (for example a bare enum value with an evidence row), never filled in from recollection of the SDK, and leaked RenderWare source is never used.

### Reconstructed RenderWare sources

`src/rwsdk/` holds sources written for this project from the target's own disassembly; no Criterion RenderWare source was used. Names follow the rules above. `baimmedi.c` (text-only, all 7 functions), `baworobj.c` (all 48 functions and its `.sbss`), `babinary.c` (all 14 functions and its jump table in `.data`), `batypehf.c` (text-only, both functions), `basync.c` (text-only, all 7 functions), `baraster.c` (all 11 functions, its `.data` and its `.sbss`), `bapipe.c` (all 4 functions and its `.sbss`), `baimras.c` (text-only, all 3 functions), `bapipew.c` (text-only, all 5 functions; GIHE78-rev0), `wrldpipe.c` (text-only, all 7 functions; GIHE78-rev0), `bacolor.c` (both functions and its `.sbss`; GIHE78-rev0), `p2renderstate.c` (all 4 functions and its `.sdata2`; GIHE78-rev0), `baim3d.c` (all 9 functions, its `.data` strings and its `.sbss`; GIHE78-rev0), `gcn/im3dpipe.c` (text-only, all 4 functions; GIHE78-rev0), `osintf.c` (text-only, its one function; GIHE78-rev0), `babbox.c` (text-only, all 3 functions; GIHE78-rev0), `babincam.c` (text-only, both functions; GIHE78-rev0) and the Im3D nodes `nodeImmRenderSetup.c`, `nodeImmStash.c`, `nodeSubmitLine.c` and `nodeSubmitTriangle.c` (both functions and the node `.data` each; GIHE78-rev0) and `nodeImmInstance.c` (both functions, its `.data` and its `.sdata2`; GIHE78-rev0), and on GIHE78-rev0 `rpcollis.c`, `rpskin.c`, `bamateri.c`, `bamesh.c`, `basector.c`, `vtxfmt.c`, `baerr.c`, `bafsys.c`, `batkbin.c`, `batkreg.c`, `resmem.c`, `babinfrm.c` and `badevice.c` (all functions and the data each defines) are linked as Matching objects; the other units stay NonMatching (data is not split, or functions remain). 687 functions compile to the target bytes with every relocation resolving to the target's symbol:

| Unit | Exact | Not yet |
| --- | --- | --- |
| `rwsdk/src/plcore/bavector.c` | 11 of 12 | `_rwVectorOpen` (97%: the stack slots of the two inlined table builders' locals, and one load per table loop scheduled before the store instead of after it) |
| `rwsdk/src/plcore/rwstring.c` | 7 of 7 | (Matching on GIHE78-rev0 and GIHE78-rev1) |
| `rwsdk/src/plcore/baerr.c` | 4 of 4 | (Matching on GIHE78-rev0, with its `.sbss` `errorModule`) |
| `rwsdk/src/plcore/bafsys.c` | 4 of 4 | (Matching on GIHE78-rev0, with its `.sdata` mode string) |
| `rwsdk/src/plcore/resmem.c` | 4 of 4 | (Matching on GIHE78-rev0; GIHE78-rev1 carries it in part) |
| `rwsdk/src/plcore/baresour.c` | 7 of 8 | `RwResourcesAllocateResEntry` (98%: the first eviction path keeps the entry in `r3` before copying it to `r30`) |
| `rwsdk/src/plcore/baimmedi.c` | 7 of 7 | (Matching) |
| `rwsdk/src/plcore/batkreg.c` | 10 of 10 | (Matching on GIHE78-rev0, with its `.sbss`) |
| `rwsdk/src/badevice.c` | 14 of 14 | (Matching on GIHE78-rev0, with `engineTKList` in `.data`, `staticGlobals` in `.bss` and `engineInstancesOpened` and `RwEngineInstance` in `.sbss`) |
| `rwsdk/src/baframe.c` | 20 of 21 | `rwFrameDestroyRecurse` (91.4%: GC/1.3.2 stops inlining the recursion one level later than the target, which calls `FrameDestroyRecurseDeInitLeaf` there) |
| `rwsdk/src/bacamera.c` | 18 of 22 | the four camera builders `CameraSync` calls: `CameraBuildPerspViewMatrix` (94.3%), `CameraBuildParallelViewMatrix` (93.1%), `CameraBuildPerspClipPlanes` (91.9%) and `CameraBuildParallelClipPlanes` (89.1%), float scheduling and register choice |
| `rwsdk/src/baraster.c` | 11 of 11 | (Matching; ends at `_rwRasterOpen`, `0x8016DD04`) |
| `rwsdk/src/baimras.c` | 3 of 3 | (Matching) |
| `rwsdk/src/basync.c` | 7 of 7 | (Matching; starts at `SyncObject`, `0x8016E9E4`) |
| `rwsdk/src/pipe/p2/baim3d.c` | 9 of 9 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/bapipe.c` | 4 of 4 | (Matching) |
| `rwsdk/src/pipe/p2/p2core.c` | 11 of 13 | `PipelineNodeDestroy` (99.8%: the output fix-up loop's counters take other registers) and `RxClusterInitializeData` (99.3%: the target restores the saved registers before the link register); its data is not split |
| `rwsdk/src/pipe/p2/p2define.c` | 11 of 13 | `RxPipelineLock` (99.7%: the inlined block grow's offset and the node definition take each other's registers) and `RxLockedPipeUnlock` (97.6%: the size sums add the outputs' total at another point and the copy of the sort data to the top of the block subtracts its entry size from the size first, so their values take other registers); `.sdata` `gMemoryLimits` is in the split |
| `rwsdk/src/pipe/p2/p2dep.c` | 5 of 11 | `_PropDownElimPath` (98.1%), `_AssignClusterSlots` (97.7%), `_ForAllNodesWriteClusterAllocations` (96.8%), `_PropagateDependenciesAndKillDeadPaths` (95.6%), `_TraceClusterScopes` (94.7%) and `_ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields` (90.5%): the inlined entry helpers' values take other registers and some of their stores and constants are scheduled differently |
| `rwsdk/src/pipe/p2/p2heap.c` | 7 of 8 | `RxHeapRealloc` (99.6%: the rounded size and the inlined shrink path's size difference take each other's registers); its message string is not split |
| `rwsdk/src/pipe/p2/p2renderstate.c` | 4 of 4 | (Matching on GIHE78-rev0; split by hand) |
| `rwsdk/src/pipe/p2/p2resort.c` | 1 of 2 | `_rx_rxRadixExchangeSort` (98.6%: the inlined insertion sort's sentinel search takes other registers) |
| `rwsdk/src/pipe/p2/gcn/im3dpipe.c` | 4 of 4 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/nodeImmRenderSetup.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/nodeImmStash.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/nodeSubmitLine.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/nodeSubmitTriangle.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/nodeClipLine.c` | 2 of 4 | `PerspectiveClipProcessPacket` (98.5%) and `ParallelClipProcessPacket` (98.4%): the saved registers of the loop counter, the result flag, the doubled stride and the keep flag differ, and the set-up schedules i = 0 and the line count load differently |
| `rwsdk/src/pipe/p2/nodeClipTriangle.c` | 2 of 4 | `PerspectiveClipProcessPacket` (99.9%) and `ParallelClipProcessPacket` (99.8%): the loop counter, the new-triangle count and the first new vertex take each other's saved registers |
| `rwsdk/src/pipe/p2/nodeCullTriangle.c` | 2 of 4 | `ParallelCullProcessPacket` (98.5%: the triangle index is computed as N + (2N - 3) and the loop registers differ) and `PerspectiveCullProcessPacket` (94.6%: its area tests schedule the loads and products differently) |
| `rwsdk/src/pipe/p2/nodeTransform.c` | 2 of 4 | `TransformNodeParallel` and `TransformNodePerspective` (98.3% each: the packet, stride and clip-flag accumulator registers and the order of the x and y loads) |
| `rwsdk/src/pipe/p2/nodeImmInstance.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/pipe/p2/p2clpcom.c` | 13 of 16 | `_rwForAllEdgesWithRxInterpolant` (73.2%), `_rwForAllEdgesWithoutRxInterpolant` (65.6%) and `_rwForOneEdge` (63.6%): the vertex, cursor and ring-index variables take other registers and the loop updates are scheduled differently |
| `rwsdk/src/pipe/p2/nodeImmMangleLineIndices.c` | 1 of 2 | `_ImmMangleLineIndicesNode` (96.9%: the doubled line count is recomputed for `RxClusterSetData` and `numUsed`, where the source's two `numLines * 2` share one register) |
| `rwsdk/src/pipe/p2/nodeImmMangleTriangleIndices.c` | 1 of 2 | `_ImmMangleTriangleIndicesNode` (87.6%: its locals take other saved registers, and the strip rewrite's source index is scheduled differently) |
| `rwsdk/world/pipe/p2/bapipew.c` | 5 of 5 | (Matching on GIHE78-rev0) |
| `rwsdk/world/pipe/p2/nodeLight.c` | 1 of 7 | `_LightFn` (99.2%) and the five `_rwApply*Light` functions (95.3-99.1%): register choice and the order of a few loads |
| `rwsdk/world/pipe/p2/nodePostLight.c` | 1 of 2 | `_PostLightFn` (94.6%: the target reloads each color channel after its clamp test; also on GIHE78-rev1) |
| `rwsdk/world/pipe/p2/gcn/wrldpipe.c` | 7 of 7 | (Matching on GIHE78-rev0; starts at `_rxWorldDevicePluginAttach`, `0x8015D830`) |
| `rwsdk/src/batextur.c` | 19 of 29 | `TextureDefaultNormalRead` (99.7%), `_rwTextureOpen` (99.6%), `TextureDefaultMipmapRead` (99.6%), `_rwTextureClose` (98.0%), `TextureImageReadAndSize` (97.5%), `PalettizeMipmaps` (94.2%), `RwTextureDestroy` (93.3%: the target loads the return value 1 before reading the reference count as well as at the end), `PalettizeImage` (91.4%), `TextureRasterDefaultBuildMipmaps` (87.0%) and `RwTextureRead` (86.3%), register choice and scheduling |
| `rwsdk/src/plcore/batkbin.c` | 8 of 8 | (Matching on GIHE78-rev0) |
| `rwsdk/src/plcore/bamatrix.c` | 15 of 21 | `MatrixOrthoNormalize` (98.2%) and `RwMatrixTransform` (98.0%), register choice and stack traffic; `_rwMatrixDeterminant` (94.3%: register choice), `RwMatrixRotateOneMinusCosineSine` (90.1%), `_rwMatrixOpen` (89.3%: the target loads the default tolerance before it stores the optimization flags) and `RwMatrixOptimize` (72.1%) |
| `rwsdk/src/plcore/bastream.c` | 5 of 7 | `RwStreamWrite` (99.9%: the operands of the file-write length compare are the other way round) and `RwStreamOpen` (94.5%) |
| `rwsdk/src/babinfrm.c` | 3 of 3 | (Matching on GIHE78-rev0, with its `.sdata2`) |
| `rwsdk/os/gcn/osintf.c` | 1 of 1 | (Matching on GIHE78-rev0) |
| `rwsdk/src/babbox.c` | 3 of 3 | (Matching on GIHE78-rev0) |
| `rwsdk/src/babincam.c` | 2 of 2 | (Matching on GIHE78-rev0) |
| `rwsdk/src/plcore/bamemory.c` | 5 of 8 | `RwFreeListCreate` (99.9%: the two error blocks' stack slots are in the reverse order), `_rwFreeListAllocReal` (97.7%) and `RwFreeListForAllUsed` (91.6%), where the target associates the block address arithmetic differently; `.sbss` `llGAllFreeLists` is in the split (round 28 sweep drafts) |
| `rwsdk/src/pipe/p2/p2altmdl.c` | 0 of 3 | `_rxPacketDestroy` (98.6%), `_rxEmbeddedPacketBetweenPipelines` (94.5%) and `_rxEmbeddedPacketBetweenNodes` (84.0%): the target clears the cluster flags with a word store where this source emits a halfword store, and register choice (text only) |
| `rwsdk/tool/slerp/rtslerp.c` | 0 of 1 | `RtQuatSetupSlerpCache` (99.3%: operand and register order in the inlined arc cosine's products; text only) |
| `rwsdk/src/babintex.c` | 7 of 9 | `_rwStringStreamFindAndRead` (99.0%: register choice in the inlined string readers) and `RwTextureStreamRead` (83.6%: the target keeps the inlined readers' NULL test and allocation path for its name buffers) |
| `rwsdk/src/baimage.c` | 9 of 18 | `ImageDetermineExtender` (99.1%), `RwImageCopy` (98.9%), `ImagePathForAllFullNames` (97.8%), `RwImageMakeMask` (97.4%), `ImageConvertDepth` (92.4%), `RwImageReadMaskedImage` (92.3%), `RwImageApplyMask` (89.2%), `RwImageAllocatePixels` (83.1%: the target compares the depth unsigned and computes the palette size without a branch) and `RwImageGammaCorrect` (79.1%), register choice and scheduling |
| `rwsdk/src/batypehf.c` | 2 of 2 | (Matching) |
| `rwsdk/src/plcore/babinary.c` | 14 of 14 | (Matching) |
| `rwsdk/src/plcore/bacolor.c` | 2 of 2 | (Matching, with `colorModule` in `.sbss`) |
| `rwsdk/src/baresamp.c` | 0 of 4 | `ImageResampleGetAvgPixel` (99.2%: register choice), `RwImageCreateResample` (88.7%: mostly register choice), `RwImageResample` (70.0%) and `ImageResampleGetSpan` (67.9%: the target unpacks a pixel's four bytes before converting them) |
| `rwsdk/world/basector.c` | 6 of 6 | (Matching on GIHE78-rev0, with `sectorTKList` and `sectorModule`) |
| `rwsdk/world/bamateri.c` | 8 of 8 | (starts at `0x8014D044` now that `_rpLightOpen` is in `balight.c`; Matching on GIHE78-rev0, with its `.data`, `.sbss` and `.sdata2`) |
| `rwsdk/world/balight.c` | 12 of 13 | `RpLightGetConeAngle` (99.9%) |
| `rwsdk/world/baclump.c` | 35 of 38 | `GeometryListStreamRead` (97.4%: the list's return is placed after the loop branch), `ClumpAtomicStreamRead` (91.1%: the two chunk structs take each other's stack slots) and `RpClumpStreamRead` (99.4%: the target keeps a second chunk version word) |
| `rwsdk/world/bageomet.c` | 9 of 12 | `RpGeometryCreate` (98.3%: the target keeps the format's two masks apart), `RpGeometryDestroy` (94.3%: the target loads its result at entry) and `RpGeometryStreamRead` (99.8%: the morph loop's two zeroes); starts at `_rpGeometryOpen`, `0x8014AFC4`, three functions before the size-sequence placement |
| `rwsdk/world/baworld.c` | 21 of 25 | `WorldSectorRender`, `RpWorldUnlock`, `RpWorldDestroy` and `RpWorldCreate` (94-99%: saved registers and store order) |
| `rwsdk/world/baworobj.c` | 48 of 48 | (Matching; starts at `WorldObjectOpen`, `0x8015275C`) |
| `rwsdk/world/bamatlst.c` | 4 of 5 | `_rpMaterialListStreamRead` (97.4%: the list, the stream and the material take other saved registers) |
| `rwsdk/world/bamesh.c` | 11 of 11 | (Matching on GIHE78-rev0, with `meshModule` and `MeshStatic`) |
| `rwsdk/world/bameshop.c` | 5 of 11 | `SortPolygonsInTriListMesh` (97.0%) and `TriStripMeshGenerate` (98.4%), register choice; `TriStripBinEntryArrayCreate` (88.1%: the target reloads the entry pointer inside the three inlined edge searches), `TriStripFollow` (90.0%) and `TriStripStripTris` (93.4%), where the target evaluates the edge tests into temporaries, and `TriStripJoin` (89.3%: the target reloads the joined strip's count and pointer per index); data open (`.data` and `.sdata2` not split) |
| `rwsdk/world/babinwor.c` | 9 of 11 | `WorldSectorStreamRead` (98.1%) and `RpWorldStreamRead` (98.0%), register choice in the polygon widening loop and the size computation; data open (`.sbss` not split) |
| `rwsdk/world/pipe/p2/gcn/instance/geominst.c` | 0 of 6 | `fn_80159380`, the vertex data sizer (89.9%), and the five `_rwGCNVtxFmtInst*` writers (71.5-92.6%): the same switches and unrolled loops; the target saves r26 or r27 to r31 through `_savegpr_*`, and register choice (round 27 sweep drafts; text only) |
| `rwsdk/world/pipe/p2/gcn/instance/instancegeom.c` | 0 of 1 | `_rwDlGeometryInstanceFast` (91.1%: register choice, and the target indexes the texture coordinate arrays through a computed offset); its inlined stride setter is `VtxDescInitFast`, bfbb's static there (text only) |
| `rwsdk/world/pipe/p2/gcn/instance/instanceworld.c` | 0 of 1 | `_rwDlWorldSectorInstanceFast` (92.7%, as `instancegeom.c`; text only) |
| `rwsdk/world/pipe/p2/gcn/instance/ibuffer.c` | 1 of 2 | `GCNDisplayListsBuild` (91.8%: register choice, and the target narrows each index with `clrlwi`; text only) |
| `rwsdk/world/pipe/p2/gcn/gclights.c` | 1 of 2 | `_rwGCLightsGlobalEnable` (96.9%: register choice for the lighting data and the light); `.data` `GCLightIDTable` and `.bss` `_RwGCLightObjs` are in the split |
| `rwsdk/world/pipe/p2/gcn/native.c` | 3 of 8 | `_rpNativeRead` (92.4%), `_rpNativeWrite` (90.2%), `_rpGeometryNativeSize` (86.4%), `_rpWorldSectorNativeSize` (90.8%) and `_rpWorldSectorNativeWrite` (90.0%): the target associates the instance size and alignment arithmetic differently, and register choice (text only) |
| `rwsdk/world/pipe/p2/gcn/gcpipe.c` | 7 of 10 | `fn_80158B20` (99.9%), `fn_80158FB0`, the default render callback (98.3%), and `_rxGCResEntryWaitDone` (96.4%: the target reloads the draw sync token for `GXSetDrawSync`); `.sdata2` (0x80262988 to 0x802629A0) not split |
| `rwsdk/world/pipe/p2/nodeAtomicEnumerateLights.c` | 4 of 4 | (`.data` 98.5%: the four bytes of padding at its end; `.sdata` in the split) |
| `rwsdk/world/pipe/p2/nodeMaterialScatter.c` | 2 of 2 | (`.data` 98.8%, as `nodeAtomicEnumerateLights.c`) |
| `rwsdk/world/pipe/p2/nodeWorldSectorEnumerateLights.c` | 2 of 3 | `WorldSectorEnumerateLightsNodeBody` (96.4%: the target keeps `params` in r29 and loads the sector straight after locking the cluster); `.data` and `.sdata` in the split |
| `rwsdk/world/pipe/p2/nodeAtomicInstance.c` | 1 of 5 | `fn_80154E8C` (98.1%), `AtomicInstanceNode` (92.4%), `fn_801551A8` (83.8%) and `fn_80154994` (73.8%), register choice and scheduling; `.data` in the split (with the body's jump table) |
| `rwsdk/world/pipe/p2/nodeWorldSectorInstance.c` | 1 of 4 | `WorldSectorInstanceNodeFn` (96.0%), `fn_80157A3C` (79.7%) and `fn_80157FB8` (79.5%), register choice and scheduling; `.data` and `.sdata` in the split, `.sdata2` not |
| `rwsdk/world/pipe/p2/gcn/vtxfmt.c` | 14 of 14 | (Matching on GIHE78-rev0, with its `.bss` and `.sbss`; the start edge stays open) |
| `rwsdk/plugin/hanim/stdkey.c` | 6 of 8 | `RpHAnimStdKeyFrameInterpolate` (95.2%) and `RpHAnimStdKeyFrameBlend` (94.1%): the target tests the 0.999 slerp threshold with `cror` and `beq`, and register choice |
| `rwsdk/plugin/hanim/rphanim.c` | 10 of 14 | `HAnimCopy`, `HAnimWrite` and `HAnimRead` (99.1-99.6%), register choice; `RpHAnimHierarchyCreate` (94.3%: the aligned matrix allocation) |
| `rwsdk/plugin/matfx/rpmatfx.c` | 41 of 48 | `MatFXMaterialCopy` (98.6%), `fn_8013E188` (98.2%), `fn_8013E5CC` (98.0%), `fn_8013DEC4` (97.7%), `_rpMatFXTextureMaskCreate` (85.2%), `fn_8013E76C` and `fn_8013E894` (78.4%), register choice and scheduling; data open (`.sdata2` and `MatFXInfo` not split) |
| `rwsdk/plugin/matfx/multiTex.c` | 15 of 17 | `MultiTextureCopy` (99.4%) and `MultiTextureStreamRead` (99.0%): the inlined create helper's size, platform entry and texture count take each other's registers |
| `rwsdk/plugin/matfx/multiTexEffect.c` | 12 of 14 | `RpMTEffectDestroy` (99.4%: the target forms the platform entry's address with `mulli` into other registers) and `RpMTEffectFind` (98.4%: the dictionary walk's cursor and effect take each other's registers, and the target has one more `mr`) |
| `rwsdk/plugin/skin2/rpskin.c` | 26 of 26 | (Matching on GIHE78-rev0; `_rpSkinGlobals` stays unsplit, since the source only declares it; the skin code after it reads it; starts at `SkinOpen`, `0x80144154`, since `SkinCreate` is inlined; ends after `RpSkinAtomicSetType`, `0x80144D5C`, an edge the unsplit code after it abuts) |
| `rwsdk/plugin/skin2/gcn/skingcn.c` | 4 of 8 | `fn_801450A8` (98.0%), `_rpSkinResEntryWaitDone` (96.4%), `fn_80145228` (96.6%) and `fn_80145598`, the reinstance callback (75.8%), differences not diagnosed yet; the skinning loop `fn_80144D5C` is an asm function (text only) |
| `rwsdk/plugin/skin2/gcn/skinstream.c` | 0 of 3 | `_rpSkinGeometryNativeRead` (99.3%), `_rpSkinGeometryNativeWrite` (94.4%) and `_rpSkinGeometryNativeSize` (83.4%), differences not diagnosed yet (text only) |
| `rwsdk/plugin/skin2/gcn/skingcng.c` | 2 of 3 | `_rpSkinPipelinesCreate` (71.2%, not diagnosed yet; text only) |
| `rwsdk/plugin/matfx/gcn/multiTexGcn.c` | 0 of 1 | `_rpMultiTexturePlatformPluginsAttach` (91.3%, not diagnosed yet; text only) |
| `rwsdk/plugin/matfx/gcn/multiTexGcnData.c` | 4 of 7 | `GameCubeMTEffectStreamRead` (98.2%), `GameCubeMTEffectStreamWrite` (86.5%) and `RpGameCubeMTEffectCreate` (85.5%), not diagnosed yet |
| `rwsdk/plugin/matfx/gcn/multiTexGcnPipe.c` | 3 of 9 | `_rpGameCubeMTMeshRenderCallBack` (94.7%), `GetTexFrameMatrix` (90.2%), `fn_80141958` (90.1%), `_rpGameCubeMTPipeDataQueryNBTs` (89.6%), `fn_80141D20` (82.8%) and `_rpGameCubeMTEffectSend` (68.7%), not diagnosed yet (text only) |
| `rwsdk/plugin/matfx/gcn/effectPipesGcn.c` | 10 of 15 | `fn_8013FEC0` (96.3%), `fn_80140AF4` (92.7%), `_rpGCMatFXAtomicReinstanceCallBack` (84.1%), `fn_80140178`, the MatFX render callback (78.7%), and `fn_8013FA64` (67.9%), not diagnosed yet (text only) |
| `rwsdk/world/pipe/p2/gcn/nodeGameCubeAtomicAllInOne.c` | 2 of 7 | `_rxGCAtomicAllInOneNode` (99.6%), `fn_8015DBA4` (95.3%), `fn_8015E054` (95.0%), `fn_8015DA50`, the lighting callback (86.3%), and `fn_8015E2E8`, the instance callback (73.6%), not diagnosed yet (text only) |
| `rwsdk/world/pipe/p2/gcn/nodeGameCubeWorldSectorAllInOne.c` | 3 of 5 | `_rxGCWorldSectorAllInOneNode` (96.6%) and `fn_8015E908`, the instance callback (75.5%), not diagnosed yet (text only) |
| `rwsdk/src/pipe/p2/gcn/nodeDolphinSubmitNoLight.c` | 3 of 4 | `DlSubmitNode` (51.3%: the target's loads around the volatile FIFO writes differ; text only) |
| `rwsdk/plugin/collis/ctgeom.c` | 7 of 7 | (NonMatching: this source pools `1.0` before `0.0` in `.sdata2`, the target the other way round) |
| `rwsdk/plugin/collis/ctworld.c` | 1 of 10 | the leaf callbacks, the old-sector walks and `WorldSectorForAllBoxedPrimitivePolyIntersections` (98.9-99.8%), register choice; `WorldForAllLineWorldSectorIntersections` (91.5%: the plane-crossing splits) and `RpCollisionWorldForAllIntersections` (96.2%: a 0x3E0-byte frame against the target's 0x3D0); the two static box helpers are `static inline`, as the target inlines them |
| `rwsdk/plugin/collis/ctbsp.c` | 7 of 8 | `_rpCollBSPTreeForAllLineLeafNodeIntersections` (95.1%: the inlined plane-crossing point keeps the split values in other registers and reloads the stored coordinate on other axes) |
| `rwsdk/plugin/collis/rpcollis.c` | 9 of 9 | (Matching on GIHE78-rev0, with its `.sbss`; ends after `RpCollisionPluginAttach`, `0x8013BD2C`) |
| `rwsdk/tool/intsec/rtintsec.c` | 1 of 2 | `RtIntersectionSphereTriangle` (97.5%: register choice) |
| `rwsdk/plugin/userdata/rpusrdat.c` | 12 of 17 | `UserDataListCopy`, `UserDataObjectStreamRead`, `UserDataObjectStreamWrite`, `UserDataObjectGetSize` and `RpGeometryGetUserDataArrayCount` (90-99%: the loop counter and the array offset take each other's registers; `UserDataObjectGetSize` also orders the string length sum `addi` then `add`) |
| `rwsdk/driver/common/palquant.c` | 6 of 17 | `nMaximize`, `CountLeafs` and `_rwPalQuantInit` (96.6-98.6%), register choice; `assignindex` and `addvolume` (98.0% and 98.3%: the box test's branch layout), `ExtractNodes` (97.2%: two loads swapped per compare), `AllocateToLeaf`, `nCut`, `_rwPalQuantAddImage`, `_rwPalQuantResolvePalette` and `_rwPalQuantMatchImage` (93.5-95.2%: reloads, product order and unsigned loop compares); starts at `InitLeaf`, `0x801714EC` |
| `rwsdk/driver/gcn/dltoken.c` | 3 of 4 | `_rwDlTokenQueryDone` (75.5%: the target reloads both draw sync tokens before each comparison); the three paired-single routines are assembly functions (text only) |
| `rwsdk/driver/gcn/dl2drend.c` | 2 of 6 | `_rwDlIm2DRenderTriangle` (96.7%), `_rwDlIm2DRenderLine` (96.9%), `_rwDlIm2DRenderPrimitive` (96.9%) and `_rwDlIm2DRenderIndexedPrimitive` (95.6%): register choice for the FIFO address and the color bytes (text only) |
| `rwsdk/driver/gcn/dlconvrt.c` | 13 of 25 | the image and raster converters at 88 to 97% (`_rwDlRasterPalletized4SetFromImage` 96.8%, `_rwDlRaster16SetFromImage` and `_rwDlRaster32SetFromImage` 96.4%, `_rwDlImage8GetFromRaster` 95.5%, `_rwDlImage32GetFromRaster` 95.4%, `_rwDlRasterSetFromImage` 95.3%, `_rwDlImage4GetFromRaster` 95.3%, `_rwDlImageFindFormat` 95.0%, `_rwDlRasterPalletized8SetFromImage` 93.1%, `_rwDlPixelToRGB` 92.7%, `_rwDlRGBToPixel` 88.3%, `_rwDlImageGetFromRaster` 88.2%): the conversion function goes through `r0`, the error blocks' stack slots are in another order, and register choice |
| `rwsdk/driver/gcn/dldevice.c` | 8 of 13 | `_rwDlRenderModeSelect` (64.5%: the draft recomputes the address of the render mode copy where the target reuses r8), `_rwDlRasterShowRaster` (77.3%: the target reloads the ring positions after each ring store, spills a local and does not keep the interrupt state), `_rwDlSystem` (97.0%: the draft reuses the compared draw sync token in its wait loop where the target reloads it, and schedules the half-frame setup differently), `_rwDlBreakPtCallback` (94.5%) and `_rwDlVIPreRetraceCallback` (94.3%), register choice (text only) |
| `rwsdk/driver/gcn/dlraster.c` | 10 of 17 | `DlRasterTile` (84.2%) and `DlRasterUntile` (82.3%): the tile loops pick other induction variables; the lock and unlock functions `fn_80179198` (96.3%) and `fn_80179614` (96.5%): register choice and an extra reload of the draw sync token in the wait; `fn_8017A374`, the raster create (91.0%: register choice for the zero stores); `fn_80179FA0`, the format picker and allocator (97.0%), and `fn_8017A4D8`, the raster destroy (98.3%), not diagnosed yet (round 30 sweep drafts; text only) |
| `rwsdk/driver/gcn/dlrendst.c` | 4 of 7 | `_rwDlRenderStateFogEnable` (96.4%), `RwGameCubeSetAlphaCompare` (93.8%) and `_rwDlSetRenderState` (94.0%), register choice, not diagnosed further (text only) |
| `rwsdk/driver/gcn/dlsprite.c` | 7 of 12 | `_rwDlRasterRenderQuad` (51.5%) and `_rwDlRasterCamera_ZClearRect` (76.1%: the target converts the rectangle's coordinates to float before its FIFO writes); `_rwDlRasterRenderQuadInit` (99.4%), `_rwDlRasterCamera_ZClearRectInit` (99.99%) and `_rwDlRasterClearGeneric` (98.1%), register choice (text only) |
| `rwsdk/driver/gcn/dltexdic.c` | 0 of 3 | `_rwDlNativeTextureGetSize` (93.8%), `fn_8017C9E4`, the native texture writer (81.8%), and `fn_8017CCBC`, the reader (93.9%), not diagnosed yet; the unit now ends at `0x8017CF40` (text only) |
| `rwsdk/driver/gcn/dltextur.c` | 4 of 6 | `_rwDlTextureSet` (77.5%) and `RwGameCubeTextureSetLOD` (86.4%), which inline the texture object rebuild; not diagnosed yet. The unit now starts at `fn_8017CF40` (`0x8017CF40`), the LOD bias rescale `_rwDlTextureSet` inlines (text only) |

Structure layouts come from the SLUS-20349 DWARF where it has the type (game code includes RenderWare's headers) and from the target's accesses otherwise; `evidence.tsv` has a `type` or `field` row for each. The GameCube layout can differ: `RwTexture.refCount` is at `+0x54` here and `+0x50` in the DWARF, and `RpClump` has a light list at `+0x10` and a camera list at `+0x18` (size `0x30`) where the DWARF's PS2 class has neither, as bfbb's GameCube header does. The size-sequence labels at `0x80149A4C` and `0x80149A80` were corrected to `RpClumpRemoveLight` and `RpClumpRemoveCamera` (the bodies use the light and camera extension offsets, in bfbb's order). `include/rwcore.h` and `include/rpworld.h` now declare their types with `typedef` so the same headers serve the C RenderWare units and the C++ game code. Type, member, typedef and macro names the DWARF lacks (`RwGlobals`, `RwDevice`, `RwError`, `RwPluginRegistry`, `RwPluginRegEntry`, the plugin, device and callback typedefs, `RWSRCGLOBAL`) follow bfbb's `include/rwsdk/rwplcore.h`, `rwcore.h` and `rpworld.h` (derived from the SDK headers) as related names. The file-local module data (`VectorModuleData`, `ErrorModuleData`, `MaterialModuleData`, `LightModuleData`, `TextureModuleData`, `FrameModuleData`, `CameraModuleData`, `WorldObjModuleData`, `MatrixModuleData`, `StreamModuleData`), `ClumpObjectExt`, `WorldSectorSearch`, the world-object extensions and ties (`WorldCameraExt`, `WorldObjectExt`, `WorldTie`, `LightTie`), the clump, geometry, mesh and world module data and stream structs (`ClumpModuleData`, `GeometryList`, `AtomicStreamData`, `AtomicStreamDataOld`, `ClumpStreamData`, `GeometryStreamData`, `MorphTargetStreamData`, `MeshGlobals`, `MeshStaticData`, the mesh optimiser's strip types (`TriStripBinEntry`, `TriStripEdge`, `TriStripFreeLists`, `TriStripList`, `MeshOp`), the palette quantizer's types (`PalQuantLeaf`, `PalQuantBranch`, `PalQuantBox`, `PalQuant`), `WorldModuleData`, `WorldListEntry`), the clump destroy callbacks `DestroyClumpLight` and `DestroyClumpCamera` and the round-19 constants and macros (`ATOMIC_OBJECTTYPE`, `CLUMP_OBJECTTYPE`, `GEOMETRY_OBJECTTYPE`, `ATOMIC_GEOMINDEXVERSION`, `CLUMP_ATOMICSONLYVERSION`, `GEOMETRY_VERTEXLIMIT`, `GEOMETRYLIST_DEINITIALIZE`, `SCALEMAX`), the world-object plugin ids and sector-walk constants, `BinaryChunkHeader`, `RasterModuleData`, `ImageModuleData`, `UserDataList` and the `UserDataStringCopy` macro, the material effect data (`MatFXMaterialData`, `MatFXEffect`, `MatFXBumpMapData`, `MatFXEnvMapData`, `MatFXDualData`, `MatFXModuleInfo`), `RpGameCubeVtxFmt`, the GameCube lighting data and all-in-one callbacks (`GCNLightingData`, `GCNAllInOnePrivateData`), the camera stream chunk (`CameraChunkInfo`), the animation frame data (`HAnimFrameData`, `HANIM_FRAMEDATA`), the standard keyframe (`HAnimStdKey`, its layout from the target's accesses), the collision sector-walk macros (`COLLSECTOR_*`), the skin data (`RpSkin`, `SkinGlobals`), the pipeline globals and execution context (`RxPipelineGlobals`, `RxExecContext`, `RXPIPELINEGLOBAL`), the cluster flags (`CLUSTER_*`), the multitexture data (`RpMultiTexture`, `MultiTextureRegEntry`, `RpMTEffect`, `RpMTEffectDict`, `MTEffectGlobals`, `MTEffectRegEntry`), the collision data flag `COLLISIONDATA_SINGLEBLOCK`, the private frame flags, the `STDFUNC_*` indices into `RwGlobals.stdFunc` and resource-heap types (`ResHeap`, `ResHeapBlock`) have no source and use this project's own names. Source-form details the target fixes:

- `_rwSqrt` and `_rwInvSqrt` reach the argument's bits through the SDK's own `RwSplitBits` union, whose `volatile` integer members give the target's reload of the word after each store; a plain `U32` pointer keeps it in a register (59% and 62%).
- `bavector.c`'s table builders take `sqrt` from the C branch of `include/libc/math.h` (the `frsqrte` form with four Newton steps the MSL math units use), which the target inlines.
- `basync.c`'s hierarchy walks take the parent frame into a local before `RwMatrixMultiply`; without it CodeWarrior inlines the recursion one level deeper than the target's six. `FrameSyncHierarchy` keeps the LTM flag in a `U8` local, which gives the target's registers.
- `RwRasterShowRaster` and `RwRasterCreate` load the device function into a local first, as the target does before `_rwResourcesPurge` and the raster's field stores.
- The resource heap tests a block's `used` word as `~used & 1` where the block must be free (`nor` then `clrlwi.`) and as `used & 1` where it must be in use (`clrlwi.` alone). `!(used & 1)` and `(used & 1) == 0` compile to `clrlwi.` with the opposite branch and do not match.
- `_rpClumpRegisterExtensions` ends with `return (_rpClumpLightExtOffset = RpLightRegisterPlugin(...)) >= 0;`: the target takes the sign bit before it stores the offset. Storing first and testing the global puts the store before the shift.
- `StrChr` and `StrRChr` copy the `char` argument into an `S32` local before the loop: the target sign-extends it once into `r5`, and comparing with the parameter directly swaps `r4` and `r5`.
- `baworobj.c` defines `_rpTieDestroy` and `_rpLightTieDestroy` before the extension deinitializers and `RpWorldRemoveLight`, which call them: the target has their bodies inline there, and keeps the global copies.
- The three tie-list walks in `baworobj.c` declare the list head before the cursor; the other order swaps their registers.
- `baworobj.c` defines its five `.sbss` statics in reverse address order (`worldObjModule` first): CodeWarrior emits them in reverse of their definition order.
- objdiff pairs functions and data by name, so a unit can score 100% and still not link byte-identically. The earlier attempts to link `baerr.c`, `batkreg.c`, `bamateri.c` and `vtxfmt.c` failed for two reasons. First, the data each source defines was not split to its unit, so the original copy stayed in a toolkit object beside the compiled one: a duplicate definition for globals, an extra copy for statics. Second, the functions or variables came out in a different order. The `cflags_rw` compiler emits uninitialized file-scope variables in the reverse of their definition order, so `rpcollis.c`, `batkreg.c`, `vtxfmt.c` and `badevice.c` define theirs in reverse address order. `bamateri.c`, `babinfrm.c` and `badevice.c` define their functions in the target's address order. Functions the target inlines and has no copy of (`RpMaterialCreate`, `_rwDeviceSystemRequest`, `EngineOpen`) are still compiled, and the linker strips them.
- `WorldLightSync` declares all its locals at the top and returns the light pointer cast to `RwObjectHasFrame*` on every path, so the argument and the light share a register; `WorldCameraSync` calls the saved sync as the `if` condition and returns early only on failure.
- `WorldSyncCamera` tests the frustum planes through the SDK's own `RwSplitBits` union, whose `volatile` integer members make the target's float store and integer reload; the box corner is a separate `RwV3d` temporary.
- `RpWorldAddClump` takes the clump's world extension as an `RpWorld**` local, which gives the target's indexed load (`lwzx`).
- `babinary.c` swaps 32-bit words with the plain C expression `(w >> 24) | ((w >> 8) & 0xFF00) | ((w << 8) & 0xFF0000) | (w << 24)`, which CodeWarrior compiles to `stwbrx`. The `__stwbrx` intrinsic gives the same instruction, but in a lighter loop that every GC compiler unrolls eight times instead of the target's four. The swap and float-conversion loops reuse `size` as their count, which the inlined copies need.
- `ChunkIsComplex` sets an `isComplex` local in each case and returns it once. It compiles to the same code as returning from each case, but the larger source keeps it out of line in `RwStreamReadChunkHeaderInfo`, as in the target.
- `_rwStreamReadChunkHeader` stores the read-size comparison in a local before testing it, which gives the target's `cntlzw` form.
- `RpMatFXMaterialGetEffects`, `RpMatFXMaterialSetupBumpMap` and `RpMatFXMaterialSetupEnvMap` test results with `== NULL`, which branches over the NULL return as the target does; `!` places the NULL return after the success path.
- `RpMatFXAtomicEnableEffects`, `RpMatFXWorldSectorEnableEffects` and `RpGameCubeGeometrySetVtxFmt` take the address of the object's plugin slot into a pointer local, so the offset stays in one register across the call; `RpMatFXMaterialSetBumpMapTexture` converts the raster width as unsigned (`U32` cast), as the target's conversion constant shows.
- `VectorMultPoint` and `VectorMultVector` declare `z, y, x` then `outX, outY, outZ` and build each output in place (`outX = x * right.x; outX += y * up.x;`); other declaration orders or one expression per component give the same instructions with other registers.


The flags are shown compatible with the target, not established. MSL is one profile for the whole library: the 52 imported units match under GC/1.3 with `-inline auto,deferred`, `-char signed` and `-fp_contract on`, twelve of them differ under GC/1.3.2, and none needs GC/1.3.2. Ten of the fdlibm units need `-fp_contract on` (fused multiply-add), and every other MSL unit compiles identically with it on or off. Each later MSL import reconfirms the profile on its own unit.

Bink is ProDG (SN Systems GCC) code, built through `tools/prodg_cc.py`. `fft.c` compiles to its unit under every ProDG version from 3.5 to 3.9.3, and Ooura's FFT compiles to none of its functions under CodeWarrior, so the family is established but the version is not. `cflags_bink` is likewise shown compatible, not established: it rests on `fft.c` only. `ProDG/3.5` is the only version the wrapper supports; the first import whose code separates the versions settles it.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK and CARD units therefore carry `comment:8` in `splits.txt`.

CARD is the SDK's debug build ([splits.md](splits.md#card)). `cflags_card` compiles it at `-opt level=0 -inline off -schedule off -sym on -char signed` with `DEBUG=1`, the same flags as libmap's `card_debug` profile, so its assertions are compiled in, whatever `--debug` the build uses.

## Imported units

| Unit | Compiler | Data | Source |
| --- | --- | --- | --- |
| `Runtime.PPCEABI.H/__va_arg.c` | GC/1.3.2 (with `cflags_runtime`, 1.3 differs and 1.3.2 through 2.7 are identical; with `-char signed` added, 1.3 also matches) | none | C |
| `Runtime.PPCEABI.H/global_destructor_chain.c` | GC/1.3.2 (1.3 also matches) | `.dtors`, `.sbss` | C |
| `Runtime.PPCEABI.H/runtime.c` | GC/1.3.2 (1.3 also matches) | `.rodata` | asm functions, each with an `assembly` row in `evidence.tsv` |
| `Runtime.PPCEABI.H/GCN_mem_alloc.c` | GC/1.3.2 (1.3 and 2.0 also match) | `.rodata` | C |
| `TRK_MINNOW_DOLPHIN/` `mainloop.c`, `nubevent.c`, `nubinit.c`, `msg.c`, `serpoll.c`, `usr_put.c`, `dispatch.c`, `msghndlr.c`, `mutex_TRK.c`, `notify.c`, `mem_TRK.c`, `main_TRK.c`, `targcont.c`, `target_options.c` | GC/1.3 | each unit's `.rodata`, `.data` and `.bss`, as `data` rows in `evidence.tsv` | C |
| `MSL_C.PPCEABI.bare.H/abort_exit.c` | GC/1.3 (msl profile) | `.bss`, `.sbss` | C |
| `MSL_C.PPCEABI.bare.H/alloc.c` | GC/1.3 (msl profile) | `.rodata`, `.bss`, `.sbss` | C |
| `MSL_C.PPCEABI.bare.H/ansi_files.c` | GC/1.3 (msl profile) | `.data`, `.bss` | C |
| `MSL_C.PPCEABI.bare.H/ansi_fp.c` | GC/1.3 (msl profile) | `.rodata`, `.data`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/arith.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/buffer_io.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/char_io.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/ctype.c` | GC/1.3 (msl profile) | `.data` | C |
| `MSL_C.PPCEABI.bare.H/direct_io.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/e_acos.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/e_asin.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/e_atan2.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/e_exp.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/e_fmod.c` | GC/1.3 (msl profile) | `.rodata` | C |
| `MSL_C.PPCEABI.bare.H/e_pow.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/e_rem_pio2.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/file_io.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/FILE_POS.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/k_cos.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/k_rem_pio2.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/k_sin.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/k_tan.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/math_ppc.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/mbstring.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/mem.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/mem_funcs.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/misc_io.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/printf.c` | GC/1.3 (msl profile) | `.rodata`, `.data`, `.sdata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/qsort.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/rand.c` | GC/1.3 (msl profile) | `.sdata` | C |
| `MSL_C.PPCEABI.bare.H/s_atan.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_copysign.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/s_cos.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_floor.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_frexp.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_ldexp.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_modf.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/s_nextafter.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/s_sin.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/s_tan.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/scanf.c` | GC/1.3 (msl profile) | `.rodata`, `.data` | C |
| `MSL_C.PPCEABI.bare.H/string.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata` | C |
| `MSL_C.PPCEABI.bare.H/strtold.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/strtoul.c` | GC/1.3 (msl profile) | `.data` | C |
| `MSL_C.PPCEABI.bare.H/uart_console_io.c` | GC/1.3 (msl profile) | `.sbss` | C |
| `MSL_C.PPCEABI.bare.H/w_acos.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/w_asin.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/w_atan2.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/w_exp.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/w_fmod.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/w_pow.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/wchar_io.c` | GC/1.3 (msl profile) | none | C |
| `TRK_MINNOW_DOLPHIN/` `flush_cache.c`, `targimpl.c`, `dolphin_trk.c`, `mpc_7xx_603e.c`, `dolphin_trk_glue.c` | GC/1.3 | `data` rows in `evidence.tsv` | C, and asm functions, each with an `assembly` row in `evidence.tsv` |
| `TRK_MINNOW_DOLPHIN/targsupp.s` | assembled | none | assembly, with an `assembly` row per stub |
| `TRK_MINNOW_DOLPHIN/__exception.s` | assembled | none | assembly (the exception vector table), with an `assembly` row |
| `TRK_MINNOW_DOLPHIN/msgbuf.c` | GC/1.3 | `.rodata`, `.bss` | C |
| `TRK_MINNOW_DOLPHIN/mslsupp.c` | GC/1.3 | none | C |
| `dolphin/card/*.c` (16 objects) | GC/1.2.5n, `card_debug` flags | `.data`, `.sdata`, `.bss` | C ([CARD](#card)) |
| `dolphin/` SDK objects (`ai`, `amcstubs`, `ar`, `base`, `db`, `si`, `mtx`, `ax`, `dsp`, `dvd`, `exi`, `pad`, `vi`, `OdemuExi2`, `odenotstub`, `os` from `OS.c` to `OSTime.c`, and `gx` from `GXInit.c` to `GXPerf.c`) | GC/1.2.5n, `cflags_sdk` | each unit's sections, as `data` rows | C; asm functions where the original was assembly, each with an `assembly` row |

All of them are `Matching` and link byte-identical, except `dolphin/os/OSMemory.c`, `OSReset.c` and `OSRtc.c`, `dolphin/vi/vi.c`, and `TRK_MINNOW_DOLPHIN/msgbuf.c` and `mslsupp.c`. Each of the three os units compiles to the target's code except one function's stack frame, which is smaller than the target's unless an unused local is added (CREDITS.md, "Dolphin SDK"), so they stay `NonMatching` and the original bytes stay linked. `msgbuf.c` matches in 15 of its 19 functions: the compiler inlines `TRKAppendBuffer` into the other four, where the target calls it, and only a per-function `#pragma dont_inline` changes that (the `source-form` row for the unit in `evidence.tsv`), so it stays `NonMatching` too. `mslsupp.c` matches in eight of its nine functions: `convertFileMode` ends the binary flag with a narrowing that only sonicheroes' no-op `& 0xFF` mask reproduces, which this project does not use (the `source-form` row), so it stays `NonMatching` as well. `vi.c` matches in 14 of its 18 functions: without ac-decomp's `#pragma dont_inline` around `getTiming`, the compiler inlines `getTiming` into `__VIInit`, `VIInit` and `VIConfigure`, where the target calls it, and `VIGetNextField` needs ac-decomp's unused `u8 unused[4]` local, so it stays `NonMatching` too (unit row). `Pad.c` keeps the write-only `chanShift` local in `PADRead` that ac-decomp, tww, bfbb and FFCC-Decomp have; the owner accepted it on that evidence (source-form row). `__DSPHandler` keeps an unused `tmp_task`, `__DSP_boot_task` a write-only `mail`, `__fstLoad` keeps a write-only `arenaHi`, and `__OSInitAudioSystem` keeps a write-only `errFlag`, each under the write-only locals rule ("Importing a unit") with a source-form row. The AX units come from bfbb's AX files, which are from a 2003 SDK, cut back to the 2001 AX the target links: a 0xC0-byte `AXPB` with no low-pass filter, two output frames with no buffer mode or thread queue, no Dolby Pro Logic II or compressor commands, and a single 32-entry mix-cycle table. Each difference has a `source-form` or `type` row, and `__AXGetAuxBInput` tests the AuxA callback as the target does. `axDspSlave`, the DSP microcode image, and `axDspSlaveLength` stay unsplit data. The MSL units build under the `msl` profile in `configure.py`: GC/1.3 with `-inline auto,deferred` and `-char signed`. The `compiler` row in `evidence.tsv` records why.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Dolphin SDK public headers under the SDK's names (`dolphin/os.h` and its `dolphin/os/*.h` parts, `dolphin/db.h`, `dolphin/si.h`, ...), each created by the first import that needs it. The SDK's base types are in `dolphin/types.h`. |
| `include/libc/` | MSL C headers, shared with game code (`ctype.h`, `eof.h`, `errno.h`, `float.h`, `limits.h`, `locale.h`, `math.h`, `mem_funcs.h`, `pool_alloc.h`, `stdarg.h`, `stddef.h`, `stdio.h`, `stdlib.h`, `string.h`) |
| `include/runtime/` | Runtime library headers (`NMWException.h`) |
| `src/<library>/` | Private headers of one library, next to its source, included as `"<library>/<header>"` through an `-i src` path in that library's flags (`cflags_sdk` for `dolphin/dvd/__dvd.h` and `dolphin/gx/__gx.h`, `cflags_trk` for `TRK_MINNOW_DOLPHIN/trk.h`, `cflags_card` for `dolphin/card/__card.h`, `cflags_msl` for `MSL_C.PPCEABI.bare.H/ansi_files.h`, `ansi_fp.h`, `buffer_io.h`, `char_io.h`, `direct_io.h`, `fdlibm.h`, `file_io.h` and `scanf.h`) |

Dolphin SDK code uses the SDK's type names. The runtime and MSL use plain C types. Game code keeps `types.h`.

When an import needs a declaration:

- Search the existing headers first, and extend the header that should own the declaration rather than adding a second copy.
- Add declarations in the order the original header would, so that parallel imports touch different lines.
- Record each header's upstream in CREDITS.md.

## Importing a unit

- Every function copied from an upstream is reviewed against this project's standards, whichever repository it comes from. A match in an upstream is evidence that the code exists, not that its source is acceptable.
- Check the upstream's licence at the pinned commit, and the licence of any code that the upstream itself reused.
- Originally handwritten assembly needs provenance (CONTRIBUTING.md, "Compiler versions and flags"). A library function may be linked as assembly only when the original library shipped it as assembly, shown by the original library itself (a vendor-shipped `.s` file, for example) or by the target. A decompilation's `.s` file or `asm` block is that project's choice and is never provenance, and a failed match is never evidence of assembly. Each such function also needs an evidence row showing that no compiler emits it, and review checks every one, as the project owner decided for library assembly. `asm` blocks in C source that the compiler builds are compiled source, not this exception.
- Check the unit's locals against the owner's 2026-10-27 write-only locals rule ([CONTRIBUTING.md](../CONTRIBUTING.md#write-only-and-unused-locals)): compile with MWCC `-w unusedvar`, scan for locals that are set but never read, and report each hit in the PR as deleted, dropped as padding, or kept with a `source-form` row.
- Map the unit's data sections when its source compiles, and record them in `evidence.tsv`. Each section's start is where the target's relocations to it land, at the offsets the compiled object's relocations give. Its end is the compiled size, rounded up to the next object when padding follows. One `data` row per section names the unit and section. Where a data symbol in `symbols.txt` spans the edge, split it there.

## CARD

The CARD source is ac-decomp's, revised against the target (CREDITS.md, "Dolphin SDK"). Points that apply to the debug build:

- `DEBUG=1` selects the SDK's debug `os.h`, where `OSPhysicalToCached` and `OSCachedToPhysical` are functions rather than macros. The target calls them, so `include/dolphin/os.h` declares the function forms under `DEBUG`.
- An assertion's message is its condition as text, so the spacing inside the condition is part of the target's `.data` (`(u32) card->size`). Do not run `clang-format` over these files: it rewrites those conditions.
- The sources keep functions the linker strips, such as `CARDFastOpen`. With `-str reuse` the compiler pools each object's strings in order of first use, and a stripped function can be the first user of a string that kept code shares (`CARDFastOpen` asserts `0 <= chan && chan < 2` before `CARDOpen` does), so only the full source reproduces each `.data` section. These functions have no code in the target to check them against.
- `CardData`, the 352-byte DSP program in `CARDUnlock.c`, is binary data. `config/GIHE78-rev0/config.yml` extracts it from `main.dol` at build time (`extract`, raw header `dolphin/card/CardData.inc`), and `CARDUnlock.c` includes it, so the bytes are never committed.
- The Bink logo movie at `0x801CDF48` contains a word that looks like a pointer into CARD's `.data`. Its symbol is marked `noreloc`, so the build does not turn that word into a relocation against a CARD symbol.

## Bink

No public decompilation supplies Bink source this project can import:

- bfbb (`d8e8eac`) has the same Bink build (see [Bink units](splits.md#bink-units)), but 10 of its 15 Bink files are empty and the rest are unfinished. Its `src/bink/include/bink.h`, `radbase.h` and `rad3d.h` are RAD Game Tools' SDK headers (`radbase.h` carries RAD's copyright string), and its `binkread.c` embeds RAD's logo movie as `LogoData`. bfbb's CC0 dedication cannot cover RAD's material, so none of its code, header text or movie data is copied. The exception is bfbb's own `binkngc.c` and `binkngc.h`, which carry no RAD material: `binkngc.c` here follows them for the memory functions, as CREDITS.md records. The project owner allowed names-only use of `bink.h`'s struct and field names: the Bink headers keep this project's own names, and each type and field row in `evidence.tsv` gives the `bink.h` name it corresponds to ([Bink SDK names](../CREDITS.md#bink-sdk-names)).
- tssm (`fc32870`) has a Bink header and no Bink source.

`bink/src/sdk/fft.c` is Takuya Ooura's general-purpose FFT package (`fftsg.c`) built for `float`: the function names match, and nine functions of the current release (`fft.tgz`, SHA-256 `52bb637c70b971958ec79c9c8752b1df5ff0218a4db4510e60826e0cb79b5296`) compile to the target unchanged apart from the type. Five Bink functions (`cftrec1`, `cftrec2`, `cftexp1`, `cftexp2`, `cftfx42`) follow the package's pre-2006 structure and are fitted to the target. The licence on Ooura's FFT page allows use, copying, modification and distribution of modified code for any purpose, on condition that modified code refers to the package. His notice is in CREDITS.md, not in the source, as the project owner decided. `src/bink/src/sdk/fft.c` reproduces the unit's whole `.text` and its `.rodata` under every ProDG version from 3.5 to 3.9.3, so it does not settle the version either. It is `Matching`: the link resolves its calls to the MSL float wrappers (`atanf`, `cosf` and `sinf`, `0x800F38F8`, `0x800F38D4` and `0x800F38B0`) by name, and the MSL import names them.

`bink/shared/time/radcb.c`, `bink/src/sdk/popmal.c` and `bink/src/sdk/varbits.c` are this project's own reconstructions from the target, and are `Matching`. Each compiles to its unit's whole `.text` and data under every ProDG version from 3.5 to 3.9.3, byte for byte once the relocations are resolved, so they do not settle the version either. Five details of their source come from the target's code, not from style:

- `radcb.c`: the `state` and `suspends` counters of a handler and of a callback are `volatile`. The target reloads each one after storing it and keeps every access in source order, which only a volatile access does.
- `radcb.c` reads the bus clock at `0x800000F8` as a member of a structure at `0x80000000`. The target loads it with `lis 0x8000` and `lwz 0xF8` and no relocation. GCC folds a constant pointer such as the SDK's `*(u32*)(OS_BASE_CACHED | 0x00F8)` into `lis`/`ori` and a zero-offset load. An extern `__OSBusClock` would give the right instructions, but it would need a symbol at an address outside every section.
- `popmal.c` calls `memcpy` and `radcb.c` calls `memset` with no prototype in scope. The target sets up both calls with `crclr 6`, which GCC emits only for a call to an unprototyped or variadic function. So neither file includes `string.h`.
- `varbits.c` indexes `VarBitsLens` in its 8-bit loop through a local, `len = 8`, rather than the literal `VarBitsLens[8]`. The target keeps the byte offset in a callee-saved register: it saves `r31` in a 16-byte frame, sets `li r31,32` before the loop and loads with `lwzx r0,r12,r31`. With the literal, GCC folds the offset into `lwz r0,32(r12)` and drops the frame, and 11 lines differ. No literal or `const` form reproduces the target, so the original indexed through a non-constant width at that point. A form that uses the width everywhere, masks included, does not reproduce it either, so the exact original form is not established.
- `varbits.c` declares `_bitlevels` 64-byte aligned. That alignment is what places it at `+0xC0` after `VarBitsLens` and pads the section to `0x180` bytes, which is where `fft.c`'s `.rodata` starts.

`bink/src/sdk/dct.c` is a `NonMatching` partial reconstruction. Its 8x8 transforms are the Independent JPEG Group's fast integer DCTs, `jpeg_fdct_ifast` (`jfdctfst.c`) and `jpeg_idct_ifast` (`jidctfst.c`) from IJG release 6b, which RAD adapted. The reconstruction follows IJG 6b statement for statement and keeps IJG's variable names; RAD's changes, which the target shows, are listed with IJG's notice in CREDITS.md ([Bink DCT](../CREDITS.md#bink-dct)). `FastFDCT8x8`, `FastFDCTs8x8` and the three table wrappers (`FastIDCT8x8`, `FastIDCT8x8d`, `FastmIDCT8x8`) compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do the four quantisation tables in `.rodata`. Three functions still differ:

- `fastidct8x8`: the column pass matches except for the registers. The row pass spills fewer values into the `t[8]` array than the target does, so it is 14 instructions short.
- `fastidct8x8d` writes each output pixel twice across and each row twice down, as 32-bit words. Its shape is close, but the register allocation differs throughout.
- `FastmIDCT8x8WithMotion` adds each output pixel to an 8-byte-stride motion block. It has the same register allocation and spill differences as `fastidct8x8`.

The 8 zero bytes between `fft.c`'s `.rodata` and the tables (`0x801A8E18`-`0x801A8E20`) are unassigned. The object's `.rodata` has 4-byte alignment, so the tables will not land at `0x801A8E20` until the source or the split accounts for those bytes.

`bink/src/sdk/decode/ngc/binkngc.c` is a `NonMatching` partial reconstruction of Bink's GameCube platform layer: the 64-bit multiply and divide helpers, the timers and the memory hooks. `radmemset16`, `RADSetMemory`, `radmalloc`, `radfree`, `RADSetAudioMemory`, `radaudiomalloc` and `radaudiofree` compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so does the unit's `.sdata` (`0x8025E6F8`-`0x8025E710`). Three details of the source come from the target:

- The time base reads (`mftbu`, `mftb`, `mftbu`, retried while the upper halves differ) and the four `cntlzw` instructions in `mult64anddiv` and `div64` are inline `asm`. GCC has no C form for `mftb`, and it emits `cntlzw` from C only with extra instructions the target lacks. The project owner approved inline asm for exactly these two instructions, with a `source-form` row in `evidence.tsv` for each use.
- `div64` and `ReadTimeBase` are plain (non-static) `inline` functions defined before their callers. GCC 2.95 inlines only a function it has already seen, and emits the out-of-line copies at the end of the file, which is where the target has them.
- The statics are written with an explicit `= 0`, which places them in `.sdata` rather than `.sbss`, and the file-scope hooks are defined just before `RADSetMemory`, so that `RADTimerRead`'s function-scope `starttime` comes first, as in the target.

Nine functions still differ. `mult64anddiv`, `mult64andshift` and `div64` build their 64-bit products and differences in registers that are not consecutive pairs (`mulhwu`/`mullw` and `subfc`/`subfe` into unrelated registers), which GCC's `DImode` values never do, so the original likely wrote them as assembly; no C form tried reproduces them, and any further asm needs the owner's approval first. `RADTimerRead`, the four cycle-timer functions and `ReadTimeBase` have the target's shape, within one instruction of its length, but allocate registers differently around the time base reads and the 64-bit arithmetic.

`bink/src/sdk/decode/ngc/ngcfile.c` is Bink's GameCube file reader: it streams a movie from the disc through a read-ahead buffer with asynchronous DVD reads, or reads it directly when no buffer is set. It is this project's own reconstruction from the target, and its whole `.text` compiles to the unit under every ProDG version from 3.5 to 3.9.3, byte for byte once the relocations are resolved. The reader's state block is declared in `include/bink/binkio.h` as `BinkIO`, with project names; its rows give the matching `BINKIO` names from RAD's `bink.h` ([Bink SDK names](../CREDITS.md#bink-sdk-names)). Four details of the source come from the target:

- `intelendian` swaps each word with `lwbrx`, written as one inline `asm` statement, since GCC 2.95 has no C form for the instruction.
- The `BinkIO` counters and three private fields are `volatile`; for all but `idle_time` and `bg_time` the target reloads them after each store or keeps their accesses in order (the volatile row lists which access decides each).
- `memmove` is declared in the file and `string.h` is not included. The `crclr 6` before `memcpy` and `memset` comes from GCC's builtins for them, so compatible prototypes or none give the same bytes; `string.h`'s `size_t` prototypes conflict with the builtins.
- `BinkFileReadFrame` uses one scratch local for three short-lived values, `BinkFileIdle`'s switch names the three DVD states it ignores, and `BinkFileSetInfo` masks the size at each use. Each changes the register allocation or the code layout, as the `source-form` rows in `evidence.tsv` record.

`bink/src/sdk/decode/binkacd.c`, Bink's audio decoder, is this project's own reconstruction from the target and is `NonMatching`. Seven of its eight functions compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do its `.rodata` (`0x801A8730`-`0x801A8790`) and `.data` (`rlelens`, `bandtopfreq` and `invertbins`, `0x801DC240`-`0x801DC380`). `quanttos16chans2` has the target's instructions except that the loop counter and the second channel's offset swap registers, which adds one `mr`; the cause is not established. Four details of the source come from the target's code:

- Its one-bit reads (`VARBITSGET1`) are a conditional expression, not a statement. The target tests the run flag and negates the sign bit inside each arm of the refill, which GCC does only for a conditional expression used as a whole.
- `BinkAudioDecompressOpen` takes two square roots inline with `frsqrte` and three Newton steps. GCC 2.95 has no C form for `frsqrte`, so the estimate is one `__asm__` statement in a `static inline` helper, and the steps are C.
- `CLAMP16` casts the in-range value to `S16`. The cast gives the clamp its own result register, as the target has.
- The file does not include `string.h`, whose `size_t` prototypes conflict with GCC's builtin `memset` and `memcpy` and change both functions that call them. Compatible prototypes or none give the target's bytes.

`bink/src/sdk/decode/ngc/ngcyuy2.c` converts decoded YUV 4:2:0 rows to the GameCube's YUY2 format, two output rows per call, with luma-only and pixel-doubling variants. It is this project's own reconstruction from the target and is `Matching`: its whole `.text` compiles to the target under ProDG 3.5 to 3.9.3. It reads and advances the YUV state `S`, which belongs to `yuv.cpp`, which `include/bink/yuvstate.h` declares. Its four helpers are plain `inline` functions, which is what gives the target both the inlined bodies and the out-of-line copies at the end of the unit. The signedness of each count and the separate locals in the odd-word tails also come from the target's code (see the source-form rows).

`bink/src/sdk/decode/ngc/ngcsnd.c` is Bink's GameCube sound backend: `BinkOpenNGCSound` returns its `Open`, which fills a `BinkSnd` with eight callbacks and streams the decoded samples through two halves of a staging buffer to ARAM with ARQ, playing them on one AX voice per channel. It is this project's own reconstruction from the target (bfbb's `ngcsnd.c` is empty) and is `Matching`: its whole `.text` and its `.rodata` compile to the target under ProDG 3.5 to 3.9.3. `include/bink/binksnd.h` declares the whole 0x178-byte `BinkSnd`, because `Open` clears it with `memset(snd, 0, sizeof(*snd))`. The Bink API functions that drive the backend use the fields this unit does not touch. `Open` is declared with the six arguments BinkOpen passes when it calls it through the `BinkSndOpen` pointer (`blrl` at `0x80114D90`, with the open flags in `r7`); this unit uses the first four. The backend's private state (`NGCSND`) lives in `BinkSnd.priv` and is reached through a cast of `snd->priv`. Its nine helpers are plain `inline` functions, which gives the target both the inlined bodies and the out-of-line copies at the end of the unit. The volatile lock index, the `U16` voice state, the missing `string.h` and the statement forms in `NGC_SoundInit`, `Lock`, `Unlock`, `NGC_StarvedClear` and `Ready` come from the target's code (see the source-form rows).

`bink/src/sdk/decode/ngc/ngcrgb.c` is a `NonMatching` reconstruction of Bink's YUV to RGB converters, which write the decoded planes into GameCube 4x4 tiles (RGBA8 tiles are 64 bytes, an AR plane then a GB plane; 16-bit tiles are 32 bytes). 18 of its 25 functions compile to the target byte for byte under ProDG 3.5 to 3.9.3: the 32-bit colour, mono and alpha converters, `YUV_16m_4x2`, `YUV_16mx2_4x2`, the eight empty `_odd` converters and `GetTiledRgbLoc`. The rest compile to the target's instructions with a different register allocation (`YUV_16_4x2_even`, `YUV_16x2_4x2_even`, `YUV_16a4_4x2_even`, `YUV_16a4x2_4x2_even`, `YUV_32ax2_4x2_even`) or a different setup schedule (`YUV_16a4m_4x2`, `YUV_16a4mx2_4x2`); the cause is not established. The register allocation fixes several details of the source:

- Plane pointers are taken from `S` in the order `u`, `v`, `y0`, `y1`, with `a0` and `a1` first in the alpha converters, and stored back as `u`, `v`, `y0`, `y1`, `a0`, `a1`. In the exact converters the target fixes the order of the loads and stores and the declaration order of `u` and `v`, but not the order of the `*p++` reads in the loop, except in `YUV_16a4m_4x2` and `YUV_16a4mx2_4x2`, which read `a0`, `y0`, `a1`, `y1`. `YUV_16m_4x2` and `YUV_16mx2_4x2` take `y1` before `y0`.
- Each pixel pair computes its red and blue table offsets before the green sum. Which of red and blue comes first is not fixed by the target.
- The 16-bit converters take each luma halfword into a `U16` local and read a pixel as the low halfword of a `U32` table entry with a `(U16)` cast, which the target loads with `lhz` at offset 2.
- `YUV_32m_4x2` loads all four `mono32` entries of a row before it writes any of them.

The source also keeps two of the library's own bugs, which the target code shows: the second row of `YUV_32mx2_4x2` and `YUV_32amx2_4x2` reads luma bytes 1 and 2 and then 2 and 3, and `YUV_32amx2_4x2` uses alpha bytes 0 and 1 for both pixel pairs of a row. The YUV tables are zero in `main.dol`, since `YUV_init` fills them at run time, so their sizes come from bfbb's data extents.

Under GCC, `dolphin/types.h` defines `AT_ADDRESS` as empty, so the ProDG units can include `dolphin/os.h`. The MW units are unchanged.

`bink/src/sdk/decode/binkread.c`, Bink's public API (opening, decoding, timing, sound tracks and statistics), is this project's own reconstruction from the target and is `NonMatching`. 47 of its 53 functions compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do its `.rodata`, `.bss`, `.sdata` and `.sbss` and the `TrackNums` table in `.data`. `BinkCopyToBufferRect`, `BinkDoFrame`, `BinkGetKeyFrame`, `smallestrect`, `BinkOpenTrack` and `BinkSetSoundOnOff` differ in register choice, block order or scheduling; each function row says how, and none of the causes is established. The unit would stay `NonMatching` without them, because its `.data` also holds the logo movie. Some details of the source come from the target's code:

- `BinkOpen`'s failure paths share code through `goto` labels placed inside the first error block of each kind, and the preload failure frees the stack copy's `planes[0]`, as the target does.
- `BinkGetRealtime` fills the result's `frame_rate_div` from `frame_rate`, and `BinkNextFrame` sets `io.busy` to 1 and straight back to 0.
- The file does not include `string.h`, whose `size_t` prototypes conflict with GCC's builtin `memset` and `memcpy` and change six functions. It declares `memmove` itself.

`bink/src/sdk/decode/expand.c`, the video plane decoder (bundle readers, `ExpandPlane` and `ExpandBink`), is this project's own reconstruction from the target and is `NonMatching`. bfbb's `expand.c` holds only the unit's tables, as strings, and is not used: the tables here are written from the target's bytes. 15 of its 16 functions compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do its `.rodata`, `.data` and `.sdata2`. `ExpandPlane` has the target's structure in every block type, but its register allocation differs in several of them and it is 4 bytes longer; its function row says how, and the cause is not established. Some details of the source come from the target's code:

- The 8-row copies and fills advance two pointers in turn, each set from the other right after its row is written, and the raw 8x8 block reads its rows by index from the color bundle, which the target reloads after every store.
- The motion source of types 2, 4 and 7 is computed in one expression, so both offset bundles are read before either pointer is stored.
- The scratch block `tmp` is written by byte through a `U8` pointer; plain subscripts of a `U8` array change the code. It is declared as 8 doubles, for the alignment of its 8-byte copies, though `U8 tmp[64]` with the same writes compiles the same. The 16x16 fill writes its first and last rows around a 7-iteration loop, and the pattern index is signed.
- `ExpandPlane` declares `readcolors`, `rowadd` and `maskpitch` in the order of their stack slots, and `OpenReadBundle` computes the last index in a local before masking it to 16 bits.
- Two 8-byte zero objects, `huff4decode00align` and `huff4decodesalign` (bfbb's names), precede `rlelens` in `.sdata2`, where the target has them.

`bink/src/sdk/decode/yuv.cpp`, Bink's blitter layer (`YUV_init`, `YUV_blit`, `YUV_blit_mask`, the unaligned row and column converters and the per-format `BLITS` tables that pick the `ngcrgb.c` and `ngcyuy2.c` converters), is this project's own reconstruction from the target (bfbb's `yuv.cpp` is empty) and is `NonMatching`. It is C++, built by ProDG's `cc1plus` with the C flags. 89 of its 97 functions compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do its `.data`, `.bss`, `.sdata` and `.sbss`. `YUV_blit`, `YUV_blit_mask` and `YUV_init` have the target's instructions with a different register allocation, and the five YUY2 mono column converters differ in a few instructions; the causes are not established (function rows). Some details of the source come from the target's code:

- The zero tables are initialised (`= { 0 }`) and 32-byte aligned, which puts them in `.data` as the target has them; GCC would put uninitialised ones in `.bss`.
- `S` owns its row pointers as `U8*`, which the row functions step by bytes, so `ngcrgb.c` and `ngcyuy2.c` scale their steps to bytes. `YUVSTATE` names the three colour offset indexes at `+0x20` (`g`, `b`, `r`).
- `setevenodd` tests the mode as an `if` chain and chains its mono stores (`ODD = EVEN = ...`), `YUV_blit` copies the caller's pitch before doubling it, `YUV_blit_mask` reuses one local for two short-lived values, and `YUV_init` takes an `S32` layout and builds its tables in the target's block order (`source-form` rows).
- The converters take a `long` count, as their mangled C++ callers' types show, and the file-local functions take `unsigned long` arguments.
- `radmem.h` and `binkngc.h` wrap their declarations in `extern "C"` so the C++ unit links to the C units.

`bink/src/sdk/bitplane.c`, Bink's bit-plane coefficient coder, is this project's own reconstruction from the target (bfbb's `bitplane.c` is empty) and is `Matching`: its whole `.text` compiles to the unit under every ProDG version from 3.5 to 3.9.3. It codes an 8x8 block's coefficients in zigzag order one bit plane at a time, from the highest plane down, keeping a list of pending groups (three 16-coefficient regions and the first coefficients or quad) that is split into quads and single coefficients as they become significant. `LenBPLossless` counts the bits `WriteBPLossless` would write for a 16-bit block, `WriteBPLossy` writes an 8-bit block and returns 0 for an all-zero one, and `ReadBPLossless`, `ReadBPLossy` and `ReadBPLossyWithMotion` decode them; the lossy readers share the file-local `readlossy`. The object has no data of its own: it reads `zigzag` (in `dct.c`'s `.rodata` in bfbb's split) and `VarBitsLens` from `varbits.c`. Some details of the source come from the target's code:

- The three encoders count leading zeros with `cntlzw`, written as an `__asm__` statement in the `CNTLZW` macro that `expand.c` also uses; the project owner approved this use, with one row per instruction.
- The list entries are `U8` in the readers (index above a 2-bit type) and `U16` in the encoders (index, type and length byte), and the list grows downwards from the middle of a stack buffer. `readlossy` keeps the bit count, the entries and the indices in one `U8` variable, and each pass of `ReadBPLossless` declares its own entry variable.
- `LenBPLossless` casts each magnitude to `U16` before counting, the encoders compare a new entry's length byte and rebuild the entry for the store, `VARBITSPUT` copies its bit count into a local, `WriteBPLossy` increments its refinement index inside the subscript, and `memset` is declared without `string.h`. Each changes the code, as the `source-form` rows in `evidence.tsv` record.

Bink headers are this project's own, declaring only what the reconstructed units use. The logo movie stays out of the repository: `binkread.c` declares `LogoData` `extern`, and its bytes come from `main.dol` at build time.
