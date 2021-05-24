#include "global.h"
#include "item.h"
#include "string_util.h"
#include "event_data.h"
#include "malloc.h"
#include "strings.h"
#include "window.h"
#include "menu.h"
#include "item_icon.h"
#include "overworld.h"
#include "gpu_regs.h"
#include "sound.h"
#include "constants/songs.h"

// EWRAM variables
EWRAM_DATA static u8 sHeaderBoxWindowId = 0;
EWRAM_DATA static u8 sItemIconSpriteId1 = 0;
EWRAM_DATA static u8 sItemIconSpriteId2 = 0;

// rodata
// #include "data/items.h"


#define ITEM_ICON_X 26
#define ITEM_ICON_Y 24
#define ITEM_TAG    0x2722 //same as money label

void DrawToastBox(u8 left, u8 top, u8 width, u8 height)
{
    struct WindowTemplate template;

    SetWindowTemplateFields(&template, 0, left, top, width, height, 15, 8);
    sHeaderBoxWindowId = AddWindow(&template);
    FillWindowPixelBuffer(sHeaderBoxWindowId, PIXEL_FILL(0));
    PutWindowTilemap(sHeaderBoxWindowId);
    CopyWindowToVram(sHeaderBoxWindowId, 3);
    DrawStdFrameWithCustomTileAndPalette(sHeaderBoxWindowId, FALSE, 0x214, 14);
}

void ShowItemIconSprite(u16 item, bool8 firstTime, bool8 flash)
{
    s16 x, y;
    u8 iconSpriteId;
    u8 spriteId2 = MAX_SPRITES;

    if (flash)
    {
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
        SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
    }

    iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, item);
    if (flash)
        spriteId2 = AddItemIconSprite(ITEM_TAG, ITEM_TAG, item);

    if (iconSpriteId != MAX_SPRITES)
    {
        if (!firstTime)
        {
            //show in message box
            x = 215;
            y = 140;
        }
        else
        {
            // show in header box
            x = ITEM_ICON_X;
            y = ITEM_ICON_Y;
        }

        gSprites[iconSpriteId].pos2.x = x;
        gSprites[iconSpriteId].pos2.y = y;
        gSprites[iconSpriteId].oam.priority = 0;
    }

    if (spriteId2 != MAX_SPRITES)
    {
        gSprites[spriteId2].pos2.x = x;
        gSprites[spriteId2].pos2.y = y;
        gSprites[spriteId2].oam.priority = 0;
        gSprites[spriteId2].oam.objMode = ST_OAM_OBJ_WINDOW;
        sItemIconSpriteId2 = spriteId2;
    }

    sItemIconSpriteId1 = iconSpriteId;
}

void DestroyItemIconSprite(void)
{
    FreeSpriteTilesByTag(ITEM_TAG);
    FreeSpritePaletteByTag(ITEM_TAG);
    FreeSpriteOamMatrix(&gSprites[sItemIconSpriteId1]);
    DestroySprite(&gSprites[sItemIconSpriteId1]);

    if (Overworld_GetFlashLevel() > 1 && sItemIconSpriteId2 != MAX_SPRITES)
    {
        FreeSpriteOamMatrix(&gSprites[sItemIconSpriteId2]);
        DestroySprite(&gSprites[sItemIconSpriteId2]);
    }
}

void HideToastBox()
{
    ClearStdWindowAndFrameToTransparent(sHeaderBoxWindowId, FALSE);
    CopyWindowToVram(sHeaderBoxWindowId, 3);
    RemoveWindow(sHeaderBoxWindowId);
}

void DrawToastBoxText(u8* dst, u8 x, u8 y)
{
    AddTextPrinterParameterized(sHeaderBoxWindowId, 0, dst, x, y, 0, NULL);
}

void DrawHeaderBox(u16 itemToShow, u8* dst, bool8 handleFlash, u8 numLinesToDraw)
{
    u8 textY = (numLinesToDraw == 1) ? 4 : 0;

    DrawToastBox(1, 1, 28, 3);
    ShowItemIconSprite(itemToShow, TRUE, handleFlash);
    DrawToastBoxText(dst, ITEM_ICON_X + 2, textY);
}

void HideHeaderBox(bool8 hadItem, bool8 hadMsgBox)
{
    if (hadItem == TRUE)
        DestroyItemIconSprite();
    if (hadMsgBox == TRUE)
        HideToastBox();
}



// START: AUTO-RUN TOAST CODE
#define tState            data[0]
#define tSoundEffect      data[1]
#define tTextToShowIndex  data[2]
#define tToShow           data[3]

#define TOAST_TIMEOUT        100

#define AUTO_RUN_TOAST_TEXT_ENABLED  0
#define AUTO_RUN_TOAST_TEXT_DISABLED 1


static const u8* const autoRunToastText[] = {
    [AUTO_RUN_TOAST_TEXT_ENABLED] = gText_AutoRunEnabled,
    [AUTO_RUN_TOAST_TEXT_DISABLED] = gText_AutoRunDisabled,
};

static void Task_DoAutoRunToast(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (FlagGet(FLAG_EARLY_CLOSE_TOAST))
    {
        task->tState = TOAST_TIMEOUT;
        FlagClear(FLAG_EARLY_CLOSE_TOAST);
    }
    switch (task->tState)
    {
    case 0:
        StringCopy(gStringVar1, autoRunToastText[task->tTextToShowIndex]);
        DrawHeaderBox(task->tToShow, gStringVar1, EXM_FLASH_ACTIVE, 1);
        break;
    case 1:
        PlaySE(task->tSoundEffect);
        break;
    case TOAST_TIMEOUT:
        HideHeaderBox(TRUE, TRUE);
        FlagClear(FLAG_AUTO_RUN_SWAP);
        FlagClear(FLAG_HIDE_MAP_NAME_POPUP);
        DestroyTask(taskId);
        break;
    }
    task->tState++;
}

