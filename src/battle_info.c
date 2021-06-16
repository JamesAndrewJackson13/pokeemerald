#include "global.h"
#include "window.h"
#include "graphics.h"
#include "bg.h"
#include "text.h"
#include "battle_info.h"
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
#include "malloc.h"
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


struct BattleInfoBox
{
    const u8* spriteCoords;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
};

/* Macros */
// For the (x, y) of different elements in the bar
#define NICKNAME_POS_X 22
#define NICKNAME_POS_Y  1
#define GENDER_POS_X   64
#define GENDER_POS_Y   10
#define LEVEL_POS_X    35
#define LEVEL_POS_Y    10

// For the different text colors that can be used
#define TEXT_COLOR_DEFAULT           0
#define TEXT_COLOR_UNUSED_1          1
#define TEXT_COLOR_GENDER_SYMBOL     2
#define TEXT_COLOR_SELECTION_ACTIONS 3
#define TEXT_COLOR_FIELD_MOVES       4
#define TEXT_COLOR_UNUSED_2          5
#define TEXT_COLOR_HEADER            6

// For the different status ailment icons that can be shown
#define AILMENT_NONE  0
#define AILMENT_PSN   1
#define AILMENT_PRZ   2
#define AILMENT_SLP   3
#define AILMENT_FRZ   4
#define AILMENT_BRN   5
#define AILMENT_PKRS  6
#define AILMENT_FNT   7

// For all the different palette mode flags
#define PARTY_PAL_SELECTED     (1 << 0)
#define PARTY_PAL_FAINTED      (1 << 1)
#define PARTY_PAL_TO_SWITCH    (1 << 2)
#define PARTY_PAL_MULTI_ALT    (1 << 3)
#define PARTY_PAL_SWITCHING    (1 << 4)
#define PARTY_PAL_TO_SOFTBOIL  (1 << 5)
#define PARTY_PAL_NO_MON       (1 << 6)
#define PARTY_PAL_UNUSED       (1 << 7)

// Used to clarify what healthbox we're working with
#define HB_SINGLES_PLAYER         0
#define HB_SINGLES_OPPONENT       1
#define HB_DOUBLES_PLAYER_LEFT    2  // Note; the doubles are just 2+GetBattlerPosition(battler)
#define HB_DOUBLES_PLAYER_RIGHT   4
#define HB_DOUBLES_OPPONENT_LEFT  3
#define HB_DOUBLES_OPPONENT_RIGHT 5

#define MON_WINDOW_HEIGHT          3
#define MON_WINDOW_WIDTH          10

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

static void BattleInfoSelect_OpenInit(MainCallback callback);
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
static void BattleInfoSelect_FadeAndExit(void);
static void BattleInfoSelect_FreeResources(void);
static void InitBattleMonWindows(void);
static void LoadBattleInfoPokeballGfx(void);
static void LoadPartyMenuAilmentGfx(void);
static void BlitMainMonWindow(u8 windowId, u8 x, u8 y);
static void BlitEmptyMonWindow(u8 windowId, u8 x, u8 y);
static bool8 AllocBattleInfoBgGfx(void);
static void LoadPartyBoxPalette(u8 windowId, u8 palFlags);
static void RenderMonWindows(void);
static void CreateBattleMonSprites(u8 slot);
static bool8 CreateBattleMonSpritesLoop(void);
static bool8 RenderBattleMonBoxes(void);
static void InitBattleInfoBoxes();
static void DisplayPartyPokemonBarDetail(u8 windowId, const u8* str, u8 color, u8 x, u8 y);
static void DisplayPartyPokemonGenderNidoranCheck(struct Pokemon* mon, struct BattleInfoBox* battleInfoBox, u8 c);
static void DisplayPartyPokemonGender(u8 gender, u16 species, u8* nickname, struct BattleInfoBox* battleInfoBox);
static void DisplayPartyPokemonNickname(struct Pokemon* mon, struct BattleInfoBox* battleInfoBox, u8 c);
static void DisplayPartyPokemonLevel(u8 level, struct BattleInfoBox* battleInfoBox);
static struct Pokemon* GetPokemonAtSlot(u8 slot);
static void AnimateBattleInfoSlot(u8 slot, u8 animNum, struct Pokemon* curPokemon, struct BattleInfoBox* curBattleInfoBox);
static void Task_HandleInputs(u8 taskId);

