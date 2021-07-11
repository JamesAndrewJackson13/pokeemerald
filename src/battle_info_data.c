#include "global.h"
#include "window.h"
#include "graphics.h"
#include "bg.h"
#include "dma3.h"
#include "malloc.h"
#include "random.h"
#include "util.h"
#include "m4a.h"
#include "decompress.h"
#include "palette.h"
#include "main.h"
#include "menu.h"
#include "terrain_effects.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_main.h"
#include "battle_interface.h"
#include "battle_anim.h"
#include "battle_info_utils.h"
#include "battle_info_mon_window.h"
#include "battle_info_utils.h"
#include "battle_info.h"
#include "reshow_battle_screen.h"
#include "data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "task.h"
#include "text_window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "party_menu.h"
#include "overworld.h"
#include "scanline_effect.h"
#include "battle_info_data.h"
#include "pokemon_icon.h"
#include "sound.h"
#include "sprite.h"
#include "pokemon_summary_screen.h"
#include "custom_utils.h"
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// MACROS
// How many windows are in use
#define NUMBER_OF_WINDOWS (9)

// Pallets for the type icons
#define TYPE_ICON_PAL_NUM_0     (13)
#define TYPE_ICON_PAL_NUM_1     (14)
#define TYPE_ICON_PAL_NUM_2     (15)

// x and y pos for the type sprites
#define TYPE_SPRITE_1_X (174)
#define TYPE_SPRITE_2_X (214)
#define TYPE_SPRITES_Y  (34)

// The u16 palette colors for the terrain info windows
#define BODY_HIGHLIGHT_COLOR   (0x73BD)
#define BODY_NORMAL_COLOR      (0x6359)
#define BORDER_HIGHLIGHT_COLOR (0x19DF)
#define BORDER_NORMAL_COLOR    (0x254A)

// Data used for the terrain tile
#define TERRAIN_TILE_TEXT_FONT_ID (0)
#define TERRAIN_TILE_TEXT_POS_X (7)
#define TERRAIN_TILE_TEXT_POS_Y (1)

// data used for the description window
#define DESCRIPTION_WINDOW_ID (7)
#define DESCRIPTION_WINDOW_TEXT_POS_X (7)
#define DESCRIPTION_WINDOW_TEXT_POS_Y (5)
#define DESCRIPTION_WINDOW_WIDTH (90 - DESCRIPTION_WINDOW_TEXT_POS_X)
#define DESCRIPTION_WINDOW_ANIMATION_LENGNTH (12)
#define DESCRIPTION_WINDOW_ANIMATION_OPEN (1)
#define DESCRIPTION_WINDOW_ANIMATION_CLOSE (0)

// Which font to use for the HP section
#define LEVEL_HP_FONT (0)

// How many stats need to get drawn
#define NUM_STATS_TO_DRAW (7)

// The tilemap number for the up/down chevrons
#define BASE_STATS_UP_ARROW    (0x34)
#define EXTRA_STATS_UP_ARROW   (0x5C)

// Scroll arrow
#define SCROLL_ARROW_X      (100)
#define SCROLL_ARROW_TOP     (40)
#define SCROLL_ARROW_BOTTOM (145)
#define SCROLL_ARROW_TAG    (110)


// Graphics Consts
static const u32 sBattleInfoData_Tiles[] = INCBIN_U32("graphics/battle_info/battle_info_data_tiles.4bpp.lz");
static const u32 sBattleInfoData_Pal[] = INCBIN_U32("graphics/battle_info/battle_info_data_palette.gbapal.lz");
static const u32 sBattleInfoData_TileMap[] = INCBIN_U32("graphics/battle_info/battle_info_data_tilemap.bin.lz");
static const u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];

// Declarations
static void VBlankCB_BattleInfoData(void);
static void CB2_BattleInfoDataRun(void);
static void CB2_BattleInfoData_RunSetup(void);
static bool8 BattleInfoData_SetupGfx(void);
static void Task_BattleInfoData_WaitFadeIn(u8 taskId);
static bool8 AllocBattleInfoBgGfx(void);
static bool8 BattleInfoData_InitBGs(void);
static void BattleInfoData_OpenInit(u8 monIndex, MainCallback callback);
static bool8 LoadBattleInfoData_Graphics(void);
static void UpdateTerrainPaletteHighlight(void);
static void InitBattleMonDataWindows(void);
static u16 SetupAndCreateMonIcon(u8 whichMon);
static void Task_BattleInfoData_Main(u8 taskId);
static void BattleInfoData_FadeAndExit(void);
static void InitBattleInfoBox(void);
static void RenderWindow_LevelAndHP(void);
static void RenderWindow_ButtonInfo(void);
static void RenderWindow_StatNumbers(void);
static void DrawStatStageIcons(void);
static void RenderWindow_PutAndCopy(void);
static void SetupTerrainEffects(void);
static void RenderWindow_DoTerrainInfoTiles(void);
static void PlayMonCry(void);
static void BattleInfoData_SetupSprites(void);
static void RenderWindow_TerrainDescription(void);
static void Task_DescriptionWindowAnimation(u8 taskId);
static void BattleInfoData_PlaceTopMenuScrollIndicatorArrows(void);
static void BattleInfoData_RemoveScrollIndicatorArrowPair(void);


