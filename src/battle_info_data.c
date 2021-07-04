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
#include "constants/pokemon.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// MACROS

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
static void InitBattleMonDataWindows(void);
static u16 SetupAndCreateMonIcon(u8 whichMon);
static void Task_BattleInfoData_Main(u8 taskId);
static void BattleInfoData_FadeAndExit(void);
static void InitBattleInfoBox(void);
static void RenderWindow_LevelAndHP(void);
static void RenderWindow_StatNumbers(void);
static void DrawStatStageIcons(void);
static void RenderWindow_PutAndCopy(void);



// EWRAM Data
static EWRAM_DATA u8* sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8* sMonWindowBgTilemapBuffer = NULL;
static EWRAM_DATA struct BattleInfoBox* sBattleInfoBox = NULL;
static EWRAM_DATA struct BattleInfoDataGUI_Struct
{
    MainCallback savedCallback;
    u8 state;
    u8 cursorSpriteId;
    u8 graphicsLoadState;
    u8 windowsBackgroundLoadState;
    u8 pokemonIconLoadState;
    u8 cursorIndex;
    u8 curMon;
}  *sBattleInfoDataGUI = NULL;

// String Templates
static const u8 sLevelAndHPTemplate[] = _("{STR_VAR_1}/{STR_VAR_2}\n{STR_VAR_3}");
static const u8 sBlank[] = _("");
static const u8 sNL[] = _("\n");

// Code
static void VBlankCB_BattleInfoData(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
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
    logDebug("BattleInfoData_SetupGfx: %u", gMain.state);

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
        RenderWindow_LevelAndHP();
        RenderWindow_StatNumbers();
        RenderWindow_PutAndCopy();
        DrawStatStageIcons();
        break;
    case 14:
        CreateTask(Task_BattleInfoData_WaitFadeIn, 0);
        break;
    case 15:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        break;
    case 16:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gPaletteFade.bufferTransferDisabled = FALSE;
        break;
    case 17:
        SetVBlankCallback(VBlankCB_BattleInfoData);
        SetMainCallback2(CB2_BattleInfoDataRun);
        return TRUE;
    }
    ++gMain.state;
    return FALSE;
}

static void Task_BattleInfoData_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_BattleInfoData_Main;
}

static bool8 AllocBattleInfoBgGfx(void)
{
    logDebug("AllocBattleInfoBgGfx<Data>");
    u8 battler;
    u32 sizeout;
    switch (sBattleInfoDataGUI->windowsBackgroundLoadState)
    {
    case 0:
    case 1:
        AllocMonWindowBgGfx(sBattleInfoDataGUI->windowsBackgroundLoadState);
        break;
    case 2:
        PartyPaletteBufferCopy(sBattleInfoBox->pallet);
        return TRUE;
    }
    ++sBattleInfoDataGUI->windowsBackgroundLoadState;
    return FALSE;
}

static bool8 BattleInfoData_InitBGs(void)
{
    logDebug("BattleInfoData_InitBGs");
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
    {
        logDebug("    FALSE");
        return FALSE;
    }
    memset(sBg1TilemapBuffer, 0, 0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates_BattleInfoData, NELEMS(sBgTemplates_BattleInfoData));
    SetBgTilemapBuffer(3, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    ScheduleBgCopyTilemapToVram(3);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    FillWindowPixelBuffer(3, PIXEL_FILL(0));
    FillWindowPixelBuffer(4, PIXEL_FILL(0));
    FillWindowPixelBuffer(5, PIXEL_FILL(0));
    FillWindowPixelBuffer(6, PIXEL_FILL(0));
    logDebug("    TRUE");
    return TRUE;
}

static void BattleInfoData_OpenInit(u8 monIndex, MainCallback callback)
{
    logDebug("BattleInfoData_OpenInit");
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
    sBattleInfoDataGUI->cursorIndex = 0;
    sBattleInfoDataGUI->curMon = monIndex;
    SetMainCallback2(CB2_BattleInfoData_RunSetup);
}

static bool8 LoadBattleInfoData_Graphics(void)
{
    logDebug("LoadBattleInfoData_Graphics");
    logDebug("    sBattleInfoDataGUI->graphicsLoadState: %u", sBattleInfoDataGUI->graphicsLoadState);
    switch (sBattleInfoDataGUI->graphicsLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sBattleInfoData_Tiles, 0, 0, 0);
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
        sBattleInfoDataGUI->graphicsLoadState++;
        break;
    default:
        sBattleInfoDataGUI->graphicsLoadState = 0;
        logDebug("    TRUE");
        return TRUE;
    }
    logDebug("    FALSE");
    return FALSE;
}


