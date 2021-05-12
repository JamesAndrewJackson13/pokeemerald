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