// Data
// graphics
static const u32 sBattleInfoSelect_Tiles[] = INCBIN_U32("graphics/battle_info/battle_info_selection_tiles.4bpp.lz");
static const u32 sBattleInfoSelect_Pal[] = INCBIN_U32("graphics/battle_info/battle_info_selection_palette.gbapal.lz");
static const u32 sBattleInfoSelect_TileMap[] = INCBIN_U32("graphics/battle_info/battle_info_selection_tilemap.bin.lz");
// static const u32 gHeldItemGfx[] = INCBIN_U32("graphics/interface/hold_icons.4bpp");
// static const u16 gHeldItemPalette[] = INCBIN_U16("graphics/interface/hold_icons.gbapal");
// static const u32 sHeldItemGfx[] = INCBIN_U32("graphics/interface/hold_icons.4bpp");
static const u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];

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
static EWRAM_DATA u8* sMonWindowBgGfxTilemap = NULL;
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

static const struct SpritePalette sSpritePalette_HeldItem =
{
    gHeldItemPalette, 0xd750
};

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
            AnimateBattleInfoSlot(0, 0 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
            AnimateBattleInfoSlot(1, 1 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
            AnimateBattleInfoSlot(2, 2 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
            AnimateBattleInfoSlot(3, 3 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
        }
    }
    else
    {
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_RIGHT) || JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_DOWN))
        {
            sBattleInfoSelectionGUI->cursorIndex = sBattleInfoSelectionGUI->cursorIndex == 0;
            AnimateBattleInfoSlot(0, 0 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
            AnimateBattleInfoSlot(1, 1 == sBattleInfoSelectionGUI->cursorIndex, NULL, NULL);
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
    if (sMonWindowBgGfxTilemap)
        Free(sMonWindowBgGfxTilemap);
    if (sBattleInfoBoxes)
        Free(sBattleInfoBoxes);
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

static void BattleInfoSelect_OpenInit(MainCallback callback)
{
    logDebug("BattleInfoSelect_OpenInit");
    // Get all vars set up and ready to go, then set the callback to the main loop
    if ((sBattleInfoSelectionGUI = AllocZeroed(sizeof(struct BattleInfoSelectionGUI_Struct))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sMonWindowBgTilemapBuffer = NULL;
    sMonWindowBgGfxTilemap = NULL;

    sBattleInfoSelectionGUI->graphicsLoadState = 0;
    sBattleInfoSelectionGUI->windowsBackgroundLoadState = 0;
    sBattleInfoSelectionGUI->savedCallback = callback;
    sBattleInfoSelectionGUI->cursorIndex = 0;
    SetMainCallback2(CB2_BattleInfoSelect_RunSetup);
}


// Start Menu Function
void Task_OpenBattleInfoDebug(u8 taskId)
{
    logDebug("Task_OpenBattleInfoDebug");
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        BattleInfoSelect_OpenInit(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

void Task_OpenBattleInfo(u8 taskId)
{
    logDebug("Task_OpenBattleInfo");
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        BattleInfoSelect_OpenInit(CB2_SetUpReshowBattleScreenAfterMenu);
        DestroyTask(taskId);
    }
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

static u16 SetupAndCreateMonIcon(u8 whichMon)
{
    FreeMonIconPalettes(); //Free space for new pallete
    LoadMonIconPalette(gBattleMons[whichMon].species); //Loads pallete for current mon
    return CreateMonIcon(gBattleMons[whichMon].species, SpriteCB_MonIcon, sPokemonIconPositions[whichMon].x, sPokemonIconPositions[whichMon].y, 4, 0); //Create pokemon sprite
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
    // AddTextPrinterParameterized3(gBattlersCount, 0, 4, 2, sFontColorTable[TEXT_COLOR_DEFAULT], 0, StringCopyPadded(gStringVar4, gText_BattleInfoHeader, CHAR_SPACE, 2));
    // PutWindowTilemap(gBattlersCount);
    // CopyWindowToVram(gBattlersCount, 2);
}

static void LoadBattleInfoPokeballGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeball);
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeballSmall);
    LoadCompressedSpritePalette(&sSpritePalette_MenuPokeball);
}

static void LoadPartyMenuAilmentGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_StatusIcons);
    LoadCompressedSpritePalette(&sSpritePalette_StatusIcons);
}

static void CreateBattleMonIconSpriteParameterized(u16 species, u32 pid, struct BattleInfoBox* battleInfoBox, u8 priority)
{
    logDebug("CreateBattleMonIconSpriteParameterized");
    logDebug("    species: %u", species);
    logDebug("    pid: %u", pid);
    logDebug("    priority: %u", priority);
    logDebug("    (x<%u>, y<%u>)", battleInfoBox->spriteCoords[0], battleInfoBox->spriteCoords[1]);
    battleInfoBox->monSpriteId = CreateMonIcon(species, SpriteCB_MonIcon, battleInfoBox->spriteCoords[0], battleInfoBox->spriteCoords[1], 4, pid);
    gSprites[battleInfoBox->monSpriteId].oam.priority = priority;
}

static void ShowOrHideHeldItemSprite(u16 item, struct BattleInfoBox* battleInfoBox)
{
    logDebug("    ShowOrHideHeldItemSprite");
    logDebug("        item: %u", item);
    if (item == ITEM_NONE)
    {
        logDebug("        item == ITEM_NONE");
        gSprites[battleInfoBox->itemSpriteId].invisible = TRUE;
    }
    else
    {
        logDebug("        item == ITEM_%s", ItemIsMail(item) ? "MAIL" : "OTHER");
        if (ItemIsMail(item))
            StartSpriteAnim(&gSprites[battleInfoBox->itemSpriteId], 1);
        else
            StartSpriteAnim(&gSprites[battleInfoBox->itemSpriteId], 0);
        gSprites[battleInfoBox->itemSpriteId].invisible = FALSE;
    }
}

static void CreateBattleMonHeldItemSpriteParameterized(u16 species, u16 item, struct BattleInfoBox* battleInfoBox)
{
    logDebug("CreateBattleMonHeldItemSpriteParameterized");
    logDebug("    species: %u", species);
    logDebug("    item: %u", item);
    logDebug("    (x<%u>, y<%u>)", battleInfoBox->spriteCoords[2], battleInfoBox->spriteCoords[3]);
    battleInfoBox->itemSpriteId = CreateSprite(&sSpriteTemplate_HeldItem, battleInfoBox->spriteCoords[2], battleInfoBox->spriteCoords[3], 0);
    gSprites[battleInfoBox->itemSpriteId].oam.priority = 0;
    ShowOrHideHeldItemSprite(item, battleInfoBox);
}

static void CreateBattleMonPokeballSpriteParameterized(u16 species, struct BattleInfoBox* battleInfoBox)
{
    logDebug("CreateBattleMonPokeballSpriteParameterized");
    logDebug("    species: %u", species);
    logDebug("    (x<%u>, y<%u>)", battleInfoBox->spriteCoords[6], battleInfoBox->spriteCoords[7]);
    battleInfoBox->pokeballSpriteId = CreateSprite(&sSpriteTemplate_MenuPokeball, battleInfoBox->spriteCoords[6], battleInfoBox->spriteCoords[7], 8);
    gSprites[battleInfoBox->pokeballSpriteId].oam.priority = 0;
}

// static u8 GetAilmentFromStatus(u32 status)
// {
//     if (status & STATUS1_PSN_ANY)
//         return AILMENT_PSN;
//     if (status & STATUS1_PARALYSIS)
//         return AILMENT_PRZ;
//     if (status & STATUS1_SLEEP)
//         return AILMENT_SLP;
//     if (status & STATUS1_FREEZE)
//         return AILMENT_FRZ;
//     if (status & STATUS1_BURN)
//         return AILMENT_BRN;
//     return AILMENT_NONE;
// }


static void UpdateBattleMonAilmentGfx(u8 status, struct BattleInfoBox* battleInfoBox)
{
    switch (status)
    {
    case AILMENT_NONE:
    case AILMENT_PKRS:
        gSprites[battleInfoBox->statusSpriteId].invisible = TRUE;
        break;
    default:
        StartSpriteAnim(&gSprites[battleInfoBox->statusSpriteId], status - 1);
        gSprites[battleInfoBox->statusSpriteId].invisible = FALSE;
        break;
    }
}

static void SetBattleMonAilmentGfx(struct Pokemon* mon, struct BattleInfoBox* battleInfoBox)
{
    UpdateBattleMonAilmentGfx(GetMonAilment(mon), battleInfoBox);
}

static void CreateBattleMonStatusSpriteParameterized(u16 species, u8 status, struct BattleInfoBox* battleInfoBox)
{
    logDebug("CreateBattleMonStatusSpriteParameterized");
    logDebug("    species: %u", species);
    logDebug("    status: %u", status);
    logDebug("    (x<%u>, y<%u>)", battleInfoBox->spriteCoords[4], battleInfoBox->spriteCoords[5]);
    battleInfoBox->statusSpriteId = CreateSprite(&sSpriteTemplate_StatusIcons, battleInfoBox->spriteCoords[4], battleInfoBox->spriteCoords[5], 0);
    UpdateBattleMonAilmentGfx(status, battleInfoBox);
    gSprites[battleInfoBox->statusSpriteId].oam.priority = 0;
}

static void CreateBattleMonSprites(u8 slot)
{
    logDebug("CreateBattleMonSprites");
    logDebug("    slot: %u", slot);

    struct Pokemon curPokemon = *GetPokemonAtSlot(slot);
    struct BattlePokemon curBattlePokemon = gBattleMons[slot];
    u8 status;
    logUnsigned(GetMonData(curPokemon, MON_DATA_STATUS, NULL));

    if (curBattlePokemon.species != SPECIES_NONE)
    {
        CreateBattleMonIconSpriteParameterized(curBattlePokemon.species, curBattlePokemon.personality, &sBattleInfoBoxes[slot], 0);
        CreateBattleMonHeldItemSpriteParameterized(curBattlePokemon.species, curBattlePokemon.item, &sBattleInfoBoxes[slot]);
        CreateBattleMonPokeballSpriteParameterized(curBattlePokemon.species, &sBattleInfoBoxes[slot]);
        if (curBattlePokemon.hp == 0)
            status = AILMENT_FNT;
        else
            status = GetAilmentFromStatus(curBattlePokemon.status1);

        logDebug("    status: %u", status);
        logUnsigned(curBattlePokemon.status1);
        logUnsigned(curBattlePokemon.status2);
        CreateBattleMonStatusSpriteParameterized(curBattlePokemon.species, status, &sBattleInfoBoxes[slot]);
    }
}


static bool8 CreateBattleMonSpritesLoop(void)
{
    CreateBattleMonSprites(sBattleInfoSelectionGUI->pokemonIconLoadState);
    if (++sBattleInfoSelectionGUI->pokemonIconLoadState == gBattlersCount)
        return TRUE;
    else
        return FALSE;
}

static void PartyPaletteBufferCopy(u8 offset)
{
    offset *= 16;
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferUnfaded[offset], 32);
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferFaded[offset], 32);
}