// EWRAM Data
static EWRAM_DATA u8* sBattleInfoDataTileGfx = NULL;
static EWRAM_DATA u8* sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8* sMonWindowBgTilemapBuffer = NULL;
static EWRAM_DATA struct BattleInfoBox* sBattleInfoBox = NULL;
static EWRAM_DATA struct BattleInfoDataGUI_Struct
{
    MainCallback savedCallback;
    u16 scrollIndicatorArrowPairId;
    u8 graphicsLoadState;
    u8 windowsBackgroundLoadState;
    u16 cursorIndex;
    u8 cursorViewStart;
    u8 curMon;
    u8 terrainEffects[NUM_TERRAIN_EFFECTS];
    u8 pokemonTypeSpriteIds[2];
    u8 showDescriptionAnimationTaskId;
    u8 scrollUpdateTaskId;
    u8 terrainTilesCount;
    bool8 showDescription;
}  *sBattleInfoDataGUI = NULL;

// String Templates
static const u8 sLevelAndHPTemplate[] = _("{STR_VAR_1}/{STR_VAR_2}\n{STR_VAR_3}");
static const u8 sBlank[] = _("");
static const u8 sNL[] = _("\n");
static const u8 sButtonInfo[] = {
    CHAR_KEYPAD_ICON, CHAR_A_BUTTON, CHAR_SPACE, CHAR_T, CHAR_O, CHAR_G, CHAR_G, CHAR_L, CHAR_E, EOS
};

// The hard coded slide animation values
static const u8 sDescriptionSlideAnimation[2][DESCRIPTION_WINDOW_ANIMATION_LENGNTH] =
{
    // Ease in out
    [DESCRIPTION_WINDOW_ANIMATION_CLOSE] = {
        // 0
        -2,
        -5,
        -10,
        -18,
        -33,
        -60,
        -87,
        -102,
        -110,
        -115,
        -118,
        -120,
    },
    // Ease Out
    [DESCRIPTION_WINDOW_ANIMATION_OPEN] = {
        // -120
        -80,
        -60,
        -36,
        -24,
        -16,
        -11,
        -7,
        -5,
        -3,
        -2,
        -1,
        0
    },
};

// Code
static void VBlankCB_BattleInfoData(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
}

static void CB2_BattleInfoDataRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void CB2_BattleInfoData_RunSetup(void)
{
    // Keep looping the setup function until it returns true (aka, it's done)
    while (MenuHelpers_CallLinkSomething() != TRUE && BattleInfoData_SetupGfx() != TRUE && MenuHelpers_LinkSomething() != TRUE)
    {
    };
}

static bool8 BattleInfoData_SetupGfx(void)
{

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        break;
    case 1:
        ScanlineEffect_Stop();
        break;
    case 2:
        FillPalette(0, 0, 512);
        FreeAllSpritePalettes();
        break;
    case 3:
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        sBattleInfoDataGUI->windowsBackgroundLoadState = 0;
        // gPaletteFade.bufferTransferDisabled = TRUE;
        break;
    case 4:
        if (AllocBattleInfoBgGfx() != TRUE)
            --(gMain.state);
        break;
    case 5:
        if (BattleInfoData_InitBGs())
        {
            sBattleInfoDataGUI->graphicsLoadState = 0;
        }
        else
        {
            BattleInfoData_FadeAndExit();
            return TRUE;
        }
        break;
    case 6:
        if (LoadBattleInfoData_Graphics() != TRUE)
            --(gMain.state);
        break;
    case 7:
        // Handle Windows
        InitBattleMonDataWindows();
        ScheduleBgCopyTilemapToVram(0);
        break;
    case 8:
        LoadBattleInfoPokeballGfx();
        LoadPartyMenuAilmentGfx();
        break;
    case 9:
        LoadHeldItemIcons();
        break;
    case 10:
        LoadMonIconPalettes();
        break;
    case 11:
        InitBattleInfoBox();
        break;
    case 12:
        CreateBattleMonSprites(sBattleInfoBox);
        break;
    case 13:
        RenderMonWindow(sBattleInfoBox, FALSE);
        break;
    case 14:
        RenderWindow_LevelAndHP();
        RenderWindow_ButtonInfo();
        break;
    case 15:
        RenderWindow_StatNumbers();
        break;
    case 16:
        GetActiveTerrainEffects(sBattleInfoDataGUI->curMon, sBattleInfoDataGUI->terrainEffects);
        break;
    case 17:
        SetupTerrainEffects();
        break;
    case 18:
        RenderWindow_DoTerrainInfoTiles();
        break;
    case 19:
        RenderWindow_TerrainDescription();
        break;
    case 20:
        if (sBattleInfoDataGUI->terrainTilesCount > 4)
            BattleInfoData_PlaceTopMenuScrollIndicatorArrows();
        break;
    case 21:
        RenderWindow_PutAndCopy();
        break;
    case 22:
        DrawStatStageIcons();
        break;
    case 23:
        BattleInfoData_SetupSprites();
        break;
    case 24:
        CreateTask(Task_BattleInfoData_WaitFadeIn, 0);
        break;
    case 25:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        break;
    case 26:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gPaletteFade.bufferTransferDisabled = FALSE;
        break;
    case 27:
        PlayMonCry();
        SetVBlankCallback(VBlankCB_BattleInfoData);
        SetMainCallback2(CB2_BattleInfoDataRun);
        return TRUE;
    }
    ++gMain.state;
    return FALSE;
}

