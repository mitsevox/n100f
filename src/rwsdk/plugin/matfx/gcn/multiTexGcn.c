#include <rpmtex.h>

// The GameCube multitexture plugins: the generic plugin, the material registration for this
// platform, then the GameCube data and pipeline plugins.

S32 _rpGameCubeMTDataPluginAttach(void);
S32 _rpGameCubeMTPipePluginAttach(void);

S32 _rpMultiTexturePlatformPluginsAttach(void)
{
    S32 result;

    if (!_rpMultiTexturePluginAttach())
    {
        return FALSE;
    }

    if (!_rpMaterialRegisterMultiTexturePlugin(6, 0x129, 0xC))
    {
        return FALSE;
    }

    if (!_rpGameCubeMTDataPluginAttach())
    {
        return FALSE;
    }

    result = _rpGameCubeMTPipePluginAttach();
    return result;
}
