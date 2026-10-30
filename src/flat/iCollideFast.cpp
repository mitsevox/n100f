#include "types.h"

struct _xScene;
struct RpIntersection;
struct RpWorldSector;
struct RpCollisionTriangle;

static U32 hitzhit;

void iCollideFastInit(_xScene* sc)
{
}

// The fast ray tests only ask whether anything is hit, so the first triangle ends the walk.
static RpCollisionTriangle* rayHitsModelFastCB(RpIntersection* isx, RpCollisionTriangle* tri,
                                               F32 dist, void* data)
{
    hitzhit = 1;
    return NULL;
}

static RpCollisionTriangle* rayHitsEnvFastCB(RpIntersection* isx, RpWorldSector* sector,
                                             RpCollisionTriangle* tri, F32 dist, void* data)
{
    hitzhit = 1;
    return NULL;
}