static void InitBattleMonDataWindows(void)
{
    logDebug("InitBattleMonDataWindows");
    u8 i;

    InitWindows(sBattleInfoDataWindows);
    DeactivateAllTextPrinters();

    LoadUserWindowBorderGfx(0, 0x4F, 0xD0);
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xE0, 0x20);
    LoadPalette(gUnknown_0860F074, 0xF0, 0x20);

    // ClearWindowTilemap(gBattlersCount);
    // AddTextPrinterParameterized3(gBattlersCount, 0, 4, 2, sFontColorTable[BATTLE_MON_TEXT_COLOR_DEFAULT], 0, StringCopyPadded(gStringVar4, gText_BattleInfoHeader, CHAR_SPACE, 2));
    // PutWindowTilemap(gBattlersCount);
    // CopyWindowToVram(gBattlersCount, 2);
}

// static u16 SetupAndCreateMonIcon(u8 whichMon)
// {
//     FreeMonIconPalettes(); //Free space for new pallete
//     LoadMonIconPalette(gBattleMons[whichMon].species); //Loads pallete for current mon
//     return CreateMonIcon(gBattleMons[whichMon].species, SpriteCB_MonIcon, sPokemonIconPositions[whichMon].x, sPokemonIconPositions[whichMon].y, 4, 0); //Create pokemon sprite
// }

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
#define LEVEL_HP_FONT 0
static void RenderWindow_LevelAndHP(void)
{
    logDebug("RenderWindow_LevelAndHP");
    ConvertIntToDecimalStringN(gStringVar1, sBattleInfoBox->battleMon->hp, STR_CONV_MODE_RIGHT_ALIGN, getN(sBattleInfoBox->battleMon->hp));
    ConvertIntToDecimalStringN(gStringVar2, sBattleInfoBox->battleMon->maxHP, STR_CONV_MODE_RIGHT_ALIGN, getN(sBattleInfoBox->battleMon->maxHP));
    ConvertIntToDecimalStringN(gStringVar3, sBattleInfoBox->battleMon->level, STR_CONV_MODE_RIGHT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sLevelAndHPTemplate);
    AddTextPrinterParameterized4(1, LEVEL_HP_FONT, 1, 0, 0, 1, GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER), 0, gStringVar4);
}
#undef LEVEL_HP_FONT
#undef getN

#define appendStatToString(__STAT__) \
ConvertIntToDecimalStringN(gStringVar1, (sBattleInfoBox->battleMon->statStages[__STAT__] - 6), STR_CONV_MODE_RIGHT_ALIGN, 2); \
StringAppend(gStringVar4, gStringVar1); \
StringAppend(gStringVar4, sNL) \

#define NUM_STATS_TO_DRAW 7
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
    logDebug("RenderWindow_StatNumbers");
    u8* txtPtr = StringCopy(gStringVar4, sBlank);
    for (u8 i = 0; i < NUM_STATS_TO_DRAW; i++)
    {
        logInt(sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]]);
        logUnsigned(sBattleInfoBox->battleMon->statStages[sStatDrawOrder[i]]);
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

#define BASE_STATS_UP_ARROW    0x34
#define EXTRA_STATS_UP_ARROW   0x5C

static void DrawStatStageIcons(void)
{
    logDebug("DrawStatStageIcons");
    // CpuCopy16(src, (void *)(sGpuBgConfigs2[bg].tilemap + (destOffset * 2)), mode);
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
            logUnsigned(*cursor);
            *cursor = tile;
            cursor++;
            cursor++;
        }
    }

}
#undef NUM_STATS_TO_DRAW

static void RenderWindow_PutAndCopy(void)
{
    PutWindowTilemap(1);
    CopyWindowToVram(1, 2);

    PutWindowTilemap(2);
    CopyWindowToVram(2, 2);

    PutWindowTilemap(3);
    CopyWindowToVram(3, 2);

    PutWindowTilemap(4);
    CopyWindowToVram(4, 2);

    PutWindowTilemap(5);
    CopyWindowToVram(5, 2);

    PutWindowTilemap(6);
    CopyWindowToVram(6, 2);
}

