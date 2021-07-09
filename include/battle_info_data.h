#ifndef GUARD_BATTLE_INFO_DATA_H
#define GUARD_BATTLE_INFO_DATA_H

// Passed data
#define battleInfoMonIndex    data[1]

static const struct BgTemplate sBgTemplates_BattleInfoData[4] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 27,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 25,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

// Mon window's height/width
#define DATA_MON_WINDOW_HEIGHT             3
#define DATA_MON_WINDOW_WIDTH             13
// Stage state window's height/width
#define STAGE_STATE_WINDOW_HEIGHT          2
#define STAGE_STATE_WINDOW_WIDTH          13
// Terrain description window
#define TERRAIN_WINDOW_HEIGHT             15
#define TERRAIN_WINDOW_WIDTH              16

#define MON_WINDOW_BG         0
#define HP_LVL_WINDOW_BG      0
#define STATS_WINDOW_BG       2
#define CUR_STATE_WINDOW_BG   2
#define DESCRIPTION_WINDOW_BG 1

static const struct WindowTemplate sBattleInfoDataWindows[] =
{
    // Mon Window
    {
        .bg = MON_WINDOW_BG,
        .tilemapTop = 1,
        .tilemapLeft = 1,
        .width = DATA_MON_WINDOW_WIDTH,
        .height = DATA_MON_WINDOW_HEIGHT,
        .paletteNum = 0x4,
        .baseBlock = 0x103,
    },
    // HP / Level
    {
        .bg = HP_LVL_WINDOW_BG,
        .tilemapTop = 0,
        .tilemapLeft = 20,
        .width = 7,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 0xE7,
    },
    // Stat Numbers
    {
        .bg = STATS_WINDOW_BG,
        .tilemapTop = 5,
        .tilemapLeft = 27,
        .width = 2,
        .height = 15,
        .paletteNum = 0xF,
        .baseBlock = 0xCB,
    },
    // Current State 1
    {
        .bg = CUR_STATE_WINDOW_BG,
        .tilemapTop = 6,
        .tilemapLeft = 1,
        .width = STAGE_STATE_WINDOW_WIDTH,
        .height = STAGE_STATE_WINDOW_HEIGHT,
        .paletteNum = 0x6,
        .baseBlock = 0xB1,
    },
    // Current State 2
    {
        .bg = CUR_STATE_WINDOW_BG,
        .tilemapTop = 9,
        .tilemapLeft = 1,
        .width = STAGE_STATE_WINDOW_WIDTH,
        .height = STAGE_STATE_WINDOW_HEIGHT,
        .paletteNum = 0x7,
        .baseBlock = 0x97,
    },
    // Current State 3
    {
        .bg = CUR_STATE_WINDOW_BG,
        .tilemapTop = 12,
        .tilemapLeft = 1,
        .width = STAGE_STATE_WINDOW_WIDTH,
        .height = STAGE_STATE_WINDOW_HEIGHT,
        .paletteNum = 0x8,
        .baseBlock = 0x7D,
    },
    // Current State 4
    {
        .bg = CUR_STATE_WINDOW_BG,
        .tilemapTop = 15,
        .tilemapLeft = 1,
        .width = STAGE_STATE_WINDOW_WIDTH,
        .height = STAGE_STATE_WINDOW_HEIGHT,
        .paletteNum = 0x9,
        .baseBlock = 0x63,
    },
    // Description Window
    {
        .bg = DESCRIPTION_WINDOW_BG,
        .tilemapTop = 5,
        .tilemapLeft = 15,
        .width = TERRAIN_WINDOW_WIDTH,
        .height = TERRAIN_WINDOW_HEIGHT,
        .paletteNum = 0x0,
        .baseBlock = 0x134,
    },
    // Button Info
    {
        .bg = HP_LVL_WINDOW_BG,
        .tilemapTop = 18,
        .tilemapLeft = 1,
        .width = 5,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 0x12A,
    },
    DUMMY_WIN_TEMPLATE
};

#undef MON_WINDOW_BG
#undef HP_LVL_WINDOW_BG
#undef STATS_WINDOW_BG
#undef CUR_STATE_WINDOW_BG

#define MON_POKEBALL 16, 25
#define MON_POKEMON  16, 20
#define MON_HELDITEM 22, 28
#define MON_STATUS   48, 26

static const u8 sBattleInfoData_SpriteCoords[4 * 2] = {
    MON_POKEMON, MON_HELDITEM, MON_STATUS, MON_POKEBALL
};

// Tile nums
static const u8 sTerrainEffectFilledTileNums[] =  {  8,  9, 10,
                                                    18, 19, 20,
                                                    28, 29, 30 };

void Task_OpenBattleInfoDataDebug(u8 taskId);
void Task_OpenBattleInfoData(u8 taskId);

#endif //GUARD_BATTLE_INFO_DATA_H