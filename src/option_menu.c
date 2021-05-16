#include "global.h"
#include "option_menu.h"
#include "main.h"
#include "menu.h"
#include "scanline_effect.h"
#include "palette.h"
#include "sprite.h"
#include "sound.h"
#include "task.h"
#include "malloc.h"
#include "bg.h"
#include "gpu_regs.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "strings.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"

#define Y_DIFF 16 // Difference in pixels between items.

// Menu items page 1
enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    MENUITEM_POKEANIMATION,
#endif
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_HP_BAR,
    MENUITEM_EXP_BAR,
    MENUITEM_UNIT_SYSTEM,
    MENUITEM_FRAMETYPE,
#ifdef FEATURE_CHANGEDEXLOOK
    MENUITEM_DEXTYPE,
#endif
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};
#define MENU_START_INDEX MENUITEM_TEXTSPEED
#define MENU_END_INDEX MENUITEM_CANCEL

// Window Ids
enum
{
    WIN_TEXT_OPTION,
    WIN_OPTIONS
};

struct OptionMenu
{
    u16 sel[MENUITEM_COUNT];
    u16 menuCursor;
    u16 visibleCursor;
    u16 scrollIndicatorArrowPairId;
    u16 SelectionArrowPair;
};

// this file's functions
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u16 cursor);
static void TextSpeed_DrawChoices(int selection, u16 y, u8 textSpeed);
static void BattleScene_DrawChoices(int selection, u16 y, u8 textSpeed);
static void BattleStyle_DrawChoices(int selection, u16 y, u8 textSpeed);
static void HpBar_DrawChoices(int selection, u16 y, u8 textSpeed);
static void UnitSystem_DrawChoices(int selection, u16 y, u8 textSpeed);
static void DexMode_DrawChoices(int selection, u16 y, u8 textSpeed);
static void PokeAnimation_DrawChoices(int selection, u16 y, u8 textSpeed);
static void Sound_DrawChoices(int selection, u16 y, u8 textSpeed);
static void FrameType_DrawChoices(int selection, u16 y, u8 textSpeed);
static void ButtonMode_DrawChoices(int selection, u16 y, u8 textSpeed);
static int FrameType_ProcessInput(int selection);
static int FourOptions_ProcessInput(int selection);
static int ThreeOptions_ProcessInput(int selection);
static int TwoOptions_ProcessInput(int selection);
static int BattleStyle_ProcessInput(int selection);
static int ElevenOptions_ProcessInput(int selection);
static int Sound_ProcessInput(int selection);
static void DrawTextOption(void);
static void DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);

struct
{
    void (*drawChoices)(int selection, u16 y, u8 textSpeed);
    int (*processInput)(int selection);
} static const sItemFunctions[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED] = {TextSpeed_DrawChoices, FourOptions_ProcessInput},
    [MENUITEM_BATTLESCENE] = {BattleScene_DrawChoices, TwoOptions_ProcessInput},
    [MENUITEM_BATTLESTYLE] = {BattleStyle_DrawChoices, BattleStyle_ProcessInput},
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    [MENUITEM_POKEANIMATION] = {PokeAnimation_DrawChoices, TwoOptions_ProcessInput},
#endif
    [MENUITEM_SOUND] = {Sound_DrawChoices, Sound_ProcessInput},
    [MENUITEM_BUTTONMODE] = {ButtonMode_DrawChoices, ThreeOptions_ProcessInput},
    [MENUITEM_FRAMETYPE] = {FrameType_DrawChoices, FrameType_ProcessInput},
    [MENUITEM_HP_BAR] = {HpBar_DrawChoices, ElevenOptions_ProcessInput},
    [MENUITEM_EXP_BAR] = {HpBar_DrawChoices, ElevenOptions_ProcessInput},
    [MENUITEM_UNIT_SYSTEM] = {UnitSystem_DrawChoices, TwoOptions_ProcessInput},
#ifdef FEATURE_CHANGEDEXLOOK
    [MENUITEM_DEXTYPE] = {DexMode_DrawChoices, TwoOptions_ProcessInput},
#endif
    [MENUITEM_CANCEL] = {NULL, NULL},
};

