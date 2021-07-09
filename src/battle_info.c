#include "global.h"
#include "window.h"
#include "graphics.h"
#include "bg.h"
#include "text.h"
#include "battle_info_utils.h"
#include "battle_info_mon_window.h"
#include "battle_info.h"
#include "battle_info_data.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_main.h"
#include "battle_interface.h"
#include "battle_anim.h"
#include "reshow_battle_screen.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "main.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text_window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "party_menu.h"
#include "overworld.h"
#include "dma3.h"
#include "malloc.h"
#include "random.h"
#include "util.h"
#include "m4a.h"
#include "decompress.h"
#include "mail.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// enum
// {
//     HEALTHBOX_ALL,
//     HEALTHBOX_CURRENT_HP,
//     HEALTHBOX_MAX_HP,
//     HEALTHBOX_LEVEL,
//     HEALTHBOX_NICK,
//     HEALTHBOX_HEALTH_BAR,
//     HEALTHBOX_EXP_BAR,
//     HEALTHBOX_UNUSED_7,
//     HEALTHBOX_UNUSED_8,
//     HEALTHBOX_STATUS_ICON,
//     HEALTHBOX_SAFARI_ALL_TEXT,
//     HEALTHBOX_SAFARI_BALLS_TEXT
// };

/* Macros */
// Used to clarify what healthbox we're working with
#define HB_SINGLES_PLAYER         0
#define HB_SINGLES_OPPONENT       1
#define HB_DOUBLES_PLAYER_LEFT    2  // Note; the doubles are just 2+GetBattlerPosition(battler)
#define HB_DOUBLES_PLAYER_RIGHT   4
#define HB_DOUBLES_OPPONENT_LEFT  3
#define HB_DOUBLES_OPPONENT_RIGHT 5

// The bellow is taken from `battle_interface`
// Because the healthbox is too large to fit into one sprite, it is divided into two sprites.
// healthboxLeft  or healthboxMain  is the left part that is used as the 'main' sprite.
// healthboxRight or healthboxOther is the right part of the healthbox.
// There's also the third sprite under name of healthbarSprite that refers to the healthbar visible on the healtbox.

// data fields for healthboxMain
// oam.affineParam holds healthboxRight spriteId
#define hMain_HealthBarSpriteId     data[5]
#define hMain_Battler               data[6]
#define hMain_Data7                 data[7]

// data fields for healthboxRight
#define hOther_HealthBoxSpriteId    data[5]
#define hOther_IndicatorSpriteId    data[6] // For Mega Evo

// data fields for healthbar
#define hBar_HealthBoxSpriteId      data[5]
#define hBar_Data6                  data[6]


// Strings
static const u8 sButtonInfo[] = {
    CHAR_KEYPAD_ICON, CHAR_A_BUTTON, CHAR_SPACE, CHAR_S, CHAR_E, CHAR_L, CHAR_E, CHAR_C, CHAR_T, CHAR_NEWLINE,
    CHAR_KEYPAD_ICON, CHAR_B_BUTTON, CHAR_SPACE, CHAR_B, CHAR_A, CHAR_C, CHAR_K, EOS,
};