static void DrawAutoRunToast(u8 soundEffect, u8 textToShowIndex, u16 toShow)
{
    u8 taskId = CreateTask(Task_DoAutoRunToast, 1);
    struct Task* task = &gTasks[taskId];
    task->tState = 0;
    task->tSoundEffect = soundEffect;
    task->tTextToShowIndex = textToShowIndex;
    task->tToShow = toShow;
}

void DrawAutoRunBox(bool8 gettingEnabled)
{
    u16 toShow;
    u16 se;
    u8 textIndex;
    if (gettingEnabled)
    {
        toShow = ITEM_RUNNINGSHOES_RUNNING;
        se = SE_RG_HELP_OPEN;
        textIndex = AUTO_RUN_TOAST_TEXT_ENABLED;
    }
    else
    {
        toShow = ITEM_RUNNINGSHOES_WALKING;
        se = SE_RG_HELP_CLOSE;
        textIndex = AUTO_RUN_TOAST_TEXT_DISABLED;
    }
    FlagSet(FLAG_AUTO_RUN_SWAP);
    FlagSet(FLAG_HIDE_MAP_NAME_POPUP);
    DrawAutoRunToast(se, textIndex, toShow);
}

#undef tState
#undef tSoundEffect
#undef tTextToShowIndex
#undef tToShow

#undef TOAST_TIMEOUT

#undef AUTO_RUN_TOAST_TEXT_ENABLED
#undef AUTO_RUN_TOAST_TEXT_DISABLED
// END: AUTO-RUN TOAST CODE

// START: BIKE SWAP TOAST CODE
#ifdef FEATURE_SWAPBIKEBUTTON
#define tState            data[0]
#define tSoundEffect      data[1]
#define tTextToShowIndex  data[2]
#define tBikeToShow       data[3]

#define TOAST_TIMEOUT        100

#define BIKE_TOAST_TEXT_SWAP_ACRO  0
#define BIKE_TOAST_TEXT_SWAP_MACH 1
#define BIKE_TOAST_TEXT_FORCE_ACRO 2
#define BIKE_TOAST_TEXT_FORCE_MACH 3


static const u8* const bikeToastText[] = {
    [BIKE_TOAST_TEXT_SWAP_ACRO] = gText_BikeModeAcro,
    [BIKE_TOAST_TEXT_SWAP_MACH] = gText_BikeModeMach,
    [BIKE_TOAST_TEXT_FORCE_ACRO] = gText_BikeModeForcedAcro,
    [BIKE_TOAST_TEXT_FORCE_MACH] = gText_BikeModeForcedMach
};

static void Task_DoBikeToast(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (FlagGet(FLAG_EARLY_CLOSE_TOAST))
    {
        task->tState = TOAST_TIMEOUT;
        FlagClear(FLAG_EARLY_CLOSE_TOAST);
    }
    switch (task->tState)
    {
    case 0:
        StringCopy(gStringVar1, bikeToastText[task->tTextToShowIndex]);
        DrawHeaderBox(task->tBikeToShow, gStringVar1, EXM_FLASH_ACTIVE, 1);
        break;
    case 1:
        PlaySE(task->tSoundEffect);
        break;
    case TOAST_TIMEOUT:
        HideHeaderBox(TRUE, TRUE);
        FlagClear(FLAG_BIKE_MODE_SWAP);
        FlagClear(FLAG_HIDE_MAP_NAME_POPUP);
        DestroyTask(taskId);
        break;
    }
    task->tState++;
}

static void DrawBikeModeToast(u16 soundEffect, u8 textToShowIndex, u16 toShow)
{
    u8 taskId = CreateTask(Task_DoBikeToast, 1);
    struct Task* task = &gTasks[taskId];
    task->tState = 0;
    task->tSoundEffect = soundEffect;
    task->tTextToShowIndex = textToShowIndex;
    task->tBikeToShow = toShow;
}

void DrawBikeHeaderBox(bool8 forced)
{
    u16 toShow;
    u16 se;
    u8 textIndex = forced ? 2 : 0;  // Forced text starts at index 2
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_ACRO_BIKE)
    {
        toShow = ITEM_ACRO_BIKE;
        se = SE_BIKE_HOP;
    }
    else
    {
        toShow = ITEM_MACH_BIKE;
        se = SE_BIKE_BELL;
        textIndex += 1;  // Mach mode text is on the odd indices
    }
    FlagSet(FLAG_HIDE_MAP_NAME_POPUP);
    FlagSet(FLAG_BIKE_MODE_SWAP);
    DrawBikeModeToast(se, textIndex, toShow);
}
#undef tState
#undef tSoundEffect
#undef tTextToShowIndex
#undef tBikeToShow

#undef TOAST_TIMEOUT

#undef BIKE_TOAST_TEXT_SWAP_ACRO
#undef BIKE_TOAST_TEXT_SWAP_MACH
#undef BIKE_TOAST_TEXT_FORCE_ACRO
#undef BIKE_TOAST_TEXT_FORCE_MACH
#endif
// END: BIKE SWAP TOAST CODE
