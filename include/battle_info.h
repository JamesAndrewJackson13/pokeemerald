#ifndef GUARD_BATTLE_INFO_H
#define GUARD_BATTLE_INFO_H

#include "graphics.h"


// Mon window's height/width
#define SELECTION_MON_WINDOW_HEIGHT          3
#define SELECTION_MON_WINDOW_WIDTH          10

#define TITLE_BAR      \
{                      \
    .bg = 0,           \
    .tilemapLeft = 0,  \
    .tilemapTop = 0,   \
    .width = 13,       \
    .height = 3,       \
    .paletteNum = 0xF, \
    .baseBlock = 0xDB, \
}

#define PLAYER_MON_1                             \
{                                                \
    .bg = 0,                                     \
    .tilemapLeft = 1,                            \
    .tilemapTop = 5,                             \
    .width = SELECTION_MON_WINDOW_WIDTH,         \
    .height = SELECTION_MON_WINDOW_HEIGHT,       \
    .paletteNum = 4,                             \
    .baseBlock = 0x63,                           \
}

#define PLAYER_MON_2                             \
{                                                \
    .bg = 0,                                     \
    .tilemapLeft = 2,                            \
    .tilemapTop = 9,                             \
    .width = SELECTION_MON_WINDOW_WIDTH,         \
    .height = SELECTION_MON_WINDOW_HEIGHT,       \
    .paletteNum = 6,                             \
    .baseBlock = 0x9F,                           \
}

#define OPPONENT_MON_1                           \
{                                                \
    .bg = 0,                                     \
    .tilemapLeft = 18,                           \
    .tilemapTop = 12,                            \
    .width = SELECTION_MON_WINDOW_WIDTH,         \
    .height = SELECTION_MON_WINDOW_HEIGHT,       \
    .paletteNum = 5,                             \
    .baseBlock = 0x81,                           \
}

#define OPPONENT_MON_2                           \
{                                                \
    .bg = 0,                                     \
    .tilemapLeft = 19,                           \
    .tilemapTop = 8,                             \
    .width = SELECTION_MON_WINDOW_WIDTH,         \
    .height = SELECTION_MON_WINDOW_HEIGHT,       \
    .paletteNum = 7,                             \
    .baseBlock = 0xBD,                           \
}

