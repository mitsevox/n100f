#include "iFX.h"
#include "xString.h"
#include "zScene.h"

extern "C" {
void fn_8002A5F4(U32);
void fn_8002AC2C();
void fn_8002AC30(RpWorld*);
}

// Set up warping for the tide or tree entity in this scene.
void zFX_SceneEnter(RpWorld* world)
{
    fn_8002A5F4(0);
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
    fn_8002AC2C();
    iFXWarbSetParams(IFX_WARB_NONE, NULL);
}
