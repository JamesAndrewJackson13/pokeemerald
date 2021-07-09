#include "global.h"
#include "malloc.h"
#include "text.h"
#include "battle.h"
#include "battle_info_utils.h"
#include "window.h"

#define TEXT_COLOR_BG     0
#define TEXT_COLOR_FG     1
#define TEXT_COLOR_SHADOW 2

#define BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT           \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_LIGHT_GRAY,      \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_DARK_GRAY,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1          \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_GREEN,           \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL     \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_DYNAMIC_COLOR_2,       \
    [TEXT_COLOR_SHADOW] = TEXT_DYNAMIC_COLOR_3,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_FG]     = TEXT_COLOR_DARK_GRAY,       \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_GRAY,      \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES       \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_FG]     = TEXT_COLOR_BLUE,            \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_BLUE,      \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2          \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_DARK_GRAY,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_HEADER            \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_DARK_GRAY,       \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_GRAY,      \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_TERRAIN           \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = 8,                          \
    [TEXT_COLOR_SHADOW] = 3,                          \
}

// Text colors for BG, FG, and Shadow in that order
static const u8 sFontColorTable[][3] =
{
    BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT,
    BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1,
    BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL,
    BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS,
    BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES,
    BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2,
    BATTLE_MON_TEXT_COLOR_ARRAY_HEADER,
    BATTLE_MON_TEXT_COLOR_ARRAY_TERRAIN,
};

const u8* GetFontColor(u8 which)
{
    return sFontColorTable[which];
}

struct Pokemon* GetPokemonAtSlot(u8 slot)
{
    if (slot % 2)
    {
        return &gEnemyParty[gBattlerPartyIndexes[slot]];
    }
    else
    {
        return &gPlayerParty[gBattlerPartyIndexes[slot]];
    }
}


static const u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];
const u16* GetPalBuffer(void)
{
    return palBuffer;
}

// what the hell is 'c'?
void BlitBitmapToMonWindow(u8* bgGfxTilemap, u8 windowId, const u8* b, u8 c, u8 w, u8 h)
{
    logDebug("BlitBitmapToMonWindow");
    u8* pixels = AllocZeroed(h * w * 32);
    u8 i, j;

    if (pixels != NULL)
    {
        // TOP ROW
        // Top left
        CpuCopy16(&bgGfxTilemap[(b[0] << 5)], &pixels[0], 32);
        // Top middle
        for (j = 1; j < w - 1; j++)
            CpuCopy16(&bgGfxTilemap[(b[1] << 5)], &pixels[j * 32], 32);
        // Top right
        CpuCopy16(&bgGfxTilemap[(b[2] << 5)], &pixels[(w - 1) * 32], 32);

        // MIDDLE ROWS
        for (i = 1; i < h - 1; i++)
        {
            // Middle left
            CpuCopy16(&bgGfxTilemap[(b[3] << 5)], &pixels[w * i * 32], 32);
            // Middle middle
            for (j = 1; j < w - 1; j++)
                CpuCopy16(&bgGfxTilemap[(b[4] << 5)], &pixels[(w * i + j) * 32], 32);
            // Middle right
            CpuCopy16(&bgGfxTilemap[(b[5] << 5)], &pixels[(w * i + w - 1) * 32], 32);
        }

        // BOTTOM ROW
        // Top left
        CpuCopy16(&bgGfxTilemap[(b[6] << 5)], &pixels[((h - 1) * w) * 32], 32);
        // Top middle
        for (j = 1; j < w - 1; j++)
            CpuCopy16(&bgGfxTilemap[(b[7] << 5)], &pixels[((h - 1) * w + j) * 32], 32);
        // Top right
        CpuCopy16(&bgGfxTilemap[(b[8] << 5)], &pixels[(h * w - 1) * 32], 32);

        BlitBitmapToWindow(windowId, pixels, 0, 0, w * 8, h * 8);
        Free(pixels);
    }
}

#undef BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT
#undef BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1
#undef BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL
#undef BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS
#undef BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES
#undef BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2
#undef BATTLE_MON_TEXT_COLOR_ARRAY_HEADER

#undef TEXT_COLOR_BG
#undef TEXT_COLOR_FG
#undef TEXT_COLOR_SHADOW