static void BattleInfoSelect_OpenInit(u8 cursorMonStart, MainCallback callback);
static void CB2_BattleInfoSelect_RunSetup(void);
static bool8 BattleInfoSelect_SetupGfx(void);
static bool8 BattleInfoSelect_InitBGs(void);
static bool8 LoadBattleInfoSelect_Graphics(void);
static u16 SetupAndCreateMonIcon(u8 whichMon);
static void CreateHealthboxSprite(u8 battler);
static void VBlankCB_BattleInfoSelect(void);
static void CB2_BattleInfoSelectRun(void);
static void Task_BattleInfoSelect_FadeAndExit(u8 taskId);
static void Task_BattleInfoSelect_WaitFadeIn(u8 taskId);
static void Task_BattleInfoSelect_Main(u8 taskId);
static void BattleInfoSelect_GoToDataPlan(void);
static void BattleInfoSelect_FadeAndExit(void);
static void BattleInfoSelect_FreeResources(void);
static void InitBattleMonWindows(void);
static void BlitMainMonWindow(u8 windowId, u8 x, u8 y);
static void BlitEmptyMonWindow(u8 windowId, u8 x, u8 y);
static bool8 AllocBattleInfoBgGfx(void);
static void LoadPartyBoxPalette(u8 windowId, u8 palFlags);
static void RenderMonWindows(void);
static void RenderHeaderWindow(void);
static void RenderButtonWindow(void);
static bool8 CreateBattleMonSpritesLoop(void);
static bool8 RenderBattleMonBoxes(void);
static void InitBattleInfoBoxes();
static void Task_HandleInputs(u8 taskId);

// Data
// graphics
static const u32 sBattleInfoSelect_Tiles[] = INCBIN_U32("graphics/battle_info/battle_info_selection_tiles.4bpp.lz");
static const u32 sBattleInfoSelect_Pal[] = INCBIN_U32("graphics/battle_info/battle_info_selection_palette.gbapal.lz");
static const u32 sBattleInfoSelect_TileMap[] = INCBIN_U32("graphics/battle_info/battle_info_selection_tilemap.bin.lz");

// .rodata
struct BattleInfoSelectionGUI_Struct
{
    MainCallback savedCallback;
    u8 state;
    u8 cursorSpriteId;
    // u8 spriteId[12];
    // u8 windowPointers[10];
    // u8 itemOriginalLocation;
    u8 graphicsLoadState;
    u8 windowsBackgroundLoadState;
    u8 pokemonIconLoadState;
    // u8 curSelection;
    u8 cursorIndex;
};

struct PokemonIconPos
{
    u8 x;
    u8 y;
};

static const struct PokemonIconPos sPokemonIconPositions[] =
{
    [HB_SINGLES_PLAYER] =         {.x =  30, .y =  26},
    [HB_SINGLES_OPPONENT] =       {.x = 123, .y = 117},
    [HB_DOUBLES_PLAYER_LEFT] =    {.x =   5, .y =  53},
    [HB_DOUBLES_PLAYER_RIGHT] =   {.x =  30, .y =  26},
    [HB_DOUBLES_OPPONENT_LEFT] =  {.x = 123, .y = 117},
    [HB_DOUBLES_OPPONENT_RIGHT] = {.x = 149, .y =  91}
};


static EWRAM_DATA struct BattleInfoBox* sBattleInfoBoxes = NULL;
static EWRAM_DATA struct BattleInfoSelectionGUI_Struct* sBattleInfoSelectionGUI = NULL;
static EWRAM_DATA u8* sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8* sMonWindowBgTilemapBuffer = NULL;

static void CB2_BattleInfoSelect_RunSetup(void)
{
    // Keep looping the setup function until it returns true (aka, it's done)
    while (MenuHelpers_CallLinkSomething() != TRUE && BattleInfoSelect_SetupGfx() != TRUE && MenuHelpers_LinkSomething() != TRUE)
    {
    };
}