static const struct WindowTemplate sSinglesBattleWindowTemplate[] =
{
    PLAYER_MON_1,
    OPPONENT_MON_1,
    TITLE_BAR,
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sDoublesBattleWindowTemplate[] =
{

    PLAYER_MON_1,
    OPPONENT_MON_1,
    PLAYER_MON_2,
    OPPONENT_MON_2,
    TITLE_BAR,
    DUMMY_WIN_TEMPLATE
};

static const struct OamData sOamData_MenuPokeball =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sPokeballAnim_Closed[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sPokeballAnim_Open[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd* const sSpriteAnimTable_MenuPokeball[] =
{
    sPokeballAnim_Closed,
    sPokeballAnim_Open
};

static const struct CompressedSpriteSheet sSpriteSheet_MenuPokeball =
{
    gPartyMenuPokeball_Gfx, 0x400, 0x04b0
};

static const struct CompressedSpritePalette sSpritePalette_MenuPokeball =
{
    gPartyMenuPokeball_Pal, 0x04b0
};

// Used for the pokeball sprite on each party slot / Cancel button
static const struct SpriteTemplate sSpriteTemplate_MenuPokeball =
{
    .tileTag = 0x04b0,
    .paletteTag = 0x04b0,
    .oam = &sOamData_MenuPokeball,
    .anims = sSpriteAnimTable_MenuPokeball,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_MenuPokeballSmall =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSmallPokeballAnim_Closed[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Open[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Blank1[] =
{
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Blank2[] =
{
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Blank3[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Blank4[] =
{
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_END
};

// The blanks below are never used. See SpriteCB_BounceConfirmCancelButton, where they were intended to be used
static const union AnimCmd* const sSpriteAnimTable_MenuPokeballSmall[] =
{
    sSmallPokeballAnim_Closed,
    sSmallPokeballAnim_Open,
    sSmallPokeballAnim_Blank1,
    sSmallPokeballAnim_Blank2,
    sSmallPokeballAnim_Blank3,
    sSmallPokeballAnim_Blank4
};

static const struct CompressedSpriteSheet sSpriteSheet_MenuPokeballSmall =
{
    gPartyMenuPokeballSmall_Gfx, 0x0300, 0x04b1
};

// Used for the pokeball sprite next to Cancel and Confirm when both are present, otherwise sSpriteTemplate_MenuPokeball is used
static const struct SpriteTemplate sSpriteTemplate_MenuPokeballSmall =
{
    .tileTag = 1201,
    .paletteTag = 1200,
    .oam = &sOamData_MenuPokeballSmall,
    .anims = sSpriteAnimTable_MenuPokeballSmall,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};


static const struct OamData sOamData_HeldItem =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_HeldItem[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HeldMail[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END
};

static const union AnimCmd* const sSpriteAnimTable_HeldItem[] =
{
    sSpriteAnim_HeldItem,
    sSpriteAnim_HeldMail,
};

static const struct SpriteTemplate sSpriteTemplate_HeldItem =
{
    0xd750,
    0xd750,
    &sOamData_HeldItem,
    sSpriteAnimTable_HeldItem,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};

static const struct OamData sOamData_StatusCondition =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_StatusPoison[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusParalyzed[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusSleep[] =
{
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFrozen[] =
{
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusBurn[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusPokerus[] =
{
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFaint[] =
{
    ANIMCMD_FRAME(24, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Blank[] =
{
    ANIMCMD_FRAME(28, 0),
    ANIMCMD_END
};

static const union AnimCmd* const sSpriteTemplate_StatusCondition[] =
{
    sSpriteAnim_StatusPoison,
    sSpriteAnim_StatusParalyzed,
    sSpriteAnim_StatusSleep,
    sSpriteAnim_StatusFrozen,
    sSpriteAnim_StatusBurn,
    sSpriteAnim_StatusPokerus,
    sSpriteAnim_StatusFaint,
    sSpriteAnim_Blank
};

static const struct CompressedSpriteSheet sSpriteSheet_StatusIcons =
{
    gStatusGfx_Icons, 0x400, 1202
};

static const struct CompressedSpritePalette sSpritePalette_StatusIcons =
{
    gStatusPal_Icons, 1202
};

static const struct SpriteTemplate sSpriteTemplate_StatusIcons =
{
    .tileTag = 1202,
    .paletteTag = 1202,
    .oam = &sOamData_StatusCondition,
    .anims = sSpriteTemplate_StatusCondition,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// Palette offsets
static const u8 sGenderPalOffsets[] = { 11, 12 };
static const u8 sHPBarPalOffsets[] = { 9, 10 };
static const u8 sPartyBoxPalOffsets1[] = { 4, 5, 6 };
static const u8 sPartyBoxPalOffsets2[] = { 1, 7, 8 };
static const u8 sPartyBoxNoMonPalOffsets[] = { 1, 11, 12 };

// Palette ids
static const u8 sGenderMalePalIds[] = { 59, 60 };
static const u8 sGenderFemalePalIds[] = { 75, 76 };
static const u8 sHPBarGreenPalIds[] = { 57, 58 };
static const u8 sHPBarYellowPalIds[] = { 73, 74 };
static const u8 sHPBarRedPalIds[] = { 89, 90 };
static const u8 sPartyBoxEmptySlotPalIds1[] = { 52, 53, 54 };
static const u8 sPartyBoxMultiPalIds1[] = { 68, 69, 70 };
static const u8 sPartyBoxFaintedPalIds1[] = { 84, 85, 86 };
static const u8 sPartyBoxCurrSelectionPalIds1[] = { 116, 117, 118 };
static const u8 sPartyBoxCurrSelectionMultiPalIds[] = { 132, 133, 134 };
static const u8 sPartyBoxCurrSelectionFaintedPalIds[] = { 148, 149, 150 };
static const u8 sPartyBoxSelectedForActionPalIds1[] = { 100, 101, 102 };
static const u8 sPartyBoxEmptySlotPalIds2[] = { 49, 55, 56 };
static const u8 sPartyBoxMultiPalIds2[] = { 65, 71, 72 };
static const u8 sPartyBoxFaintedPalIds2[] = { 81, 87, 88 };
static const u8 sPartyBoxCurrSelectionPalIds2[] = { 97, 103, 104 };
static const u8 sPartyBoxSelectedForActionPalIds2[] = { 161, 167, 168 };
static const u8 sPartyBoxNoMonPalIds[] = { 17, 27, 28 };

static const struct BgTemplate sBgTemplates_BattleInfo[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 1
    }
};

#define PLAYER_MON_1_POKEBALL 16, 57
#define PLAYER_MON_1_POKEMON  16, 52
#define PLAYER_MON_1_HELDITEM 22, 60
#define PLAYER_MON_1_STATUS   48, 58

#define PLAYER_MON_2_POKEBALL 24, 89
#define PLAYER_MON_2_POKEMON  24, 80
#define PLAYER_MON_2_HELDITEM 30, 92
#define PLAYER_MON_2_STATUS   56, 90

#define OPPONENT_MON_1_POKEBALL 152, 113
#define OPPONENT_MON_1_POKEMON  152, 104
#define OPPONENT_MON_1_HELDITEM 158, 116
#define OPPONENT_MON_1_STATUS   184, 114

#define OPPONENT_MON_2_POKEBALL 160, 81
#define OPPONENT_MON_2_POKEMON  160, 72
#define OPPONENT_MON_2_HELDITEM 162, 84
#define OPPONENT_MON_2_STATUS   188, 82


// Each layout array has an array for each of the 6 party slots
// The array for each slot has the sprite coords of its various sprites in the following order
// Pokemon icon (x, y), held item (x, y), status condition (x, y), menu pokeball (x, y)
static const u8 sBattleInfoSelect_SpriteCoords_Single[2][4 * 2] =
{
    {  PLAYER_MON_1_POKEMON,   PLAYER_MON_1_HELDITEM,   PLAYER_MON_1_STATUS,   PLAYER_MON_1_POKEBALL},
    {OPPONENT_MON_1_POKEMON, OPPONENT_MON_1_HELDITEM, OPPONENT_MON_1_STATUS, OPPONENT_MON_1_POKEBALL},
};

static const u8 sBattleInfoSelect_SpriteCoords_Double[4][4 * 2] =
{
    {  PLAYER_MON_1_POKEMON,   PLAYER_MON_1_HELDITEM,   PLAYER_MON_1_STATUS, PLAYER_MON_1_POKEBALL},
    {OPPONENT_MON_1_POKEMON, OPPONENT_MON_1_HELDITEM, OPPONENT_MON_1_STATUS, OPPONENT_MON_1_POKEBALL},
    {  PLAYER_MON_2_POKEMON,   PLAYER_MON_2_HELDITEM,   PLAYER_MON_2_STATUS, PLAYER_MON_2_POKEBALL},
    {OPPONENT_MON_2_POKEMON, OPPONENT_MON_2_HELDITEM, OPPONENT_MON_2_STATUS, OPPONENT_MON_2_POKEBALL},
};

#undef PLAYER_MON_1_POKEBALL
#undef PLAYER_MON_1_POKEMON
#undef PLAYER_MON_1_HELDITEM
#undef PLAYER_MON_1_STATUS
#undef PLAYER_MON_2_POKEBALL
#undef PLAYER_MON_2_POKEMON
#undef PLAYER_MON_2_HELDITEM
#undef PLAYER_MON_2_STATUS
#undef OPPONENT_MON_1_POKEBALL
#undef OPPONENT_MON_1_POKEMON
#undef OPPONENT_MON_1_HELDITEM
#undef OPPONENT_MON_1_STATUS
#undef OPPONENT_MON_2_POKEBALL
#undef OPPONENT_MON_2_POKEMON
#undef OPPONENT_MON_2_HELDITEM
#undef OPPONENT_MON_2_STATUS

#undef TITLE_BAR
#undef PLAYER_MON_1
#undef PLAYER_MON_2
#undef OPPONENT_MON_1
#undef OPPONENT_MON_2

void Task_OpenBattleInfo(u8 taskId);
void CB2_OpenBattleInfo(void);

#endif //GUARD_BATTLE_INFO_H