static void BattleInfoData_PlaceTopMenuScrollIndicatorArrows(void)
{
    sBattleInfoDataGUI->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        SCROLL_ARROW_X,
        SCROLL_ARROW_TOP,
        SCROLL_ARROW_BOTTOM,
        sBattleInfoDataGUI->terrainTilesCount - 1,
        SCROLL_ARROW_TAG,
        SCROLL_ARROW_TAG,
        &sBattleInfoDataGUI->cursorIndex);
}

static void BattleInfoData_RemoveScrollIndicatorArrowPair(void)
{
    if (sBattleInfoDataGUI->scrollIndicatorArrowPairId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sBattleInfoDataGUI->scrollIndicatorArrowPairId);
        sBattleInfoDataGUI->scrollIndicatorArrowPairId = 0xFF;
    }
}

static void Task_BattleInfoData_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_BattleInfoData_Main;
}

static bool8 AllocBattleInfoBgGfx(void)
{
    u8 battler;
    u32 sizeout;
    switch (sBattleInfoDataGUI->windowsBackgroundLoadState)
    {
    case 0:
    case 1:
        AllocMonWindowBgGfx(sBattleInfoDataGUI->windowsBackgroundLoadState, 0x90);
        break;
    case 2:
        PartyPaletteBufferCopy(sBattleInfoBox->pallet);
        return TRUE;
    }
    ++sBattleInfoDataGUI->windowsBackgroundLoadState;
    return FALSE;
}



// Taken from dexnav
static const u8 sMoveTypeToOamPaletteNum[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL] = TYPE_ICON_PAL_NUM_0,
    [TYPE_FIGHTING] = TYPE_ICON_PAL_NUM_0,
    [TYPE_FLYING] = TYPE_ICON_PAL_NUM_1,
    [TYPE_POISON] = TYPE_ICON_PAL_NUM_1,
    [TYPE_GROUND] = TYPE_ICON_PAL_NUM_0,
    [TYPE_ROCK] = TYPE_ICON_PAL_NUM_0,
    [TYPE_BUG] = TYPE_ICON_PAL_NUM_2,
    [TYPE_GHOST] = TYPE_ICON_PAL_NUM_1,
    [TYPE_STEEL] = TYPE_ICON_PAL_NUM_0,
    [TYPE_MYSTERY] = TYPE_ICON_PAL_NUM_2,
    [TYPE_FIRE] = TYPE_ICON_PAL_NUM_0,
    [TYPE_WATER] = TYPE_ICON_PAL_NUM_1,
    [TYPE_GRASS] = TYPE_ICON_PAL_NUM_2,
    [TYPE_ELECTRIC] = TYPE_ICON_PAL_NUM_0,
    [TYPE_PSYCHIC] = TYPE_ICON_PAL_NUM_1,
    [TYPE_ICE] = TYPE_ICON_PAL_NUM_1,
    [TYPE_DRAGON] = TYPE_ICON_PAL_NUM_2,
    [TYPE_DARK] = TYPE_ICON_PAL_NUM_0,
    #ifdef TYPE_FAIRY
    [TYPE_FAIRY] = TYPE_ICON_PAL_NUM_1, //based on battle_engine
    #endif
};