static void CB2_BattleInfoSelectRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_BattleInfoSelect(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

/**
 * Sets up the battle info selection screen, one case statement at a time.
 * @return (Bool8) - If the setup is finished
 *
 * @note The following describes each case statement
 *  ** Boilerplate steps for changing the screen **
 *       00: Clear out the bg updates / callbacks
 *       01: Stop the scanline effect, if it's running
 *       02: Free all sprite palettes
 *       03: Disable the fade effect
 *       04: Reset all sprite data
 *       05: [blank]
 *       06: Reset all tasks
 *  ** Set up the background graphics
 *       07: Prep setting up the background graphics
 *
 */
static bool8 BattleInfoSelect_SetupGfx(void)
{
    logDebug("BattleInfoSelect_SetupGfx: %u", gMain.state);

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
        sBattleInfoSelectionGUI->windowsBackgroundLoadState = 0;
        // gPaletteFade.bufferTransferDisabled = TRUE;
        break;
    case 4:
        if (AllocBattleInfoBgGfx() != TRUE)
            --(gMain.state);
        break;
    case 5:
        if (BattleInfoSelect_InitBGs())
        {
            sBattleInfoSelectionGUI->graphicsLoadState = 0;
        }
        else
        {
            BattleInfoSelect_FadeAndExit();
            return TRUE;
        }
        break;
    case 6:
        if (LoadBattleInfoSelect_Graphics() != TRUE)
            --(gMain.state);
        break;
    case 7:
        // Handle Windows
        InitBattleMonWindows();
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
        InitBattleInfoBoxes();
        break;
    case 12:
        if (CreateBattleMonSpritesLoop() != TRUE)
        {
            --gMain.state;
        }
        break;
    case 13:
        RenderMonWindows();
        RenderHeaderWindow();
        RenderButtonWindow();
        ScheduleBgCopyTilemapToVram(0);
        break;
    case 14:
        CreateTask(Task_BattleInfoSelect_WaitFadeIn, 0);
        break;
    case 15:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        break;
    case 16:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gPaletteFade.bufferTransferDisabled = FALSE;
        break;
    case 17:
        SetVBlankCallback(VBlankCB_BattleInfoSelect);
        SetMainCallback2(CB2_BattleInfoSelectRun);
        return TRUE;
    }
    ++gMain.state;
    return FALSE;
}

static void Task_BattleInfoSelect_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_BattleInfoSelect_Main;
}

static void Task_BattleInfoSelect_Main(u8 taskId)
{
    if (JOY_REPEAT(A_BUTTON) || JOY_REPEAT(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyTask(taskId);
        BattleInfoSelect_GoToDataPlan();

    }
    else if (JOY_REPEAT(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyTask(taskId);
        BattleInfoSelect_FadeAndExit();
    }
    else if (IsDoubleBattle())
    {
        if (JOY_REPEAT(DPAD_UP))
        {
            if (sBattleInfoSelectionGUI->cursorIndex == 0)
                sBattleInfoSelectionGUI->cursorIndex = 1;
            else if (sBattleInfoSelectionGUI->cursorIndex == 1)
                sBattleInfoSelectionGUI->cursorIndex = 3;
            else if (sBattleInfoSelectionGUI->cursorIndex == 2)
                sBattleInfoSelectionGUI->cursorIndex = 0;
            else
                sBattleInfoSelectionGUI->cursorIndex = 2;
        }
        else if (JOY_REPEAT(DPAD_RIGHT))
        {
            if (sBattleInfoSelectionGUI->cursorIndex == 0)
                sBattleInfoSelectionGUI->cursorIndex = 3;
            else
                --sBattleInfoSelectionGUI->cursorIndex;
        }
        else if (JOY_REPEAT(DPAD_DOWN))
        {
            if (sBattleInfoSelectionGUI->cursorIndex == 0)
                sBattleInfoSelectionGUI->cursorIndex = 2;
            else if (sBattleInfoSelectionGUI->cursorIndex == 1)
                sBattleInfoSelectionGUI->cursorIndex = 0;
            else if (sBattleInfoSelectionGUI->cursorIndex == 2)
                sBattleInfoSelectionGUI->cursorIndex = 3;
            else
                sBattleInfoSelectionGUI->cursorIndex = 1;
        }
        else if (JOY_REPEAT(DPAD_LEFT))
        {
            if (sBattleInfoSelectionGUI->cursorIndex == 3)
                sBattleInfoSelectionGUI->cursorIndex = 0;
            else
                ++sBattleInfoSelectionGUI->cursorIndex;
        }
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_RIGHT) || JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_DOWN))
        {
            AnimateBattleInfoSlot(0 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[0]);
            AnimateBattleInfoSlot(1 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[1]);
            AnimateBattleInfoSlot(2 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[2]);
            AnimateBattleInfoSlot(3 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[3]);
        }
    }
    else
    {
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_RIGHT) || JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_DOWN))
        {
            sBattleInfoSelectionGUI->cursorIndex = sBattleInfoSelectionGUI->cursorIndex == 0;
            AnimateBattleInfoSlot(0 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[0]);
            AnimateBattleInfoSlot(1 == sBattleInfoSelectionGUI->cursorIndex, &sBattleInfoBoxes[1]);
        }
    }
}