static void Task_BattleInfoData_Main(u8 taskId)
{
    if (JOY_REPEAT(A_BUTTON) || JOY_REPEAT(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        // DestroyTask(taskId);
        // BattleInfoSelect_GoToDataPlan();

    }
    else if (JOY_REPEAT(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyTask(taskId);
        BattleInfoData_FadeAndExit();
    }
    // else if (IsDoubleBattle())
    // {
    //     if (JOY_REPEAT(DPAD_UP))
    //     {
    //         if (sBattleInfoSelectionGUI->cursorIndex == 0)
    //             sBattleInfoSelectionGUI->cursorIndex = 1;
    //         else if (sBattleInfoSelectionGUI->cursorIndex == 1)
    //             sBattleInfoSelectionGUI->cursorIndex = 3;
    //         else if (sBattleInfoSelectionGUI->cursorIndex == 2)
    //             sBattleInfoSelectionGUI->cursorIndex = 0;
    //         else
    //             sBattleInfoSelectionGUI->cursorIndex = 2;
    //     }
    //     else if (JOY_REPEAT(DPAD_RIGHT))
    //     {
    //         if (sBattleInfoSelectionGUI->cursorIndex == 0)
    //             sBattleInfoSelectionGUI->cursorIndex = 3;
    //         else
    //             --sBattleInfoSelectionGUI->cursorIndex;
    //     }
    //     else if (JOY_REPEAT(DPAD_DOWN))
    //     {
    //         if (sBattleInfoSelectionGUI->cursorIndex == 0)
    //             sBattleInfoSelectionGUI->cursorIndex = 2;
    //         else if (sBattleInfoSelectionGUI->cursorIndex == 1)
    //             sBattleInfoSelectionGUI->cursorIndex = 0;
    //         else if (sBattleInfoSelectionGUI->cursorIndex == 2)
    //             sBattleInfoSelectionGUI->cursorIndex = 3;
    //         else
    //             sBattleInfoSelectionGUI->cursorIndex = 1;
    //     }
    //     else if (JOY_REPEAT(DPAD_LEFT))
    //     {
    //         if (sBattleInfoSelectionGUI->cursorIndex == 3)
    //             sBattleInfoSelectionGUI->cursorIndex = 0;
    //         else
    //             ++sBattleInfoSelectionGUI->cursorIndex;
    //     }
    //     if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_RIGHT) || JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_DOWN))
    //     {
    //         AnimateBattleInfoSlot(0 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[0]);
    //         AnimateBattleInfoSlot(1 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[1]);
    //         AnimateBattleInfoSlot(2 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[2]);
    //         AnimateBattleInfoSlot(3 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[3]);
    //     }
    // }
    // else
    // {
    //     if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_RIGHT) || JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_DOWN))
    //     {
    //         sBattleInfoSelectionGUI->cursorIndex = sBattleInfoSelectionGUI->cursorIndex == 0;
    //         AnimateBattleInfoSlot(0 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[0]);
    //         AnimateBattleInfoSlot(1 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[1]);
    //     }
    // }
}

static void CB2_ReturnToBattleInfo(void)
{
    logDebug("CB2_ReturnToBattleInfo");
    u8 taskId = CreateTask(Task_OpenBattleInfo, 0);
    gTasks[taskId].battleInfoMonIndex = gBattlerInMenuId;
    SetVBlankCallback(VBlankCB_BattleInfoData);
    SetMainCallback2(CB2_BattleInfoDataRun);
}

static void HandleOpenAction(u8 taskId, MainCallback callback)
{
    logDebug("HandleOpenAction<Data>");
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        BattleInfoData_OpenInit(gTasks[taskId].battleInfoMonIndex, callback);
        DestroyTask(taskId);
    }
}

void Task_OpenBattleInfoDataDebug(u8 taskId)
{
    logDebug("Task_OpenBattleInfoDebug");
    HandleOpenAction(taskId, CB2_ReturnToFieldWithOpenMenu);
}

void Task_OpenBattleInfoData(u8 taskId)
{
    logDebug("Task_OpenBattleInfo");
    HandleOpenAction(taskId, CB2_ReturnToBattleInfo);
}


static void Task_BattleInfoData_FadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBattleInfoDataGUI->savedCallback);
        BattleInfoData_FreeResources();
        DestroyTask(taskId);
    }
}

static void BattleInfoData_FadeAndExit(void)
{
    // We need to store this somewhere since we're freeing memory. So, why not here?
    gBattlerInMenuId = sBattleInfoDataGUI->curMon;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_BattleInfoData_FadeAndExit, 0);
    SetVBlankCallback(VBlankCB_BattleInfoData);
    SetMainCallback2(CB2_BattleInfoDataRun);
}


#undef MON_WINDOW_HEIGHT
#undef MON_WINDOW_WIDTH