static bool8 AllocBattleInfoBgGfx(void)
{
    u32 sizeout;
    logInfo("AllocBattleInfoBgGfx");
    logInfo("    sBattleInfoSelectionGUI->windowsBackgroundLoadState: %u", sBattleInfoSelectionGUI->windowsBackgroundLoadState);
    switch (sBattleInfoSelectionGUI->windowsBackgroundLoadState)
    {
    case 0:
        sMonWindowBgGfxTilemap = malloc_and_decompress(gPartyMenuBg_Gfx, &sizeout);
        LoadBgTiles(2, sMonWindowBgGfxTilemap, sizeout, 0);
        break;
    case 1:
        // LoadCompressedPalette(gPartyMenuBg_Pal, 0x10, 0x160);
        // CpuCopy16(gPlttBufferUnfaded + 0x10, palBuffer, 0x160);
        LoadCompressedPalette(gPartyMenuBg_Pal, 0, 0x160);
        CpuCopy16(&gPlttBufferUnfaded, palBuffer, 0x160);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        PartyPaletteBufferCopy(sBattleInfoSelectionGUI->windowsBackgroundLoadState + 2);
        break;
    default:
        return TRUE;
    }
    sBattleInfoSelectionGUI->windowsBackgroundLoadState++;
    return FALSE;
}