static bool8 BattleInfoData_InitBGs(void)
{
    u8 i;
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }
    memset(sBg1TilemapBuffer, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates_BattleInfoData, NELEMS(sBgTemplates_BattleInfoData));
    SetBgTilemapBuffer(3, sBg1TilemapBuffer);

    gBattle_WIN0H = WIN_RANGE(120, DISPLAY_WIDTH);
    gBattle_WIN0V = WIN_RANGE(40, DISPLAY_HEIGHT);
    gBattle_BG1_X = -120;
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_ALL);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);

    // Set up the backgrounds
    for (i = 0; i < 4; i++)
    {
        ScheduleBgCopyTilemapToVram(i);
        ShowBg(i);
    }

    // Default all windows to transparent
    for (i = 0; i < NUMBER_OF_WINDOWS; i++)
    {
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
    }

    return TRUE;
}

static void PlayMonCry(void)
{
    if (ShouldPlayNormalMonCry(GetPokemonAtSlot(sBattleInfoDataGUI->curMon)) == TRUE)
        PlayCry3(gBattleMons[sBattleInfoDataGUI->curMon].species, 0, 0);
    else
        PlayCry3(gBattleMons[sBattleInfoDataGUI->curMon].species, 0, 11);
}

#define setupSprite(__WHICH__) \
{ \
    sBattleInfoDataGUI->pokemonTypeSpriteIds[__WHICH__ - 1] = CreateSprite(&sSpriteTemplate_MoveTypes, TYPE_SPRITE_##__WHICH__##_X, TYPE_SPRITES_Y, 2); \
    sprite = &gSprites[sBattleInfoDataGUI->pokemonTypeSpriteIds[__WHICH__ - 1]]; \
    StartSpriteAnim(sprite, mon->type##__WHICH__); \
    sprite->oam.paletteNum = sMoveTypeToOamPaletteNum[mon->type##__WHICH__]; \
    sprite->invisible = FALSE; \
} \

static void BattleInfoData_SetupSprites(void)
{

    // Set up the type sprites
    LoadCompressedSpriteSheet(&sSpriteSheet_MoveTypes);
    LoadCompressedPalette(gMoveTypes_Pal, 0x1D0, 0x60);
    struct Sprite* sprite;
    struct BattlePokemon* mon = &gBattleMons[sBattleInfoDataGUI->curMon];
    setupSprite(1);

    if (mon->type1 != mon->type2)
    {
        setupSprite(2);
    }
}
#undef setupSprite

static void BattleInfoData_OpenInit(u8 monIndex, MainCallback callback)
{
    // Get all vars set up and ready to go, then set the callback to the main loop
    if ((sBattleInfoDataGUI = AllocZeroed(sizeof(struct BattleInfoDataGUI_Struct))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sMonWindowBgTilemapBuffer = NULL;

    sBattleInfoDataGUI->graphicsLoadState = 0;
    sBattleInfoDataGUI->windowsBackgroundLoadState = 0;
    sBattleInfoDataGUI->savedCallback = callback;
    sBattleInfoDataGUI->cursorIndex = 255;
    sBattleInfoDataGUI->curMon = monIndex;
    sBattleInfoDataGUI->showDescription = FALSE;
    sBattleInfoDataGUI->showDescriptionAnimationTaskId = 0xFF;
    sBattleInfoDataGUI->scrollUpdateTaskId = 0xFF;
    SetMainCallback2(CB2_BattleInfoData_RunSetup);
}

static bool8 LoadBattleInfoData_Graphics(void)
{
    u32 sizeout;
    switch (sBattleInfoDataGUI->graphicsLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        sBattleInfoDataTileGfx = malloc_and_decompress(sBattleInfoData_Tiles, &sizeout);
        LoadBgTiles(1, sBattleInfoDataTileGfx, sizeout, 0);
        // DecompressAndCopyTileDataToVram(1, sBattleInfoData_Tiles, 0, 0, 0);
        sBattleInfoDataGUI->graphicsLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE && IsDma3ManagerBusyWithBgCopy() != TRUE)
        {
            LZDecompressWram(sBattleInfoData_TileMap, sBg1TilemapBuffer);
            sBattleInfoDataGUI->graphicsLoadState++;
        }
        break;
    case 2:
        LoadCompressedPalette(sBattleInfoData_Pal, 0, 32);
        // We need 4 copies of the palette data for the four terrain boxes (reusing sizeout)
        for (sizeout = 0; sizeout < 4; sizeout++)
        {
            // Putting into 6, 7, 8, and 9
            CpuCopy16(gPlttBufferUnfaded, gPlttBufferUnfaded + 0x60 + 0x10 * sizeout, 32);
            CpuCopy16(gPlttBufferFaded, gPlttBufferFaded + 0x60 + 0x10 * sizeout, 32);
        }
        UpdateTerrainPaletteHighlight();
        sBattleInfoDataGUI->graphicsLoadState++;
        break;
    default:
        sBattleInfoDataGUI->graphicsLoadState = 0;
        return TRUE;
    }
    return FALSE;
}


static void UpdateTerrainPaletteHighlight(void)
{
    u8 i;
    u16 bodyColor, borderColor;
    for (i = 0; i < 4; i++)
    {
        if (sBattleInfoDataGUI->cursorIndex == sBattleInfoDataGUI->cursorViewStart + i)
        {
            bodyColor = BODY_HIGHLIGHT_COLOR;
            borderColor = BORDER_HIGHLIGHT_COLOR;
        }
        else
        {
            bodyColor = BODY_NORMAL_COLOR;
            borderColor = BORDER_NORMAL_COLOR;
        }
        FillPalette(bodyColor, 0x6C + 0x10 * i, 2);
        FillPalette(borderColor, 0x6D + 0x10 * i, 2);
    }
}


static void InitBattleMonDataWindows(void)
{
    u8 i;

    InitWindows(sBattleInfoDataWindows);
    DeactivateAllTextPrinters();

    LoadUserWindowBorderGfx(0, 0x4F, 0xD0);
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xE0, 0x20);
    LoadPalette(gUnknown_0860F074, 0xF0, 0x20);
}

static void BattleInfoData_FreeResources(void)
{
    if (sBattleInfoDataGUI)
        Free(sBattleInfoDataGUI);
    if (sBg1TilemapBuffer)
        Free(sBg1TilemapBuffer);
    if (sMonWindowBgTilemapBuffer)
        Free(sMonWindowBgTilemapBuffer);
    if (sBattleInfoBox)
        Free(sBattleInfoBox);
    if (sBattleInfoDataTileGfx)
        Free(sBattleInfoDataTileGfx);
    FreeMonWindowData();
    FreeAllWindowBuffers();
}


static void InitBattleInfoBox(void)
{
    sBattleInfoBox = Alloc(sizeof(struct BattleInfoBox));

    sBattleInfoBox->windowId = 0;
    sBattleInfoBox->monSpriteId = SPRITE_NONE;
    sBattleInfoBox->itemSpriteId = SPRITE_NONE;
    sBattleInfoBox->pokeballSpriteId = SPRITE_NONE;
    sBattleInfoBox->statusSpriteId = SPRITE_NONE;
    sBattleInfoBox->pallet = 4;
    sBattleInfoBox->windowWidth = DATA_MON_WINDOW_WIDTH;
    sBattleInfoBox->windowHeight = DATA_MON_WINDOW_HEIGHT;
    sBattleInfoBox->mon = GetPokemonAtSlot(sBattleInfoDataGUI->curMon);
    sBattleInfoBox->battleMon = &gBattleMons[sBattleInfoDataGUI->curMon];
    sBattleInfoBox->spriteCoords = sBattleInfoData_SpriteCoords;
}


#define getN(num) (num < 100 ? (num < 10 ? 1 : 2) : 3)
static void RenderWindow_LevelAndHP(void)
{
    ConvertIntToDecimalStringN(gStringVar1, sBattleInfoBox->battleMon->hp, STR_CONV_MODE_RIGHT_ALIGN, getN(sBattleInfoBox->battleMon->hp));
    ConvertIntToDecimalStringN(gStringVar2, sBattleInfoBox->battleMon->maxHP, STR_CONV_MODE_RIGHT_ALIGN, getN(sBattleInfoBox->battleMon->maxHP));
    ConvertIntToDecimalStringN(gStringVar3, sBattleInfoBox->battleMon->level, STR_CONV_MODE_RIGHT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sLevelAndHPTemplate);
    AddTextPrinterParameterized4(1, LEVEL_HP_FONT, 1, 0, 0, 1, GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER), 0, gStringVar4);
}
#undef getN

static void RenderWindow_ButtonInfo(void)
{
    AddTextPrinterParameterized4(8, 0, 0, 0, -1, -1, GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER), 0, sButtonInfo);
}

static const u8 sStatDrawOrder[] = {
    STAT_ATK,
    STAT_DEF,
    STAT_SPATK,
    STAT_SPDEF,
    STAT_SPEED,
    STAT_ACC,
    STAT_EVASION,
};

static void RenderWindow_StatNumbers(void)
{
    u8* txtPtr = StringCopy(gStringVar4, sBlank);
    for (u8 i = 0; i < NUM_STATS_TO_DRAW; i++)
    {
        if (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] >= 6)
        {
            *txtPtr = CHAR_PLUS;
            if (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] == 7)
                *(++txtPtr) = 0x7D;
            *(++txtPtr) = CHAR_0 + (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] - 6);
        }
        else
        {
            *txtPtr = CHAR_HYPHEN;
            if (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] == 5)
                *(++txtPtr) = 0x7E;
            else
                *(++txtPtr) = 0x7D;
            *(++txtPtr) = CHAR_6 - (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]]);
        }
        *(++txtPtr) = CHAR_NEWLINE;
        ++txtPtr;
    }
    *txtPtr = EOS;
    AddTextPrinterParameterized4(2, 3, 0, 6, 0, 2, GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER), 0, gStringVar4);
}

