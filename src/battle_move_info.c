#include "global.h"
#include "graphics.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_bg.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "palette.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text_window.h"
#include "trig.h"
#include "menu.h"
#include "window.h"
#include "main.h"
#include "malloc.h"
#include "pokemon_summary_screen.h"
#include "strings.h"
#include "constants/songs.h"

//gMoveDescriptionPointers
// #include "data/text/move_descriptions.h"

/* MACROS */
// Window indexes
#define WIN_DESCRIPTION   0
#define WIN_EFFECT_LABELS 1
#define WIN_EFFECT_VALUES 2
// Which background index will everything be done on
#define SLIDER_BG 1
// The tag for the split icons
#define TAG_SPLIT_ICONS 30004
// The Y positions of the background when the slider is open / closed
#define SLIDER_POS_CLOSED_Y 65531  // -5
#define SLIDER_POS_OPENED_Y 45
// The Sprite's default position
#define SPRITE_POS_X  46
#define SPRITE_POS_Y 133
// Number of frames of slide animation; one frame should be ~16.6667ms
#define SLIDER_NUM_FRAMES 16
// The offset / width for the windows
#define WIN_DESCRIPTION_X 1
#define WIN_DESCRIPTION_Y 0
#define WIN_DESCRIPTION_WIDTH  150
#define WIN_DESCRIPTION_HEIGHT  30
#define WIN_DESCRIPTION_FILL    13

#define WIN_EFFECT_LABELS_X 0
#define WIN_EFFECT_LABELS_Y 0
#define WIN_EFFECT_LABELS_WIDTH  104
#define WIN_EFFECT_LABELS_HEIGHT  30
#define WIN_EFFECT_LABELS_FILL    12

#define WIN_EFFECT_VALUES_X 4
#define WIN_EFFECT_VALUES_Y 0
#define WIN_EFFECT_VALUES_WIDTH  19
#define WIN_EFFECT_VALUES_HEIGHT 30
#define WIN_EFFECT_VALUES_FILL   13

static const u8 sTextColors[][3] =
{
    [WIN_DESCRIPTION] = {WIN_DESCRIPTION_FILL,   1, 15},
    [WIN_EFFECT_LABELS] = {WIN_EFFECT_LABELS_FILL, 13, 1},
    [WIN_EFFECT_VALUES] = {WIN_EFFECT_VALUES_FILL, 1, 15},
};

static const struct WindowTemplate sBattleMoveInfoWindowTemplates[] =
{
    [WIN_DESCRIPTION] = { // 0 Description
        .bg = SLIDER_BG,
        .tilemapLeft = 11,
        .tilemapTop = 15,
        .width = 19,
        .height = 4,
        .paletteNum = 10,
        .baseBlock = 180,
    },
    [WIN_EFFECT_LABELS] = { // 1 Effect Labels
        .bg = SLIDER_BG,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 6,
        .height = 4,
        .paletteNum = 10,
        .baseBlock = 156,
    },
    [WIN_EFFECT_VALUES] = { // 2 Effect Values
        .bg = SLIDER_BG,
        .tilemapLeft = 7,
        .tilemapTop = 15,
        .width = 3,
        .height = 4,
        .paletteNum = 10,
        .baseBlock = 144,
    },
    DUMMY_WIN_TEMPLATE
};


static const u8 sSliderAnimationOpen[SLIDER_NUM_FRAMES] = {
    10,  //  1: -5 =>  5 | 10
     8,  //  2:  5 => 13 | 18
     6,  //  3: 13 => 19 | 24
     5,  //  4: 19 => 24 | 29
     4,  //  5: 24 => 28 | 33
     3,  //  6: 28 => 31 | 36
     3,  //  7: 31 => 34 | 39
     2,  //  8: 34 => 36 | 41
     2,  //  9: 36 => 38 | 43
     2,  // 10: 38 => 40 | 45
     1,  // 11: 40 => 41 | 46
     1,  // 12: 41 => 42 | 47
     1,  // 13: 42 => 43 | 48
     1,  // 14: 43 => 44 | 49
     0,  // 15: 44 => 44 | 49
     1,  // 16: 44 => 45 | 50
};

static const u8 sSliderAnimationClose[SLIDER_NUM_FRAMES] = {
     1,  //  1: 45 => 44 |  1
     1,  //  2: 44 => 43 |  2
     2,  //  3: 43 => 41 |  4
     2,  //  4: 41 => 39 |  6
     3,  //  5: 39 => 36 |  9
     5,  //  6: 36 => 31 | 14
     5,  //  7: 31 => 26 | 19
     6,  //  8: 26 => 20 | 25
     6,  //  9: 20 => 14 | 31
     5,  // 10: 14 =>  9 | 36
     5,  // 11:  9 =>  4 | 40
     3,  // 12:  4 =>  1 | 43
     2,  // 13:  1 => -1 | 45
     2,  // 14: -1 => -3 | 47
     1,  // 15: -3 => -4 | 49
     1,  // 16: -4 => -5 | 50
};

