#include "zFX.h"

#include "iFX.h"
#include "xFX.h"
#include "xString.h"
#include "zScene.h"

// Set up scene effects and warping for the tide or tree entity.
void zFX_SceneEnter(RpWorld* world)
{
    xFXanimUV2PSetTexture(NULL);
    xFX_SceneEnter(world);

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
    xFX_SceneExit(world);
    iFXWarbSetParams(IFX_WARB_NONE, NULL);
}