static const u16 sStatStageTileStarts[] = {
    [STAT_ATK] = 0x01A8,
    [STAT_DEF] = 0x0228,
    [STAT_SPATK] = 0x02A8,
    [STAT_SPDEF] = 0x0328,
    [STAT_SPEED] = 0x03A8,
    [STAT_ACC] = 0x0428,
    [STAT_EVASION] = 0x04A8,
};

static void DrawStatStageIcons(void)
{
    u8* tilemap = sBg1TilemapBuffer;
    u8* cursor;
    u8 i, j, tile, numToDraw;
    for (i = 0; i < NUM_STATS_TO_DRAW; i++)
    {
        tile = (i < NUM_STATS_TO_DRAW - 2) ? BASE_STATS_UP_ARROW : EXTRA_STATS_UP_ARROW;
        cursor = &(tilemap[sStatStageTileStarts[sStatDrawOrder[i]]]);
        if (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] >= 6)
        {
            numToDraw = sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] - 6;
        }
        else if (sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]] < 6)
        {
            tile++;
            numToDraw = 6 - sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]];
        }
        for (j = 0; j < numToDraw; j++)
        {
            *cursor = tile;
            cursor++;
            cursor++;
        }
    }
}

static void RenderWindow_PutAndCopy(void)
{
    u8 i;
    for (i = 1; i < NUMBER_OF_WINDOWS; i++)
    {
        PutWindowTilemap(i);
        CopyWindowToVram(i, 2);
    }
}