static EWRAM_DATA struct BattleMoveInfoData
{
    bool8 isLoaded;
    bool8 isOpen;
    u8 sliderAnimationTaskId;
    u8 graphicsLoadState;
    u8 splitIconSpriteId;
    u8 windowIdDescription;
    u8 windowIdEffectLabels;
    u8 windowIdEffectValues;
    u16 tempWinH;
    u16 tempWinV;
    u16 tempGpuReg;
    u8* tilemapBufferPtr;
} *sBattleMoveInfo = NULL;

static const u32 sBattleMoveInfoGui_Tilemap[] = INCBIN_U32("graphics/battle_interface/moveInfo_map.bin.lz");
static const u32 sBattleMoveInfoGui_Gfx[] = INCBIN_U32("graphics/battle_interface/moveInfo_map.4bpp.lz");
static const u32 sBattleMoveInfoGui_Pal[] = INCBIN_U32("graphics/battle_interface/moveInfo_map.gbapal.lz");
static const u16 sSplitIcons_Pal[] = INCBIN_U16("graphics/interface/split_icons.gbapal");
static const u32 sSplitIcons_Gfx[] = INCBIN_U32("graphics/interface/split_icons.4bpp.lz");

static const struct OamData sOamData_SplitIcons =
{
    .size = SPRITE_SIZE(16x16),
    .shape = SPRITE_SHAPE(16x16),
    .priority = 0,
};

static const struct CompressedSpriteSheet sSpriteSheet_SplitIcons =
{
    .data = sSplitIcons_Gfx,
    .size = 16 * 16 * 3 / 2,
    .tag = TAG_SPLIT_ICONS,
};

static const struct SpritePalette sSpritePal_SplitIcons =
{
    .data = sSplitIcons_Pal,
    .tag = TAG_SPLIT_ICONS };

static const union AnimCmd sSpriteAnim_SplitIcon0[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END };

static const union AnimCmd sSpriteAnim_SplitIcon1[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END };

static const union AnimCmd sSpriteAnim_SplitIcon2[] =
{
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_END };

static const union AnimCmd* const sSpriteAnimTable_SplitIcons[] =
{
    sSpriteAnim_SplitIcon0,
    sSpriteAnim_SplitIcon1,
    sSpriteAnim_SplitIcon2,
};

static const struct SpriteTemplate sSpriteTemplate_SplitIcons =
{
    .tileTag = TAG_SPLIT_ICONS,
    .paletteTag = TAG_SPLIT_ICONS,
    .oam = &sOamData_SplitIcons,
    .anims = sSpriteAnimTable_SplitIcons,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy };

static const u8 sEffectLabelText[] = _("Power\nAccuracy");
static const u8 sEffectValueTextPlaceholder[] = _("{STR_VAR_2}\n{STR_VAR_1}");

// code
// Confirms if we need to allocate the battle move info struct
bool8 BattleMoveInfo_NeedsInit(void)
{
    return sBattleMoveInfo == NULL;
}

// Allocates the struct
void BattleMoveInfo_Init(void)
{
    sBattleMoveInfo = AllocZeroed(sizeof(struct BattleMoveInfoData));
}

// Frees up the main struct, destroys the move-type sprite
void BattleMoveInfo_Clean(void)
{
    Free(sBattleMoveInfo);
    DestroySprite(&gSprites[sBattleMoveInfo->splitIconSpriteId]);
    sBattleMoveInfo = NULL;
}

// Checks if the loading process has been run yet
bool8 BattleMoveInfo_isLoaded(void)
{
    return sBattleMoveInfo->isLoaded;
}