static u8* GetBgTile(u16 tileId)
{
    return &sMonWindowBgGfxTilemap[tileId << 5];
}

// what the hell is 'c'?
static void BlitBitmapToMonWindow(u8 windowId, const u8* b, u8 c, u8 x, u8 y)
{
    u8* pixels = AllocZeroed(MON_WINDOW_HEIGHT * MON_WINDOW_WIDTH * 32);
    u8 i, j;

    if (pixels != NULL)
    {
        for (i = 0; i < MON_WINDOW_HEIGHT; i++)
        {
            for (j = 0; j < MON_WINDOW_WIDTH; j++)
                CpuCopy16(GetBgTile(b[x + j + ((y + i) * c)]), &pixels[(i * MON_WINDOW_WIDTH + j) * 32], 32);
        }
        BlitBitmapToWindow(windowId, pixels, x * 8, y * 8, MON_WINDOW_WIDTH * 8, MON_WINDOW_HEIGHT * 8);
        Free(pixels);
    }
}

static void BlitMainMonWindow(u8 windowId, u8 x, u8 y)
{
    BlitBitmapToMonWindow(windowId, sMainSlotTileNums, 10, x, y);
}

static void BlitEmptyMonWindow(u8 windowId, u8 x, u8 y)
{
    BlitBitmapToMonWindow(windowId, sEmptySlotTileNums, 10, x, y);
}

