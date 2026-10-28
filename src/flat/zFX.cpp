#include "iFX.h"
#include "xString.h"
#include "zScene.h"

extern "C" {
void fn_8002A5F4(RwTexture*);
void fn_8002AC2C(RpWorld*);
void fn_8002AC30(RpWorld*);
}

// Set up scene effects and warping for the tide or tree entity.
void zFX_SceneEnter(RpWorld* world)
{
    fn_8002A5F4(NULL);
    fn_8002AC30(world);

    iFXWarbModes mode = IFX_WARB_WATER_WAVE;
    xBase* ent = zSceneFindObject(xStrHash("tide"));
    if (ent != NULL)
    {
        mode = IFX_WARB_WATER_WAVE;
    }
    else
    {
        ent = zSceneFindObject(xStrHash("tree1"));
        if (ent != NULL)
        {
            mode = IFX_WARB_TREE;
        }
    }

    if (ent != NULL)
    {
        iFXWarbInit(ent, mode);
    }
}

// Stop the scene's warping effect.
void zFX_SceneExit(RpWorld* world)
{
    fn_8002AC2C(world);
    iFXWarbSetParams(IFX_WARB_NONE, NULL);
}