static void SetupTerrainEffects(void)
{
    u8 i = 0;
    struct terrain_effect terrainEffect;

    while (i < NUM_TERRAIN_EFFECTS && sBattleInfoDataGUI->terrainEffects[i] < NUM_TERRAIN_EFFECT_DESCRIPTIONS)
    {
        i++;
    }
    sBattleInfoDataGUI->terrainTilesCount = i;
    sBattleInfoDataGUI->cursorViewStart = 0;
    if (sBattleInfoDataGUI->terrainEffects[0] != 255)
    {
        sBattleInfoDataGUI->cursorIndex = 0;
        UpdateTerrainPaletteHighlight();
    }
}

static void RenderWindow_DoTerrainInfoTiles(void)
{
    u8 windowId, i = 0;
    while (i < 4 && sBattleInfoDataGUI->terrainEffects[i + sBattleInfoDataGUI->cursorViewStart] < NUM_TERRAIN_EFFECT_DESCRIPTIONS)
    {
        windowId = i + 3;
        BlitBitmapToMonWindow(sBattleInfoDataTileGfx, windowId, sTerrainEffectFilledTileNums, 10, STAGE_STATE_WINDOW_WIDTH, STAGE_STATE_WINDOW_HEIGHT);
        AddTextPrinterParameterized3(windowId, TERRAIN_TILE_TEXT_FONT_ID, TERRAIN_TILE_TEXT_POS_X, TERRAIN_TILE_TEXT_POS_Y, GetFontColor(BATTLE_MON_TEXT_COLOR_TERRAIN), 0, GetTerrainEffect(sBattleInfoDataGUI->terrainEffects[i + sBattleInfoDataGUI->cursorViewStart]).title);
        ++i;
    }
}