// Stores how many choices any given option will offer (needed for left/right arrows)
static const u16 sNumberOfChoices[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED] = 4,
    [MENUITEM_BATTLESCENE] = 2,
    [MENUITEM_BATTLESTYLE] = 2,
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    [MENUITEM_POKEANIMATION] = 2,
#endif
    [MENUITEM_SOUND] = 3,
    [MENUITEM_BUTTONMODE] = 3,
    [MENUITEM_FRAMETYPE] = 20,
    [MENUITEM_HP_BAR] = 11,
    [MENUITEM_EXP_BAR] = 11,
    [MENUITEM_UNIT_SYSTEM] = 2,
#ifdef FEATURE_CHANGEDEXLOOK
    [MENUITEM_DEXTYPE] = 2,
#endif
    [MENUITEM_CANCEL] = 0,
};

// Will decide if the left/right arrows will show on that particular option
static const bool8 sShowSelectionArrows[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED] = TRUE,
    [MENUITEM_BATTLESCENE] = TRUE,
    [MENUITEM_BATTLESTYLE] = TRUE,
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    [MENUITEM_POKEANIMATION] = TRUE,
#endif
    [MENUITEM_SOUND] = TRUE,
    [MENUITEM_BUTTONMODE] = TRUE,
    [MENUITEM_FRAMETYPE] = TRUE,
    [MENUITEM_HP_BAR] = TRUE,
    [MENUITEM_EXP_BAR] = TRUE,
    [MENUITEM_UNIT_SYSTEM] = TRUE,
#ifdef FEATURE_CHANGEDEXLOOK
    [MENUITEM_DEXTYPE] = TRUE,
#endif
    [MENUITEM_CANCEL] = FALSE,
};

// static const bool8 sShowSelectionArrows[MENUITEM_COUNT] =
// {
//     [MENUITEM_TEXTSPEED] = TRUE,
//     [MENUITEM_BATTLESCENE] = FALSE,
//     [MENUITEM_BATTLESTYLE] = FALSE,
//     [MENUITEM_SOUND] = FALSE,
//     [MENUITEM_BUTTONMODE] = FALSE,
//     [MENUITEM_FRAMETYPE] = TRUE,
//     [MENUITEM_HP_BAR] = TRUE,
//     [MENUITEM_EXP_BAR] = TRUE,
//     [MENUITEM_UNIT_SYSTEM] = FALSE,
//     [MENUITEM_DEXTYPE] = FALSE,
//     [MENUITEM_CANCEL] = FALSE,
// };

// EWRAM vars
EWRAM_DATA static struct OptionMenu *sOptions = NULL;

// const rom data
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/misc/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/misc/option_menu_equals_sign.4bpp");


static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyle,
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    [MENUITEM_POKEANIMATION] = gText_PokeAnimation,
#endif
    [MENUITEM_SOUND]       = gText_Sound,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_FRAMETYPE]   = gText_Frame,
    [MENUITEM_HP_BAR]      = sText_HpBar,
    [MENUITEM_EXP_BAR]     = sText_ExpBar,
    [MENUITEM_UNIT_SYSTEM] = sText_UnitSystem,
#ifdef FEATURE_CHANGEDEXLOOK
    [MENUITEM_DEXTYPE]     = gText_DexType,
#endif
    [MENUITEM_CANCEL]      = gText_OptionMenuCancel,
};


static const u8 *const sTextSpeedStrings[] = {gText_TextSpeedSlow, gText_TextSpeedMid, gText_TextSpeedFast, gText_Instant};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
   {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

// code
static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void DrawChoices(u32 id, u16 y, u8 textSpeed)
{
    if (sItemFunctions[id].drawChoices != NULL)
        sItemFunctions[id].drawChoices(sOptions->sel[id], y, textSpeed);
}


static void OptionMenu_PlaceTopMenuScrollIndicatorArrows(void)
{
    sOptions->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP,
        DISPLAY_WIDTH / 2,
        40,
        150,
        MENU_END_INDEX,
        110,
        110,
        &sOptions->menuCursor);
}

static void OptionMenu_RemoveScrollIndicatorArrowPair(void)
{
    if (sOptions->scrollIndicatorArrowPairId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sOptions->scrollIndicatorArrowPairId);
        sOptions->scrollIndicatorArrowPairId = 0xFF;
    }
}