static void BattleInfoSelect_FreeResources(void)
{
    if (sBattleInfoSelectionGUI)
        Free(sBattleInfoSelectionGUI);
    if (sBg1TilemapBuffer)
        Free(sBg1TilemapBuffer);
    if (sMonWindowBgTilemapBuffer)
        Free(sMonWindowBgTilemapBuffer);
    if (sBattleInfoBoxes)
        Free(sBattleInfoBoxes);
    FreeMonWindowData();
    FreeAllWindowBuffers();
}

static void InitBattleInfoBoxes()
{
    u8 i;
    sBattleInfoBoxes = Alloc(sizeof(struct BattleInfoBox[gBattlersCount]));

    for (i = 0; i < gBattlersCount; i++)
    {
        sBattleInfoBoxes[i].windowId = i;
        sBattleInfoBoxes[i].monSpriteId = SPRITE_NONE;
        sBattleInfoBoxes[i].itemSpriteId = SPRITE_NONE;
        sBattleInfoBoxes[i].pokeballSpriteId = SPRITE_NONE;
        sBattleInfoBoxes[i].statusSpriteId = SPRITE_NONE;
        sBattleInfoBoxes[i].pallet = i + 4;
        sBattleInfoBoxes[i].windowWidth = SELECTION_MON_WINDOW_WIDTH;
        sBattleInfoBoxes[i].windowHeight = SELECTION_MON_WINDOW_HEIGHT;
        sBattleInfoBoxes[i].mon = GetPokemonAtSlot(i);
        sBattleInfoBoxes[i].battleMon = &gBattleMons[i];

    }
    if (IsDoubleBattle())
    {
        sBattleInfoBoxes[0].spriteCoords = sBattleInfoSelect_SpriteCoords_Double[0];
        sBattleInfoBoxes[1].spriteCoords = sBattleInfoSelect_SpriteCoords_Double[1];
        sBattleInfoBoxes[2].spriteCoords = sBattleInfoSelect_SpriteCoords_Double[2];
        sBattleInfoBoxes[3].spriteCoords = sBattleInfoSelect_SpriteCoords_Double[3];
    }
    else
    {
        sBattleInfoBoxes[0].spriteCoords = sBattleInfoSelect_SpriteCoords_Single[0];
        sBattleInfoBoxes[1].spriteCoords = sBattleInfoSelect_SpriteCoords_Single[1];
    }
}

static void InitBattleMonWindows(void)
{
    logDebug("InitBattleMonWindows");
    u8 i;
    bool8 isDouble = IsDoubleBattle();

    InitWindows(isDouble ? sDoublesBattleWindowTemplate : sSinglesBattleWindowTemplate);
    DeactivateAllTextPrinters();
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    if (isDouble)
    {
        FillWindowPixelBuffer(2, PIXEL_FILL(0));
        FillWindowPixelBuffer(3, PIXEL_FILL(0));
    }

    LoadUserWindowBorderGfx(0, 0x4F, 0xD0);
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xE0, 0x20);
    LoadPalette(gUnknown_0860F074, 0xF0, 0x20);

    // // Set up drawing the header
    // ClearWindowTilemap(gBattlersCount);
    // AddTextPrinterParameterized3(gBattlersCount, 0, 4, 2, sFontColorTable[BATTLE_MON_TEXT_COLOR_DEFAULT], 0, StringCopyPadded(gStringVar4, gText_BattleInfoHeader, CHAR_SPACE, 2));
    // PutWindowTilemap(gBattlersCount);
    // CopyWindowToVram(gBattlersCount, 2);
}