static void RenderWindow_TerrainDescription(void)
{
    const u8* description;
    u8 fontSize;
    u8 tmpTextBuffer[100];
    if (sBattleInfoDataGUI->cursorIndex == 255)
    {
        description = sBlank;
        fontSize = 0;
    }
    else
    {
        struct terrain_effect terrainEffect = GetTerrainEffect(sBattleInfoDataGUI->terrainEffects[sBattleInfoDataGUI->cursorIndex]);
        fontSize = terrainEffect.fontSize;
        switch (sBattleInfoDataGUI->terrainEffects[sBattleInfoDataGUI->cursorIndex])
        {
        case TERRAIN_EFFECT_ENCORE:
            StringCopy(gStringVar1, gMoveNames[gDisableStructs[sBattleInfoDataGUI->curMon].encoredMove]);
            StringExpandPlaceholders(tmpTextBuffer, terrainEffect.description);
            description = SplitTextOnWordsWithNewLines(fontSize, tmpTextBuffer, -1, DESCRIPTION_WINDOW_WIDTH);
            break;
        case TERRAIN_EFFECT_MOVE_DISABLED:
            StringCopy(gStringVar1, gMoveNames[gDisableStructs[sBattleInfoDataGUI->curMon].disabledMove]);
            StringExpandPlaceholders(tmpTextBuffer, terrainEffect.description);
            description = SplitTextOnWordsWithNewLines(fontSize, tmpTextBuffer, -1, DESCRIPTION_WINDOW_WIDTH);
            break;
        default:
            description = terrainEffect.description;
        }
    }
    BlitBitmapToMonWindow(
        sBattleInfoDataTileGfx,
        DESCRIPTION_WINDOW_ID,
        sTerrainEffectFilledTileNums,
        10,
        TERRAIN_WINDOW_WIDTH,
        TERRAIN_WINDOW_HEIGHT
    );
    AddTextPrinterParameterized4(
        DESCRIPTION_WINDOW_ID,
        fontSize,
        DESCRIPTION_WINDOW_TEXT_POS_X,
        DESCRIPTION_WINDOW_TEXT_POS_Y,
        -1,
        -1,
        GetFontColor(BATTLE_MON_TEXT_COLOR_TERRAIN),
        0,
        description
    );
}


#define state data[1]
static void Task_OnScrollUpdate(u8 taskId)
{
    switch (gTasks[taskId].state)
    {
        case 0:
            UpdateTerrainPaletteHighlight();
            break;
        case 1:
            RenderWindow_TerrainDescription();
            break;
        case 2:
            RenderWindow_DoTerrainInfoTiles();
            sBattleInfoDataGUI->scrollUpdateTaskId = 0xFF;
            DestroyTask(taskId);
            break;
    }
    gTasks[taskId].state++;
}

// To make the code a little more readable
#define frame data[1]
#define animation sDescriptionSlideAnimation[sBattleInfoDataGUI->showDescription]
static void Task_DescriptionWindowAnimation(u8 taskId)
{
    if (gTasks[taskId].frame == DESCRIPTION_WINDOW_ANIMATION_LENGNTH)
    {
        DestroyTask(taskId);
        sBattleInfoDataGUI->showDescriptionAnimationTaskId = 0xFF;
    }
    else
    {
        gBattle_BG1_X = animation[gTasks[taskId].frame];
        gTasks[taskId].frame++;
    }
}

#define toggleDescriptionMode() \
if (sBattleInfoDataGUI->showDescriptionAnimationTaskId == 0xFF && sBattleInfoDataGUI->terrainTilesCount > 0) \
{ \
    PlaySE(SE_FU_ZAKU); \
    sBattleInfoDataGUI->showDescription = !sBattleInfoDataGUI->showDescription; \
    sBattleInfoDataGUI->showDescriptionAnimationTaskId = CreateTask(Task_DescriptionWindowAnimation, 0); \
    gTasks[sBattleInfoDataGUI->showDescriptionAnimationTaskId].frame = 0; \
} \

static void Task_BattleInfoData_Main(u8 taskId)
{
    u8 cursorMoveBy;
    if (JOY_REPEAT(A_BUTTON) || JOY_REPEAT(START_BUTTON))
    {
        toggleDescriptionMode();
    }
    else if (JOY_REPEAT(B_BUTTON))
    {
        if (sBattleInfoDataGUI->showDescription)
        {
            toggleDescriptionMode();
        }
        else
        {
            PlaySE(SE_SELECT);
            DestroyTask(taskId);
            BattleInfoData_FadeAndExit();
        }
    }
    else if (sBattleInfoDataGUI->terrainTilesCount > 1 && JOY_REPEAT(DPAD_ANY) && sBattleInfoDataGUI->scrollUpdateTaskId == 0xFF)
    {
        PlaySE(SE_DEX_SCROLL);
        if (JOY_REPEAT(DPAD_DOWN) || JOY_REPEAT(DPAD_RIGHT))
        {
            // Move down the list
            if (sBattleInfoDataGUI->cursorIndex == sBattleInfoDataGUI->terrainTilesCount - 1)
            {
                // Case: When scrolling back to the top
                sBattleInfoDataGUI->cursorIndex = 0;
                sBattleInfoDataGUI->cursorViewStart = 0;
            }
            else
            {
                // Case: When moving down normally
                sBattleInfoDataGUI->cursorIndex += 1;
                if (sBattleInfoDataGUI->cursorViewStart + 2 < sBattleInfoDataGUI->cursorIndex && sBattleInfoDataGUI->cursorViewStart != sBattleInfoDataGUI->terrainTilesCount - 4)
                {
                    // Case: There's still more items bellow the current view
                    sBattleInfoDataGUI->cursorViewStart += 1;
                }
            }
        }
        else
        {
            // Move up the list
            if (sBattleInfoDataGUI->cursorIndex == 0)
            {
                // Case: When scrolling back to the bottom
                sBattleInfoDataGUI->cursorIndex = sBattleInfoDataGUI->terrainTilesCount - 1;
                if (sBattleInfoDataGUI->terrainTilesCount > 4)
                    sBattleInfoDataGUI->cursorViewStart = sBattleInfoDataGUI->terrainTilesCount - 4;
                else
                    sBattleInfoDataGUI->cursorViewStart = 0;
            }
            else
            {
                // Case: When moving up normally
                sBattleInfoDataGUI->cursorIndex -= 1;
                if (sBattleInfoDataGUI->cursorViewStart == sBattleInfoDataGUI->cursorIndex && sBattleInfoDataGUI->cursorViewStart != 0)
                {
                    // Case: There's still more items above the current view
                    sBattleInfoDataGUI->cursorViewStart -= 1;
                }
            }
        }
        sBattleInfoDataGUI->scrollUpdateTaskId = CreateTask(Task_OnScrollUpdate, 0);
        gTasks[sBattleInfoDataGUI->scrollUpdateTaskId].state = 0;
    }
}
#undef toggleDescriptionMode
#undef frame
#undef animation
#undef state