static void OptionMenu_PlaceOptionSelectionArrowPair(void)
{
    sOptions->SelectionArrowPair = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_LEFT,
        sOptions->visibleCursor * Y_DIFF + 48,
        (DISPLAY_WIDTH / 2) - 8,
        DISPLAY_WIDTH -  (Y_DIFF) - 4,
        sNumberOfChoices[sOptions->menuCursor]-1,
        111,
        111,
        &sOptions->sel[sOptions->menuCursor]);
}

static void OptionMenu_RemoveOptionSelectionArrowPair(void)
{
    if (sOptions->SelectionArrowPair != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sOptions->SelectionArrowPair);
        sOptions->SelectionArrowPair = 0xFF;
    }
}

void CB2_InitOptionMenu(void)
{
    u32 i, taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void*)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ResetBgPositions();
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, 0, sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, 16, sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(0);
        DrawTextOption();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(1);
        DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(Task_OptionMenuFadeIn, 0);

        sOptions = AllocZeroed(sizeof(*sOptions));
        sOptions->sel[MENUITEM_TEXTSPEED]   = gSaveBlock2Ptr->optionsTextSpeed;
        sOptions->sel[MENUITEM_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        sOptions->sel[MENUITEM_BATTLESTYLE] = gSaveBlock2Ptr->optionsBattleStyle;
        sOptions->sel[MENUITEM_SOUND]       = gSaveBlock2Ptr->optionsSound;
        sOptions->sel[MENUITEM_BUTTONMODE]  = gSaveBlock2Ptr->optionsButtonMode;
        sOptions->sel[MENUITEM_FRAMETYPE]   = gSaveBlock2Ptr->optionsWindowFrameType;
        sOptions->sel[MENUITEM_HP_BAR]      = gSaveBlock2Ptr->optionsHpBarSpeed;
        sOptions->sel[MENUITEM_EXP_BAR]     = gSaveBlock2Ptr->optionsExpBarSpeed;
        sOptions->sel[MENUITEM_UNIT_SYSTEM] = gSaveBlock2Ptr->optionsUnitSystem;
#ifdef FEATURE_CHANGEDEXLOOK
        sOptions->sel[MENUITEM_DEXTYPE] = gSaveBlock2Ptr->optionsDexMode;
#endif
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
        sOptions->sel[MENUITEM_POKEANIMATION] = gSaveBlock2Ptr->optionsPokeAnimations;
#endif

        for (i = 0; i < 7; i++)
            DrawChoices(i, i * Y_DIFF, 0xFF);

        HighlightOptionMenuItem(sOptions->menuCursor);

        CopyWindowToVram(WIN_OPTIONS, 3);
        gMain.state++;
        break;
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        OptionMenu_PlaceTopMenuScrollIndicatorArrows();
        OptionMenu_PlaceOptionSelectionArrowPair();
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    if (direction == 0) // scroll down
        menuItem = sOptions->menuCursor + 3, pos = 6;
    else
        menuItem = sOptions->menuCursor - 3, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
    AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemsNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount = MENUITEM_COUNT - 7;
    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(0));

    // Clear moved items
    if (direction == 0)
    {
        y = 7 - scrollCount;
        if (y < 0)
            y = 7;
        y *= Y_DIFF;
    }
    else
    {
        y = 0;
    }

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, y, 26 * 8, Y_DIFF * scrollCount);
    // Print new texts
    for (i = 0; i < scrollCount; i++)
    {
        if (direction == 0) // From top to bottom
            menuItem = MENUITEM_COUNT - 1 - i, pos = 6 - i;
        else // From bottom to top
            menuItem = i, pos = i;
        DrawChoices(menuItem, pos * Y_DIFF, 0xFF);
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemsNames[menuItem], 8, (pos * Y_DIFF) + 1, 0xFF, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, 2);
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    int i, scrollCount = 0, itemsToRedraw;
    if (JOY_NEW(A_BUTTON))
    {
        if (sOptions->menuCursor == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sOptions->menuCursor == sOptions->visibleCursor - 1)
                sOptions->visibleCursor--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (sOptions->menuCursor == MENU_START_INDEX) // Scroll all the way to the bottom.
            {
                sOptions->visibleCursor = sOptions->menuCursor = 3;
                ScrollAll(0);
                sOptions->visibleCursor = 6;
                sOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sOptions->menuCursor--;
                sOptions->visibleCursor--;
            }
        }
        HighlightOptionMenuItem(sOptions->visibleCursor);
        OptionMenu_RemoveOptionSelectionArrowPair();
        if (sShowSelectionArrows[sOptions->menuCursor] == TRUE)
            OptionMenu_PlaceOptionSelectionArrowPair();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sOptions->visibleCursor == 3) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sOptions->menuCursor == MENUITEM_COUNT - 3)
                sOptions->visibleCursor++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sOptions->menuCursor >= MENUITEM_COUNT) // Scroll all the way to the tom.
            {
                sOptions->visibleCursor = 3;
                sOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sOptions->visibleCursor = sOptions->menuCursor = 0;
            }
            else
            {
                sOptions->visibleCursor++;
            }
        }
        HighlightOptionMenuItem(sOptions->visibleCursor);
        OptionMenu_RemoveOptionSelectionArrowPair();
        if (sShowSelectionArrows[sOptions->menuCursor] == TRUE)
            OptionMenu_PlaceOptionSelectionArrowPair();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        int cursor = sOptions->menuCursor;
        u8 previousOption = sOptions->sel[cursor];
        if (sItemFunctions[cursor].processInput != NULL)
            sOptions->sel[cursor] = sItemFunctions[cursor].processInput(previousOption);

        if (previousOption != sOptions->sel[cursor])
            DrawChoices(cursor, sOptions->visibleCursor * Y_DIFF, 0);
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2Ptr->optionsTextSpeed =       sOptions->sel[MENUITEM_TEXTSPEED];
    gSaveBlock2Ptr->optionsBattleSceneOff =  sOptions->sel[MENUITEM_BATTLESCENE];
    gSaveBlock2Ptr->optionsBattleStyle =     sOptions->sel[MENUITEM_BATTLESTYLE];
    gSaveBlock2Ptr->optionsSound =           sOptions->sel[MENUITEM_SOUND];
    gSaveBlock2Ptr->optionsButtonMode =      sOptions->sel[MENUITEM_BUTTONMODE];
    gSaveBlock2Ptr->optionsWindowFrameType = sOptions->sel[MENUITEM_FRAMETYPE];
    gSaveBlock2Ptr->optionsHpBarSpeed =      sOptions->sel[MENUITEM_HP_BAR];
    gSaveBlock2Ptr->optionsExpBarSpeed =     sOptions->sel[MENUITEM_EXP_BAR];
    gSaveBlock2Ptr->optionsUnitSystem =      sOptions->sel[MENUITEM_UNIT_SYSTEM];