static void Task_BattleInfoSelect_FadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBattleInfoSelectionGUI->savedCallback);
        BattleInfoSelect_FreeResources();
        DestroyTask(taskId);
    }
}

static void BattleInfoSelect_FadeAndExit(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_BattleInfoSelect_FadeAndExit, 0);
    SetVBlankCallback(VBlankCB_BattleInfoSelect);
    SetMainCallback2(CB2_BattleInfoSelectRun);
}

static void BattleInfoSelect_OpenInit(u8 cursorMonStart, MainCallback callback)
{
    logDebug("BattleInfoSelect_OpenInit");
    // Get all vars set up and ready to go, then set the callback to the main loop
    if ((sBattleInfoSelectionGUI = AllocZeroed(sizeof(struct BattleInfoSelectionGUI_Struct))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sMonWindowBgTilemapBuffer = NULL;

    sBattleInfoSelectionGUI->graphicsLoadState = 0;
    sBattleInfoSelectionGUI->windowsBackgroundLoadState = 0;
    sBattleInfoSelectionGUI->savedCallback = callback;
    sBattleInfoSelectionGUI->cursorIndex = cursorMonStart;
    SetMainCallback2(CB2_BattleInfoSelect_RunSetup);
}

static void RenderMonWindows(void)
{
    for (int i = 0; i < gBattlersCount; i++)
    {
        RenderMonWindow(&sBattleInfoBoxes[i], i == sBattleInfoSelectionGUI->cursorIndex);
    }
}

static void RenderHeaderWindow(void)
{
    // Set up drawing the header
    ClearWindowTilemap(gBattlersCount);
    StringCopyPadded(gStringVar4, gText_BattleInfoHeader, CHAR_SPACE, 8);
    AddTextPrinterParameterized3(
        gBattlersCount,                                   // windowId
        7,                                                // fontId
        GetStringCenterAlignXOffset(7, gStringVar4, 112), // left
        4,                                                // top
        GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER),       // color (as an array of [bg, fg, shadow])
        0,                                                // speed
        gStringVar4);                                     // str
    PutWindowTilemap(gBattlersCount);
    CopyWindowToVram(gBattlersCount, 2);
}

static void RenderButtonWindow(void)
{
    // Set up drawing the header
    ClearWindowTilemap(gBattlersCount+1);
    AddTextPrinterParameterized3(
        gBattlersCount + 1,                               // windowId
        0,                                                // fontId
        0,                                                // left
        0,                                                // top
        GetFontColor(BATTLE_MON_TEXT_COLOR_HEADER),       // color (as an array of [bg, fg, shadow])
        0,                                                // speed
        sButtonInfo);                                     // str
    PutWindowTilemap(gBattlersCount + 1);
    CopyWindowToVram(gBattlersCount + 1, 2);
}

static bool8 CreateBattleMonSpritesLoop(void)
{
    CreateBattleMonSprites(&sBattleInfoBoxes[sBattleInfoSelectionGUI->pokemonIconLoadState]);
    if (++sBattleInfoSelectionGUI->pokemonIconLoadState == gBattlersCount)
        return TRUE;
    else
        return FALSE;
}

// Start Menu Function
void HandleOpenAction(u8 taskId, MainCallback callback)
{
    logDebug("HandleOpenAction<Selection>");
    u8 passedMonIndex;
    if (!gPaletteFade.active)
    {
        if (gTasks[taskId].battleInfoMonIndex)
        {
            passedMonIndex = gTasks[taskId].battleInfoMonIndex;
        }
        else
        {
            passedMonIndex = 0;
        }
        CleanupOverworldWindowsAndTilemaps();
        BattleInfoSelect_OpenInit(passedMonIndex, callback);
        DestroyTask(taskId);
    }
}

void Task_OpenBattleInfoDebug(u8 taskId)
{
    logDebug("Task_OpenBattleInfoDebug");
    HandleOpenAction(taskId, CB2_ReturnToFieldWithOpenMenu);
}

void Task_OpenBattleInfo(u8 taskId)
{
    logDebug("Task_OpenBattleInfo");
    HandleOpenAction(taskId, CB2_SetUpReshowBattleScreenAfterMenu);
}

void CB2_OpenBattleInfo(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static bool8 BattleInfoSelect_InitBGs(void)
{
    logDebug("BattleInfoSelect_InitBGs");
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
    InitBgsFromTemplates(0, sBgTemplates_BattleInfo, NELEMS(sBgTemplates_BattleInfo));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);
    logDebug("    TRUE");
    return TRUE;
}

static bool8 LoadBattleInfoSelect_Graphics(void)
{
    logDebug("LoadBattleInfoSelect_Graphics");
    logDebug("    sBattleInfoSelectionGUI->graphicsLoadState: %u", sBattleInfoSelectionGUI->graphicsLoadState);
    switch (sBattleInfoSelectionGUI->graphicsLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sBattleInfoSelect_Tiles, 0, 0, 0);
        sBattleInfoSelectionGUI->graphicsLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE && IsDma3ManagerBusyWithBgCopy() != TRUE)
        {
            LZDecompressWram(sBattleInfoSelect_TileMap, sBg1TilemapBuffer);
            sBattleInfoSelectionGUI->graphicsLoadState++;
        }
        break;
    case 2:
        LoadCompressedPalette(sBattleInfoSelect_Pal, 0, 32);
        sBattleInfoSelectionGUI->graphicsLoadState++;
        break;
    default:
        sBattleInfoSelectionGUI->graphicsLoadState = 0;
        logDebug("    TRUE");
        return TRUE;
    }
    logDebug("    FALSE");
    return FALSE;
}