static void CB2_ReturnToBattleInfo(void)
{
    u8 taskId = CreateTask(Task_OpenBattleInfo, 0);
    gTasks[taskId].battleInfoMonIndex = gBattlerInMenuId;
    SetVBlankCallback(VBlankCB_BattleInfoData);
    SetMainCallback2(CB2_BattleInfoDataRun);
}

static void HandleOpenAction(u8 taskId, MainCallback callback)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        BattleInfoData_OpenInit(gTasks[taskId].battleInfoMonIndex, callback);
        DestroyTask(taskId);
    }
}

void Task_OpenBattleInfoDataDebug(u8 taskId)
{
    HandleOpenAction(taskId, CB2_ReturnToFieldWithOpenMenu);
}

void Task_OpenBattleInfoData(u8 taskId)
{
    HandleOpenAction(taskId, CB2_ReturnToBattleInfo);
}


static void Task_BattleInfoData_FadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gBattle_BG1_X = 0;
        SetMainCallback2(sBattleInfoDataGUI->savedCallback);
        BattleInfoData_FreeResources();
        DestroyTask(taskId);
    }
}

static void BattleInfoData_FadeAndExit(void)
{
    // We need to store this somewhere since we're freeing memory. So, why not here?
    gBattlerInMenuId = sBattleInfoDataGUI->curMon;
    BattleInfoData_RemoveScrollIndicatorArrowPair();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_BattleInfoData_FadeAndExit, 0);
    SetVBlankCallback(VBlankCB_BattleInfoData);
    SetMainCallback2(CB2_BattleInfoDataRun);
}

// Undefs
#undef NUMBER_OF_WINDOWS
#undef TYPE_ICON_PAL_NUM_0
#undef TYPE_ICON_PAL_NUM_1
#undef TYPE_ICON_PAL_NUM_2
#undef TYPE_SPRITE_0_X
#undef TYPE_SPRITE_1_X
#undef TYPE_SPRITES_Y
#undef BODY_HIGHLIGHT_COLOR
#undef BODY_NORMAL_COLOR
#undef BORDER_HIGHLIGHT_COLOR
#undef BORDER_NORMAL_COLOR
#undef TERRAIN_TILE_TEXT_FONT_ID
#undef TERRAIN_TILE_TEXT_POS_X
#undef TERRAIN_TILE_TEXT_POS_Y
#undef DESCRIPTION_WINDOW_ID
#undef DESCRIPTION_WINDOW_TEXT_POS_X
#undef DESCRIPTION_WINDOW_TEXT_POS_Y
#undef DESCRIPTION_WINDOW_WIDTH
#undef DESCRIPTION_WINDOW_ANIMATION_LENGNTH
#undef DESCRIPTION_WINDOW_ANIMATION_OPEN
#undef DESCRIPTION_WINDOW_ANIMATION_CLOSE
#undef LEVEL_HP_FONT
#undef NUM_STATS_TO_DRAW
#undef BASE_STATS_UP_ARROW
#undef EXTRA_STATS_UP_ARROW
#undef SCROLL_ARROW_X
#undef SCROLL_ARROW_TOP
#undef SCROLL_ARROW_BOTTOM
#undef SCROLL_ARROW_TAG