// temp macros for the animation task
#define step    data[2]
#define runOpen data[3]
// Animate the sprite and window
static void Task_RunSliderAnimation(u8 taskId)
{
    // If we're opening, turn the sprite visible
    if (gTasks[taskId].runOpen)
        gSprites[sBattleMoveInfo->splitIconSpriteId].invisible = FALSE;
    if (gTasks[taskId].step == SLIDER_NUM_FRAMES + 1)
    {
        // Clean up the task, and reset the window data to pre-animation state
        DestroyTask(taskId);
        sBattleMoveInfo->sliderAnimationTaskId = 0xFF;
        gBattle_WIN0H = sBattleMoveInfo->tempWinH;
        gBattle_WIN0V = sBattleMoveInfo->tempWinV;
        SetGpuReg(REG_OFFSET_WINOUT, sBattleMoveInfo->tempGpuReg);
    }
    else if (gTasks[taskId].step == SLIDER_NUM_FRAMES)
    {
        // Animate only the background
        if (gTasks[taskId].runOpen)
            gBattle_BG1_Y += sSliderAnimationOpen[gTasks[taskId].step - 1];
        else
            gBattle_BG1_Y -= sSliderAnimationClose[gTasks[taskId].step - 1];
    }
    else if (gTasks[taskId].step == 0)
    {
        // Animate only the sprite
        if (gTasks[taskId].runOpen)
            gSprites[sBattleMoveInfo->splitIconSpriteId].pos2.y -= sSliderAnimationOpen[gTasks[taskId].step];
        else
            gSprites[sBattleMoveInfo->splitIconSpriteId].pos2.y += sSliderAnimationClose[gTasks[taskId].step];
    }
    else
    {
        // Animate both the background and the sprite
        if (gTasks[taskId].runOpen)
        {
            gSprites[sBattleMoveInfo->splitIconSpriteId].pos2.y -= sSliderAnimationOpen[gTasks[taskId].step];
            gBattle_BG1_Y += sSliderAnimationOpen[gTasks[taskId].step - 1];
        }
        else
        {
            gSprites[sBattleMoveInfo->splitIconSpriteId].pos2.y += sSliderAnimationClose[gTasks[taskId].step];
            gBattle_BG1_Y -= sSliderAnimationClose[gTasks[taskId].step - 1];
        }
    }
    // It seems to take multiple frames from flagging a sprite to be invisible
    if (!gTasks[taskId].runOpen && gTasks[taskId].step == SLIDER_NUM_FRAMES - 2)
    {
        gSprites[sBattleMoveInfo->splitIconSpriteId].invisible = TRUE;
    }
    ++gTasks[taskId].step;
}
void BattleMoveInfo_ToggleSlider(void)
{
    // Ignore any calls for the slider to toggle if the animation is already running
    if (sBattleMoveInfo->sliderAnimationTaskId == 0xFF)
    {
        // Move all the window data into temps so we can restore them later
        sBattleMoveInfo->tempWinH = gBattle_WIN0H;
        sBattleMoveInfo->tempWinV = gBattle_WIN0V;
        sBattleMoveInfo->tempGpuReg = GetGpuReg(REG_OFFSET_WINOUT);
        // Move the window so it covers the screen other then the bottom message box
        gBattle_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
        gBattle_WIN0V = WIN_RANGE(0, 112);
        // Make it so objects are hidden if they're outside of a window
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_CLR);

        // Toggle the actual open bool
        sBattleMoveInfo->isOpen = !sBattleMoveInfo->isOpen;
        // Start and set up the animation task
        sBattleMoveInfo->sliderAnimationTaskId = CreateTask(Task_RunSliderAnimation, 8);
        gTasks[sBattleMoveInfo->sliderAnimationTaskId].step = 0;
        gTasks[sBattleMoveInfo->sliderAnimationTaskId].runOpen = sBattleMoveInfo->isOpen;
    }
}
#undef step
#undef runOpen