#ifdef FEATURE_CHANGEDEXLOOK
    gSaveBlock2Ptr->optionsDexMode =         sOptions->sel[MENUITEM_DEXTYPE];
#endif
#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
    gSaveBlock2Ptr->optionsPokeAnimations =  sOptions->sel[MENUITEM_POKEANIMATION];
#endif



    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        OptionMenu_RemoveOptionSelectionArrowPair();
        OptionMenu_RemoveScrollIndicatorArrowPair();
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sOptions);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightOptionMenuItem(u16 cursor)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(Y_DIFF, DISPLAY_WIDTH - Y_DIFF));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(cursor * Y_DIFF + 40, cursor * Y_DIFF + 56));
}

// Process Input functions
static int XOptions_ProcessInput(int x, int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (++selection > (x - 1))
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (--selection < 0)
            selection = (x - 1);
    }
    return selection;
}

static int ThreeOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(3, selection);
}

static int FourOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(4, selection);
}

static int ElevenOptions_ProcessInput(int selection)
{
    return XOptions_ProcessInput(11, selection);
}

static int TwoOptions_ProcessInput(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int BattleStyle_ProcessInput(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int Sound_ProcessInput(int selection)
{
    int previous = selection;

    selection = ThreeOptions_ProcessInput(selection);
    if (selection == 0 || selection == 1)
    {
        gDisableMusic = FALSE;
        SetPokemonCryStereo(selection);
        if (previous == 2)
            PlayNewMapMusic(GetCurrentMapMusic());
    }
    else
    {
        PlayBGM(0);
        gDisableMusic = TRUE;
    }

    return selection;
}

static int FrameType_ProcessInput(int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    return selection;
}

static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

// Draw Choices functions
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, u8 textSpeed)
{
    u8 dst[16];
    u32 i;

    for (i = 0; *text != EOS && i <= 14; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = 4;
        dst[5] = 5;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, 1, dst, x, y + 1, textSpeed, NULL);
}

static void HpBar_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    if (selection < 10)
    {
        u8 textPlus[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}+1{0x77}{0x77}{0x77}{0x77}{0x77}"); // 0x77 is to clear INSTANT text
        textPlus[7] = CHAR_0 + selection;
        DrawOptionMenuChoice(textPlus, 104, y, 1, textSpeed);
    }
    else
    {
        DrawOptionMenuChoice(gText_Instant, 104, y, 1, textSpeed);
    }
}