static bool8 AllocBattleInfoBgGfx(void)
{
    u8 battler;
    u32 sizeout;
    logInfo("AllocBattleInfoBgGfx");
    logInfo("    sBattleInfoSelectionGUI->windowsBackgroundLoadState: %u", sBattleInfoSelectionGUI->windowsBackgroundLoadState);
    switch (sBattleInfoSelectionGUI->windowsBackgroundLoadState)
    {
        case 0:
        case 1:
            AllocMonWindowBgGfx(sBattleInfoSelectionGUI->windowsBackgroundLoadState, 0x160);
            break;
        default:
            PartyPaletteBufferCopy(sBattleInfoBoxes[sBattleInfoSelectionGUI->windowsBackgroundLoadState - 2].pallet);
            if (sBattleInfoSelectionGUI->windowsBackgroundLoadState == gBattlersCount + 1)
            {
                return TRUE;
            }
    }
    ++sBattleInfoSelectionGUI->windowsBackgroundLoadState;
    return FALSE;
}

static void BattleInfoSelect_GoToDataPlan(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    u8 taskId = CreateTask(Task_OpenBattleInfoData, 0);
    gTasks[taskId].battleInfoMonIndex = sBattleInfoSelectionGUI->cursorIndex;
    BattleInfoSelect_FreeResources();
    FreeAllWindowBuffers();
}

#undef HB_SINGLES_PLAYER
#undef HB_SINGLES_OPPONENT
#undef HB_DOUBLES_PLAYER_LEFT
#undef HB_DOUBLES_PLAYER_RIGHT
#undef HB_DOUBLES_OPPONENT_LEFT
#undef HB_DOUBLES_OPPONENT_RIGHT

#undef hMain_HealthBarSpriteId
#undef hMain_Battler
#undef hMain_Data7
#undef hOther_HealthBoxSpriteId
#undef hOther_IndicatorSpriteId
#undef hBar_HealthBoxSpriteId
#undef hBar_Data6