#define LOAD_PARTY_BOX_PAL(paletteIds, paletteOffsets)                                    \
{                                                                                         \
    LoadPalette(&palBuffer[paletteIds[0]], paletteOffsets[0] + palNum, 2);  \
    LoadPalette(&palBuffer[paletteIds[1]], paletteOffsets[1] + palNum, 2);  \
    LoadPalette(&palBuffer[paletteIds[2]], paletteOffsets[2] + palNum, 2);  \
}

static void LoadPartyBoxPalette(u8 windowId, u8 palFlags)
{
    u8 palNum = GetWindowAttribute(windowId, WINDOW_PALETTE_NUM) * 16;

    if (palFlags & PARTY_PAL_NO_MON)
    {
        LOAD_PARTY_BOX_PAL(sPartyBoxNoMonPalIds, sPartyBoxNoMonPalOffsets);
    }
    else if (palFlags & PARTY_PAL_TO_SOFTBOIL)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (palFlags & PARTY_PAL_SWITCHING)
    {
        LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
        LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
    }
    else if (palFlags & PARTY_PAL_TO_SWITCH)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (palFlags & PARTY_PAL_FAINTED)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionFaintedPalIds, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxFaintedPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxFaintedPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (palFlags & PARTY_PAL_MULTI_ALT)
    {
        if (palFlags & PARTY_PAL_SELECTED)
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionMultiPalIds, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            LOAD_PARTY_BOX_PAL(sPartyBoxMultiPalIds1, sPartyBoxPalOffsets1);
            LOAD_PARTY_BOX_PAL(sPartyBoxMultiPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (palFlags & PARTY_PAL_SELECTED)
    {
        LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds1, sPartyBoxPalOffsets1);
        LOAD_PARTY_BOX_PAL(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
    }
    else
    {
        LOAD_PARTY_BOX_PAL(sPartyBoxEmptySlotPalIds1, sPartyBoxPalOffsets1);
        LOAD_PARTY_BOX_PAL(sPartyBoxEmptySlotPalIds2, sPartyBoxPalOffsets2);
    }
}

static void RenderMonWindows(void)
{
    u8 x;
    struct Pokemon* curPokemonPtr;
    struct BattleInfoBox* curBattleBoxPtr;

    for (int i = 0; i < gBattlersCount; i++)
    {
        curPokemonPtr = GetPokemonAtSlot(i);
        curBattleBoxPtr = &sBattleInfoBoxes[i];
        LoadPartyBoxPalette(i, (i == sBattleInfoSelectionGUI->cursorIndex) ? PARTY_PAL_SELECTED : PARTY_PAL_UNUSED);
        BlitMainMonWindow(i, 0, 0);
        DisplayPartyPokemonNickname(curPokemonPtr, curBattleBoxPtr, 0);
        DisplayPartyPokemonGenderNidoranCheck(curPokemonPtr, curBattleBoxPtr, 0);
        if (gBattleMons[i].status1 == AILMENT_NONE || CheckPartyPokerus(curPokemonPtr, 0))
            DisplayPartyPokemonLevel(gBattleMons[i].level, curBattleBoxPtr);
        AnimateBattleInfoSlot(i, i == sBattleInfoSelectionGUI->cursorIndex, curPokemonPtr, curBattleBoxPtr);

        PutWindowTilemap(i);
        CopyWindowToVram(i, 2);
    }

    // Set up drawing the header
    ClearWindowTilemap(gBattlersCount);
    StringCopyPadded(gStringVar4, gText_BattleInfoHeader, CHAR_SPACE, 8);
    x = GetStringCenterAlignXOffset(7, gStringVar4, 112);
    AddTextPrinterParameterized3(gBattlersCount, 7, x, 4, sFontColorTable[TEXT_COLOR_HEADER], 0, gStringVar4);
    PutWindowTilemap(gBattlersCount);
    CopyWindowToVram(gBattlersCount, 2);
    ScheduleBgCopyTilemapToVram(0);
}

static void DisplayPartyPokemonBarDetail(u8 windowId, const u8* str, u8 color, u8 x, u8 y)
{
    AddTextPrinterParameterized3(windowId, 0, x, y, sFontColorTable[color], 0, str);
}

static void DisplayPartyPokemonGenderNidoranCheck(struct Pokemon* mon, struct BattleInfoBox* battleInfoBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    // if (c == 1)
        // battleInfoBox->infoRects->blitFunc(battleInfoBox->windowId, battleInfoBox->infoRects->dimensions[8] >> 3, (battleInfoBox->infoRects->dimensions[9] >> 3) + 1, battleInfoBox->infoRects->dimensions[10] >> 3, battleInfoBox->infoRects->dimensions[11] >> 3, FALSE);
    GetMonNickname(mon, nickname);
    DisplayPartyPokemonGender(GetMonGender(mon), GetMonData(mon, MON_DATA_SPECIES), nickname, battleInfoBox);
}

static void DisplayPartyPokemonGender(u8 gender, u16 species, u8* nickname, struct BattleInfoBox* battleInfoBox)
{
    u8 palNum = GetWindowAttribute(battleInfoBox->windowId, WINDOW_PALETTE_NUM) * 16;
    const u8* genderPalToUse;
    const u8* symbolToUse;

    if (species == SPECIES_NONE)
        return;
    if ((species == SPECIES_NIDORAN_M || species == SPECIES_NIDORAN_F) && StringCompare(nickname, gSpeciesNames[species]) == 0)
        return;
    switch (gender)
    {
    case MON_MALE:
        genderPalToUse = sGenderMalePalIds;
        symbolToUse = gText_MaleSymbol;
        break;
    case MON_FEMALE:
        genderPalToUse = sGenderFemalePalIds;
        symbolToUse = gText_FemaleSymbol;
        break;
    }
    LoadPalette(&palBuffer[genderPalToUse[0]], sGenderPalOffsets[0] + palNum, 2);
    LoadPalette(&palBuffer[genderPalToUse[1]], sGenderPalOffsets[1] + palNum, 2);
    DisplayPartyPokemonBarDetail(battleInfoBox->windowId, symbolToUse, TEXT_COLOR_GENDER_SYMBOL, GENDER_POS_X, GENDER_POS_Y);
}

static void DisplayPartyPokemonNickname(struct Pokemon* mon, struct BattleInfoBox* battleInfoBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE)
    {
        // if (c == 1)
        //     battleInfoBox->infoRects->blitFunc(battleInfoBox->windowId, battleInfoBox->infoRects->dimensions[0] >> 3, battleInfoBox->infoRects->dimensions[1] >> 3, battleInfoBox->infoRects->dimensions[2] >> 3, battleInfoBox->infoRects->dimensions[3] >> 3, FALSE);
        GetMonNickname(mon, nickname);
        DisplayPartyPokemonBarDetail(battleInfoBox->windowId, nickname, TEXT_COLOR_DEFAULT, NICKNAME_POS_X, NICKNAME_POS_Y);
    }
}

static void DisplayPartyPokemonLevel(u8 level, struct BattleInfoBox* battleInfoBox)
{
    logDebug("DisplayPartyPokemonLevel");
    logUnsigned(level);
    logUnsigned(LEVEL_POS_X);
    logUnsigned(LEVEL_POS_Y);
    logUnsigned(battleInfoBox->windowId);
    ConvertIntToDecimalStringN(gStringVar2, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringCopy(gStringVar1, gText_LevelSymbol);
    StringAppend(gStringVar1, gStringVar2);
    // logU8String(gStringVar1);
    DisplayPartyPokemonBarDetail(battleInfoBox->windowId, gStringVar1, 0, LEVEL_POS_X, LEVEL_POS_Y);
}

static void SpriteCB_BouncePartyMonIcon(struct Sprite* sprite)
{
    u8 animCmd = UpdateMonIconFrame(sprite);

    if (animCmd != 0)
    {
        if (animCmd & 1) // % 2 also matches
            sprite->pos2.y = -3;
        else
            sprite->pos2.y = 1;
    }
}

static void SpriteCB_UpdatePartyMonIcon(struct Sprite* sprite)
{
    UpdateMonIconFrame(sprite);
}

static void AnimateSelectedPartyIcon(u8 spriteId, u8 animNum)
{
    gSprites[spriteId].data[0] = 0;
    if (animNum == 0)
    {
        if (gSprites[spriteId].pos1.x == 16)
        {
            gSprites[spriteId].pos2.x = 0;
            gSprites[spriteId].pos2.y = -4;
        }
        else
        {
            gSprites[spriteId].pos2.x = -4;
            gSprites[spriteId].pos2.y = 0;
        }
        gSprites[spriteId].callback = SpriteCB_UpdatePartyMonIcon;
    }
    else
    {
        gSprites[spriteId].pos2.x = 0;
        gSprites[spriteId].pos2.y = 0;
        gSprites[spriteId].callback = SpriteCB_BouncePartyMonIcon;
    }
}

void AnimateBattleInfoSlot(u8 slot, u8 animNum, struct Pokemon* curPokemon, struct BattleInfoBox* curBattleInfoBox)
{
    logDebug("AnimateBattleInfoSlot");

    u8 palFlags = 0;

    if (curPokemon == NULL)
    {
        curPokemon = GetPokemonAtSlot(slot);
    }

    if (curBattleInfoBox == NULL)
    {
        curBattleInfoBox = &sBattleInfoBoxes[slot];
    }

    logUnsigned(slot);
    logUnsigned(animNum);
    logUnsigned(GetMonData(curPokemon, MON_DATA_SPECIES));
    logBool(GetMonData(curPokemon, MON_DATA_SPECIES) != SPECIES_NONE);

    if (GetMonData(curPokemon, MON_DATA_SPECIES) != SPECIES_NONE)
    {
        logUnsigned(curBattleInfoBox->windowId);
        logUnsigned(curBattleInfoBox->monSpriteId);
        logUnsigned(curBattleInfoBox->pokeballSpriteId);
        if (animNum == 1)
            palFlags |= PARTY_PAL_SELECTED;
        if (GetMonData(curPokemon, MON_DATA_HP) == 0)
            palFlags |= PARTY_PAL_FAINTED;

        LoadPartyBoxPalette(curBattleInfoBox->windowId, palFlags);
        AnimateSelectedPartyIcon(curBattleInfoBox->monSpriteId, animNum);
        StartSpriteAnim(&gSprites[curBattleInfoBox->pokeballSpriteId], animNum);
    }
}

static struct Pokemon* GetPokemonAtSlot(u8 slot)
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

#undef NICKNAME_POS_X
#undef NICKNAME_POS_Y
#undef GENDER_POS_X
#undef GENDER_POS_Y

#undef TEXT_COLOR_DEFAULT
#undef TEXT_COLOR_UNUSED_1
#undef TEXT_COLOR_GENDER_SYMBOL
#undef TEXT_COLOR_SELECTION_ACTIONS
#undef TEXT_COLOR_FIELD_MOVES
#undef TEXT_COLOR_UNUSED_2
#undef TEXT_COLOR_HEADER

#undef AILMENT_NONE
#undef AILMENT_PSN
#undef AILMENT_PRZ
#undef AILMENT_SLP
#undef AILMENT_FRZ
#undef AILMENT_BRN
#undef AILMENT_PKRS
#undef AILMENT_FNT

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

// struct BattlePokemon
// {
//     /*0x00*/ u16 species;
//     /*0x02*/ u16 attack;
//     /*0x04*/ u16 defense;
//     /*0x06*/ u16 speed;
//     /*0x08*/ u16 spAttack;
//     /*0x0A*/ u16 spDefense;
//     /*0x0C*/ u16 moves[MAX_MON_MOVES];
//     /*0x14*/ u32 hpIV : 5;
//     /*0x14*/ u32 attackIV : 5;
//     /*0x15*/ u32 defenseIV : 5;
//     /*0x15*/ u32 speedIV : 5;
//     /*0x16*/ u32 spAttackIV : 5;
//     /*0x17*/ u32 spDefenseIV : 5;
//     /*0x17*/ u32 abilityNum : 2;
//     /*0x18*/ s8 statStages[NUM_BATTLE_STATS];
//     /*0x20*/ u16 ability;
//     /*0x22*/ u8 type1;
//     /*0x23*/ u8 type2;
//     /*0x24*/ u8 type3;
//     /*0x25*/ u8 pp[MAX_MON_MOVES];
//     /*0x29*/ u16 hp;
//     /*0x2B*/ u8 level;
//     /*0x2C*/ u8 friendship;
//     /*0x2D*/ u16 maxHP;
//     /*0x2F*/ u16 item;
//     /*0x31*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
//     /*0x3C*/ u8 ppBonuses;
//     /*0x3D*/ u8 otName[PLAYER_NAME_LENGTH + 1];
//     /*0x45*/ u32 experience;
//     /*0x49*/ u32 personality;
//     /*0x4D*/ u32 status1;
//     /*0x51*/ u32 status2;
//     /*0x55*/ u32 otId;
// };