static void doTwoChoices(const u8* text1, const u8* text2, int selection, u16 y, u8 textSpeed)
{
    u8 styles[2] = { 0, 0 };

    styles[selection] = 1;
    DrawOptionMenuChoice(text1, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(text2, GetStringRightAlignXOffset(1, text2, 198), y, styles[1], textSpeed);
}

static void BattleScene_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    doTwoChoices(gText_BattleSceneOn, gText_BattleSceneOff, selection, y, textSpeed);
}

static void BattleStyle_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    doTwoChoices(gText_BattleStyleShift, gText_BattleStyleSet, selection, y, textSpeed);
}

static void UnitSystem_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    doTwoChoices(gText_UnitSystemMetric, gText_UnitSystemImperial, selection, y, textSpeed);
}

#ifdef FEATURE_CHANGEDEXLOOK
static void DexMode_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    doTwoChoices(gText_Light, gText_Dark, selection, y, textSpeed);
}
#endif

#ifdef FEATURE_TOGGLEPOKEMONBATTLEANIMATIONS
static void PokeAnimation_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    doTwoChoices(gText_BattleSceneOn, gText_BattleSceneOff, selection, y, textSpeed);
}
#endif

static void FourOptions_DrawChoices(const u8 *const *const strings, int selection, u16 y, u8 textSpeed)
{
    static const u8 choiceOrders[][3] =
    {
        {0, 1, 2},
        {0, 1, 2},
        {1, 2, 3},
        {1, 2, 3},
    };
    u8 styles[4] = {0};
    int xMid;
    const u8 *order = choiceOrders[selection];

    styles[selection] = 1;
    xMid = GetMiddleX(strings[order[0]], strings[order[1]], strings[order[2]]);

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, y, 26 * 8 - 104, Y_DIFF);
    CopyWindowToVram(WIN_OPTIONS, 2);

    DrawOptionMenuChoice(strings[order[0]], 104, y, styles[order[0]], textSpeed);
    DrawOptionMenuChoice(strings[order[1]], xMid, y, styles[order[1]], textSpeed);
    DrawOptionMenuChoice(strings[order[2]], GetStringRightAlignXOffset(1, strings[order[2]], 198), y, styles[order[2]], textSpeed);
}

static void TextSpeed_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    FourOptions_DrawChoices(sTextSpeedStrings, selection, y, textSpeed);
}


static void Sound_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    u8 styles[3] = {0, 0, 0};
    int xMid = GetMiddleX(gText_SoundMono, gText_SoundStereo, gText_BattleSceneOff);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_SoundMono, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_SoundStereo, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[2], textSpeed);
}

static void FrameType_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    u8 text[16];
    u32 n = selection + 1;
    u32 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = 0x77;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, y, 0, textSpeed);
    DrawOptionMenuChoice(text, 128, y, 1, textSpeed);
}

static void ButtonMode_DrawChoices(int selection, u16 y, u8 textSpeed)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_ButtonTypeNormal, gText_ButtonTypeLR, gText_ButtonTypeLEqualsA);

    styles[selection] = 1;
    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, y, styles[0], textSpeed);
    DrawOptionMenuChoice(gText_ButtonTypeLR, xMid, y, styles[1], textSpeed);
    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(1, gText_ButtonTypeLEqualsA, 198), y, styles[2], textSpeed);
}

static void DrawTextOption(void)
{
    FillWindowPixelBuffer(WIN_TEXT_OPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_TEXT_OPTION, 1, gText_Option, 8, 1, 0, NULL);
    CopyWindowToVram(WIN_TEXT_OPTION, 3);
}

static void DrawOptionMenuTexts(void)
{
    u32 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < 7; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, 1, sOptionMenuItemsNames[i], 8, (i * Y_DIFF) + 1, 0, NULL);

    CopyWindowToVram(WIN_OPTIONS, 3);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
