#include <rwcore.h>

// The standard pipeline clusters: each names the per-vertex or per-packet data a pipeline node
// can read or write, with no default stride, attributes or attribute set.
static char _RxObjSpace3DVertices_csl[] = "RxObjSpace3DVertices.csl";
RxClusterDefinition RxClObjSpace3DVertices = { _RxObjSpace3DVertices_csl, 0, 0, NULL };

static char _RxCamSpace3DVertices_csl[] = "RxCamSpace3DVertices.csl";
RxClusterDefinition RxClCamSpace3DVertices = { _RxCamSpace3DVertices_csl, 0, 0, NULL };

static char _RxScrSpace2DVertices_csl[] = "RxScrSpace2DVertices.csl";
RxClusterDefinition RxClScrSpace2DVertices = { _RxScrSpace2DVertices_csl, 0, 0, NULL };

static char _RxInterpolants_csl[] = "RxInterpolants.csl";
RxClusterDefinition RxClInterpolants = { _RxInterpolants_csl, 0, 0, NULL };

static char _MeshState_csl[] = "MeshState.csl";
RxClusterDefinition RxClMeshState = { _MeshState_csl, 0, 0, NULL };

static char _RenderState_csl[] = "RenderState.csl";
RxClusterDefinition RxClRenderState = { _RenderState_csl, 0, 0, NULL };

static char _Indices_csl[] = "Indices.csl";
RxClusterDefinition RxClIndices = { _Indices_csl, 0, 0, NULL };

static char _RxScatter_csl[] = "RxScatter.csl";
RxClusterDefinition RxClScatter = { _RxScatter_csl, 0, 0, NULL };

static char _RxTriPlanes_csl[] = "RxTriPlanes.csl";
RxClusterDefinition RxClTriPlanes = { _RxTriPlanes_csl, 0, 0, NULL };

static char _RxUVs_csl[] = "RxUVs.csl";
RxClusterDefinition RxClUVs = { _RxUVs_csl, 0, 0, NULL };

static char _RxVSteps_csl[] = "RxVSteps.csl";
RxClusterDefinition RxClVSteps = { _RxVSteps_csl, 0, 0, NULL };

static char _RGBAs_csl[] = "RGBAs.csl";
RxClusterDefinition RxClRGBAs = { _RGBAs_csl, 0, 0, NULL };

static char _CamNorms_csl[] = "CamNorms.csl";
RxClusterDefinition RxClCamNorms = { _CamNorms_csl, 0, 0, NULL };