// Temp macro to help with the window setup
#define setupWindow(__windowId__, __macro__) \
sBattleMoveInfo->__windowId__ = AddWindow(&sBattleMoveInfoWindowTemplates[__macro__]); \
FillWindowPixelBuffer(sBattleMoveInfo->__windowId__, PIXEL_FILL(__macro__##_FILL))

// Setup all three windows
static void BattleMoveInfo_SetupWindows(void)
{
    setupWindow(windowIdDescription, WIN_DESCRIPTION);
    setupWindow(windowIdEffectLabels, WIN_EFFECT_LABELS);
    setupWindow(windowIdEffectValues, WIN_EFFECT_VALUES);
}
#undef setupWindow

// Temp macro to help with getting windows drawn
#define prepWindow(__windowId__) \
PutWindowTilemap(sBattleMoveInfo->__windowId__); \
CopyWindowToVram(sBattleMoveInfo->__windowId__, 3)

// Load up all the graphics data into memory
void BattleMoveInfo_LoadAndPrep(void)
{
    // Set up the background
    sBattleMoveInfo->tilemapBufferPtr = Alloc(0x800);
    memset(sBattleMoveInfo->tilemapBufferPtr, 0, 0x800);
    ResetTempTileDataBuffers();
    DecompressAndCopyTileDataToVram(SLIDER_BG, sBattleMoveInfoGui_Gfx, 0, 0x380, 0);
    LZDecompressWram(sBattleMoveInfoGui_Tilemap, sBattleMoveInfo->tilemapBufferPtr);
    SetBgTilemapBuffer(SLIDER_BG, sBattleMoveInfo->tilemapBufferPtr);
    LoadCompressedPalette(sBattleMoveInfoGui_Pal, 160, 0x20);
    SetBgTilemapPalette(SLIDER_BG, 0, 11, 30, 9, 10);
    CopyBgTilemapBufferToVram(SLIDER_BG);
    ScheduleBgCopyTilemapToVram(SLIDER_BG);
    ShowBg(SLIDER_BG);
    gBattle_BG1_Y = SLIDER_POS_CLOSED_Y;

    // Set up all the windows
    BattleMoveInfo_SetupWindows();
    StringCopy(gStringVar4, sEffectLabelText);
    AddTextPrinterParameterized4(sBattleMoveInfo->windowIdEffectLabels, 1, WIN_EFFECT_LABELS_X, WIN_EFFECT_LABELS_Y, 0, 0, sTextColors[WIN_EFFECT_LABELS], 0, gStringVar4);
    prepWindow(windowIdDescription);
    prepWindow(windowIdEffectLabels);
    prepWindow(windowIdEffectValues);
    ScheduleBgCopyTilemapToVram(SLIDER_BG);

    // Set up the struct
    sBattleMoveInfo->isOpen = FALSE;
    sBattleMoveInfo->sliderAnimationTaskId = 0xFF;
    sBattleMoveInfo->graphicsLoadState = 0;

    // Load up the split icon
    LoadCompressedSpriteSheet(&sSpriteSheet_SplitIcons);
    LoadSpritePalette(&sSpritePal_SplitIcons);
    sBattleMoveInfo->splitIconSpriteId = CreateSprite(&sSpriteTemplate_SplitIcons, SPRITE_POS_X, SPRITE_POS_Y, 0);
    gSprites[sBattleMoveInfo->splitIconSpriteId].invisible = TRUE;

    // Flag that everything has been loaded
    sBattleMoveInfo->isLoaded = TRUE;
}
#undef prepWindow

// Show the move info graphics
void BattleMoveInfo_Show(void)
{
    gSprites[sBattleMoveInfo->splitIconSpriteId].invisible = !sBattleMoveInfo->isOpen;
    ShowBg(SLIDER_BG);
}

// Task to hide the move info
static void Task_HideSlider(u8 taskId)
{
    switch (gTasks[taskId].data[1]++)
    {
    case 0:
        gSprites[sBattleMoveInfo->splitIconSpriteId].invisible = TRUE;
        break;
    case 1:
        HideBg(SLIDER_BG);
    default:
        DestroyTask(taskId);
        break;
    }
}

// Run the task to hide all the graphics related to the move info slider
void BattleMoveInfo_Hide(void)
{
    u8 taskId = CreateTask(Task_HideSlider, 0);
    gTasks[taskId].data[1] = 0;
}

// Updates the text on the slider whenever a new move is selected
void BattleMoveInfo_UpdateMove(u16 moveId)
{
    const u8* text;

    FillWindowPixelBuffer(sBattleMoveInfo->windowIdDescription, PIXEL_FILL(WIN_DESCRIPTION_FILL));
    FillWindowPixelBuffer(sBattleMoveInfo->windowIdEffectValues, PIXEL_FILL(WIN_EFFECT_VALUES_FILL));
    StringCopy(gStringVar4, gMoveDescriptionPointers[(moveId - 1)]);
    AddTextPrinterParameterized4(sBattleMoveInfo->windowIdDescription, 1, WIN_DESCRIPTION_X, WIN_DESCRIPTION_Y, 0, 0, sTextColors[WIN_DESCRIPTION], 0, gStringVar4);
    if (gBattleMoves[moveId - 1].power < 2)
    {
        text = gText_ThreeDashes;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveId - 1].power, STR_CONV_MODE_RIGHT_ALIGN, 3);
        text = gStringVar1;
    }
    StringCopy(gStringVar2, text);

    if (gBattleMoves[moveId - 1].accuracy == 0)
    {
        text = gText_ThreeDashes;
        StringCopy(gStringVar1, text);
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveId - 1].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
        text = gStringVar1;
    }
    StringExpandPlaceholders(gStringVar4, sEffectValueTextPlaceholder);
    AddTextPrinterParameterized4(sBattleMoveInfo->windowIdEffectValues, 1, WIN_EFFECT_VALUES_X, WIN_EFFECT_VALUES_Y, 0, 0, sTextColors[WIN_EFFECT_VALUES], 0, gStringVar4);

    StartSpriteAnim(&gSprites[sBattleMoveInfo->splitIconSpriteId], GetBattleMoveSplit(moveId));
}
