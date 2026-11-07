#include "xColor.h"

// The engine's named colors. The PS2 DWARF lists 33 of them; these five are the ones the game
// uses, the rest are stripped by the linker.
const iColor_tag g_BLACK = { 0, 0, 0, 255 };
const iColor_tag g_NEON_RED = { 255, 32, 0, 255 };
const iColor_tag g_NEON_GREEN = { 32, 255, 0, 255 };
const iColor_tag g_MAROON = { 128, 0, 0, 255 };
const iColor_tag g_PIMP_GOLD = { 215, 220, 19, 255 };
