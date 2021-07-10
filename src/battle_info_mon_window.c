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
#include "battle_info_utils.h"
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
#include "battle_info_mon_window.h"
#include "constants/songs.h"
#include "constants/rgb.h"

// Macro functions
#define loadPartyBoxPal(paletteIds, paletteOffsets)                                    \
{                                                                                         \
    LoadPalette(GetPalBuffer()+paletteIds[0], paletteOffsets[0] + palNum, 2);  \
    LoadPalette(GetPalBuffer()+paletteIds[1], paletteOffsets[1] + palNum, 2);  \
    LoadPalette(GetPalBuffer()+paletteIds[2], paletteOffsets[2] + palNum, 2);  \
}

#define isPaletteFlagSet(__FLAG_TO_CHECK__) (palFlags & __FLAG_TO_CHECK__)

// Macros
// For all the different palette mode flags
#define PARTY_PAL_SELECTED     (1 << 0)
#define PARTY_PAL_FAINTED      (1 << 1)
#define PARTY_PAL_TO_SWITCH    (1 << 2)
#define PARTY_PAL_MULTI_ALT    (1 << 3)
#define PARTY_PAL_SWITCHING    (1 << 4)
#define PARTY_PAL_TO_SOFTBOIL  (1 << 5)
#define PARTY_PAL_NO_MON       (1 << 6)
#define PARTY_PAL_UNUSED       (1 << 7)

// For the (x, y) of different elements in the bar
#define NICKNAME_POS_X 22
#define NICKNAME_POS_Y  1
#define GENDER_POS_X   64
#define GENDER_POS_Y   10
#define LEVEL_POS_X    32
#define LEVEL_POS_Y    10

static const struct SpritePalette sSpritePalette_HeldItem =
{
    gHeldItemPalette, 0xd750
};

static EWRAM_DATA u8* sMonWindowBgGfxTilemap = NULL;

static u16 SetupAndCreateMonIcon(u8 whichMon, u8 x, u8 y);
static void CreateBattleMonIconSpriteParameterized(u16 species, u32 pid, struct BattleInfoBox* battleInfoBox, u8 priority);
static void ShowOrHideHeldItemSprite(u16 item, struct BattleInfoBox* battleInfoBox);
static void CreateBattleMonHeldItemSpriteParameterized(u16 species, u16 item, struct BattleInfoBox* battleInfoBox);
static void CreateBattleMonPokeballSpriteParameterized(u16 species, struct BattleInfoBox* battleInfoBox);
static void UpdateBattleMonAilmentGfx(u8 status, struct BattleInfoBox* battleInfoBox);
static void SetBattleMonAilmentGfx(struct BattleInfoBox* battleInfoBox);
static void CreateBattleMonStatusSpriteParameterized(u8 status, struct BattleInfoBox* battleInfoBox);
static void LoadPartyBoxPalette(u8 windowId, u8 palFlags);
static void DisplayBattlePokemonBarDetail(u8 windowId, const u8* str, u8 color, u8 x, u8 y);
static void DisplayBattlePokemonGenderNidoranCheck(struct BattleInfoBox* battleInfoBox, u8 c);
static void DisplayBattlePokemonGender(u8 gender, u16 species, u8* nickname, struct BattleInfoBox* battleInfoBox);
static void DisplayBattlePokemonNickname(struct BattleInfoBox* battleInfoBox, u8 c);
static void DisplayBattlePokemonLevel(struct BattleInfoBox* battleInfoBox);
static void SpriteCB_BouncePartyMonIcon(struct Sprite* sprite);
static void SpriteCB_UpdatePartyMonIcon(struct Sprite* sprite);
static void AnimateSelectedPartyIcon(u8 spriteId, u8 animNum);
static void BlitMainMonWindow(struct BattleInfoBox* battleInfoBox);
static void BlitEmptyMonWindow(struct BattleInfoBox* battleInfoBox);
static u8* GetBgTile(u16 tileId);

bool8 AllocMonWindowBgGfx(u8 state, u16 size)
{
    u8 battler;
    u32 sizeout;
    switch (state)
    {
    case 0:
        sMonWindowBgGfxTilemap = malloc_and_decompress(gPartyMenuBg_Gfx, &sizeout);
        LoadBgTiles(2, sMonWindowBgGfxTilemap, sizeout, 0);
        break;
    case 1:
        // LoadCompressedPalette(gPartyMenuBg_Pal, 0, 0x160);
        // CpuCopy16(&gPlttBufferUnfaded, GetPalBuffer(), 0x160);
        // LoadCompressedPalette(gPartyMenuBg_Pal, 0, 0x90);
        // CpuCopy16(&gPlttBufferUnfaded, GetPalBuffer(), 0x90);
        LoadCompressedPalette(gPartyMenuBg_Pal, 0, size);
        CpuCopy16(&gPlttBufferUnfaded, GetPalBuffer(), size);
        break;
    }
}

void FreeMonWindowData(void)
{
    if (sMonWindowBgGfxTilemap)
        Free(sMonWindowBgGfxTilemap);
}

static u16 SetupAndCreateMonIcon(u8 whichMon, u8 x, u8 y)
{
    FreeMonIconPalettes(); //Free space for new pallete
    LoadMonIconPalette(gBattleMons[whichMon].species); //Loads pallete for current mon
    return CreateMonIcon(gBattleMons[whichMon].species, SpriteCB_MonIcon, x, y, 4, 0); //Create pokemon sprite
}

void LoadBattleInfoPokeballGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeball);
    LoadCompressedSpriteSheet(&sSpriteSheet_MenuPokeballSmall);
    LoadCompressedSpritePalette(&sSpritePalette_MenuPokeball);
}

void LoadPartyMenuAilmentGfx(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_StatusIcons);
    LoadCompressedSpritePalette(&sSpritePalette_StatusIcons);
}

static void CreateBattleMonIconSpriteParameterized(u16 species, u32 pid, struct BattleInfoBox* battleInfoBox, u8 priority)
{
    battleInfoBox->monSpriteId = CreateMonIcon(species, SpriteCB_MonIcon, battleInfoBox->spriteCoords[0], battleInfoBox->spriteCoords[1], 4, pid);
    gSprites[battleInfoBox->monSpriteId].oam.priority = priority;
}

static void ShowOrHideHeldItemSprite(u16 item, struct BattleInfoBox* battleInfoBox)
{
    if (item == ITEM_NONE)
    {
        gSprites[battleInfoBox->itemSpriteId].invisible = TRUE;
    }
    else
    {
        if (ItemIsMail(item))
            StartSpriteAnim(&gSprites[battleInfoBox->itemSpriteId], 1);
        else
            StartSpriteAnim(&gSprites[battleInfoBox->itemSpriteId], 0);
        gSprites[battleInfoBox->itemSpriteId].invisible = FALSE;
    }
}

static void CreateBattleMonHeldItemSpriteParameterized(u16 species, u16 item, struct BattleInfoBox* battleInfoBox)
{
    battleInfoBox->itemSpriteId = CreateSprite(&sSpriteTemplate_HeldItem, battleInfoBox->spriteCoords[2], battleInfoBox->spriteCoords[3], 0);
    gSprites[battleInfoBox->itemSpriteId].oam.priority = 0;
    ShowOrHideHeldItemSprite(item, battleInfoBox);
}

static void CreateBattleMonPokeballSpriteParameterized(u16 species, struct BattleInfoBox* battleInfoBox)
{
    battleInfoBox->pokeballSpriteId = CreateSprite(&sSpriteTemplate_MenuPokeball, battleInfoBox->spriteCoords[6], battleInfoBox->spriteCoords[7], 8);
    gSprites[battleInfoBox->pokeballSpriteId].oam.priority = 0;
}

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

static void SetBattleMonAilmentGfx(struct BattleInfoBox* battleInfoBox)
{
    UpdateBattleMonAilmentGfx(GetMonAilment(battleInfoBox->mon), battleInfoBox);
}

static void CreateBattleMonStatusSpriteParameterized(u8 status, struct BattleInfoBox* battleInfoBox)
{
    battleInfoBox->statusSpriteId = CreateSprite(&sSpriteTemplate_StatusIcons, battleInfoBox->spriteCoords[4], battleInfoBox->spriteCoords[5], 0);
    UpdateBattleMonAilmentGfx(status, battleInfoBox);
    gSprites[battleInfoBox->statusSpriteId].oam.priority = 0;
}

void CreateBattleMonSprites(struct BattleInfoBox* battleInfoBox)
{

    struct Pokemon curPokemon = *battleInfoBox->mon;
    struct BattlePokemon curBattlePokemon = *battleInfoBox->battleMon;
    u8 status;

    if (curBattlePokemon.species != SPECIES_NONE)
    {
        CreateBattleMonIconSpriteParameterized(curBattlePokemon.species, curBattlePokemon.personality, battleInfoBox, 0);
        CreateBattleMonHeldItemSpriteParameterized(curBattlePokemon.species, curBattlePokemon.item, battleInfoBox);
        CreateBattleMonPokeballSpriteParameterized(curBattlePokemon.species, battleInfoBox);
        if (curBattlePokemon.hp == 0)
            status = AILMENT_FNT;
        else
            status = GetAilmentFromStatus(curBattlePokemon.status1);
        CreateBattleMonStatusSpriteParameterized(status, battleInfoBox);
    }
}

void PartyPaletteBufferCopy(u8 offset)
{
    offset *= 16;
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferUnfaded[offset], 32);
    CpuCopy16(&gPlttBufferUnfaded[0x30], &gPlttBufferFaded[offset], 32);
}

static void LoadPartyBoxPalette(u8 windowId, u8 palFlags)
{
    u8 palNum = GetWindowAttribute(windowId, WINDOW_PALETTE_NUM) * 16;

    if (isPaletteFlagSet(PARTY_PAL_NO_MON))
    {
        loadPartyBoxPal(sPartyBoxNoMonPalIds, sPartyBoxNoMonPalOffsets);
    }
    else if (isPaletteFlagSet(PARTY_PAL_TO_SOFTBOIL))
    {
        if (isPaletteFlagSet(PARTY_PAL_SELECTED))
        {
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (isPaletteFlagSet(PARTY_PAL_SWITCHING))
    {
        loadPartyBoxPal(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
        loadPartyBoxPal(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
    }
    else if (isPaletteFlagSet(PARTY_PAL_TO_SWITCH))
    {
        if (isPaletteFlagSet(PARTY_PAL_SELECTED))
        {
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxSelectedForActionPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (isPaletteFlagSet(PARTY_PAL_FAINTED))
    {
        if (isPaletteFlagSet(PARTY_PAL_SELECTED))
        {
            loadPartyBoxPal(sPartyBoxCurrSelectionFaintedPalIds, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            loadPartyBoxPal(sPartyBoxFaintedPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxFaintedPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (isPaletteFlagSet(PARTY_PAL_MULTI_ALT))
    {
        if (isPaletteFlagSet(PARTY_PAL_SELECTED))
        {
            loadPartyBoxPal(sPartyBoxCurrSelectionMultiPalIds, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
        }
        else
        {
            loadPartyBoxPal(sPartyBoxMultiPalIds1, sPartyBoxPalOffsets1);
            loadPartyBoxPal(sPartyBoxMultiPalIds2, sPartyBoxPalOffsets2);
        }
    }
    else if (isPaletteFlagSet(PARTY_PAL_SELECTED))
    {
        loadPartyBoxPal(sPartyBoxCurrSelectionPalIds1, sPartyBoxPalOffsets1);
        loadPartyBoxPal(sPartyBoxCurrSelectionPalIds2, sPartyBoxPalOffsets2);
    }
    else
    {
        loadPartyBoxPal(sPartyBoxEmptySlotPalIds1, sPartyBoxPalOffsets1);
        loadPartyBoxPal(sPartyBoxEmptySlotPalIds2, sPartyBoxPalOffsets2);
    }
}

void RenderMonWindow(struct BattleInfoBox* battleInfoBox, bool8 isSelected)
{
    LoadPartyBoxPalette(battleInfoBox->windowId, isSelected ? PARTY_PAL_SELECTED : PARTY_PAL_UNUSED);
    BlitMainMonWindow(battleInfoBox);
    DisplayBattlePokemonNickname(battleInfoBox, 0);
    DisplayBattlePokemonGenderNidoranCheck(battleInfoBox, 0);
    if (!(battleInfoBox->battleMon->status1 || CheckPartyPokerus(battleInfoBox->mon, 0)))
        DisplayBattlePokemonLevel(battleInfoBox);
    AnimateBattleInfoSlot(isSelected, battleInfoBox);

    PutWindowTilemap(battleInfoBox->windowId);
    CopyWindowToVram(battleInfoBox->windowId, 2);

}

static void DisplayBattlePokemonBarDetail(u8 windowId, const u8* str, u8 color, u8 x, u8 y)
{
    AddTextPrinterParameterized3(windowId, 0, x, y, GetFontColor(color), 0, str);
}

static void DisplayBattlePokemonGenderNidoranCheck(struct BattleInfoBox* battleInfoBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    // if (c == 1)
        // battleInfoBox->infoRects->blitFunc(battleInfoBox->windowId, battleInfoBox->infoRects->dimensions[8] >> 3, (battleInfoBox->infoRects->dimensions[9] >> 3) + 1, battleInfoBox->infoRects->dimensions[10] >> 3, battleInfoBox->infoRects->dimensions[11] >> 3, FALSE);
    GetMonNickname(battleInfoBox->mon, nickname);
    DisplayBattlePokemonGender(GetMonGender(battleInfoBox->mon), GetMonData(battleInfoBox->mon, MON_DATA_SPECIES), nickname, battleInfoBox);
}

static void DisplayBattlePokemonGender(u8 gender, u16 species, u8* nickname, struct BattleInfoBox* battleInfoBox)
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
    LoadPalette(&GetPalBuffer()[genderPalToUse[0]], sGenderPalOffsets[0] + palNum, 2);
    LoadPalette(&GetPalBuffer()[genderPalToUse[1]], sGenderPalOffsets[1] + palNum, 2);
    DisplayBattlePokemonBarDetail(battleInfoBox->windowId, symbolToUse, BATTLE_MON_TEXT_COLOR_GENDER_SYMBOL, GENDER_POS_X, GENDER_POS_Y);
}

static void DisplayBattlePokemonNickname(struct BattleInfoBox* battleInfoBox, u8 c)
{
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    if (GetMonData(battleInfoBox->mon, MON_DATA_SPECIES) != SPECIES_NONE)
    {
        // if (c == 1)
        //     battleInfoBox->infoRects->blitFunc(battleInfoBox->windowId, battleInfoBox->infoRects->dimensions[0] >> 3, battleInfoBox->infoRects->dimensions[1] >> 3, battleInfoBox->infoRects->dimensions[2] >> 3, battleInfoBox->infoRects->dimensions[3] >> 3, FALSE);
        GetMonNickname(battleInfoBox->mon, nickname);
        DisplayBattlePokemonBarDetail(battleInfoBox->windowId, nickname, BATTLE_MON_TEXT_COLOR_DEFAULT, NICKNAME_POS_X, NICKNAME_POS_Y);
    }
}

static void DisplayBattlePokemonLevel(struct BattleInfoBox* battleInfoBox)
{
    ConvertIntToDecimalStringN(gStringVar2, battleInfoBox->mon->level, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringCopy(gStringVar1, gText_LevelSymbol);
    StringAppend(gStringVar1, gStringVar2);
    DisplayBattlePokemonBarDetail(battleInfoBox->windowId, gStringVar1, 0, LEVEL_POS_X, LEVEL_POS_Y);
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

void AnimateBattleInfoSlot(u8 animNum, struct BattleInfoBox* battleInfoBox)
{
    u8 palFlags = 0;
    if (GetMonData(battleInfoBox->mon, MON_DATA_SPECIES) != SPECIES_NONE)
    {
        if (animNum == 1)
            palFlags |= PARTY_PAL_SELECTED;
        if (GetMonData(battleInfoBox->mon, MON_DATA_HP) == 0)
            palFlags |= PARTY_PAL_FAINTED;

        LoadPartyBoxPalette(battleInfoBox->windowId, palFlags);
        AnimateSelectedPartyIcon(battleInfoBox->monSpriteId, animNum);
        StartSpriteAnim(&gSprites[battleInfoBox->pokeballSpriteId], animNum);
    }
}

static void BlitMainMonWindow(struct BattleInfoBox* battleInfoBox)
{
    BlitBitmapToMonWindow(sMonWindowBgGfxTilemap, battleInfoBox->windowId, sMainSlotTileNums, 10, battleInfoBox->windowWidth, battleInfoBox->windowHeight);
}

static void BlitEmptyMonWindow(struct BattleInfoBox* battleInfoBox)
{
    BlitBitmapToMonWindow(sMonWindowBgGfxTilemap, battleInfoBox->windowId, sEmptySlotTileNums, 10, battleInfoBox->windowWidth, battleInfoBox->windowHeight);
}


#undef PARTY_PAL_SELECTED
#undef PARTY_PAL_FAINTED
#undef PARTY_PAL_TO_SWITCH
#undef PARTY_PAL_MULTI_ALT
#undef PARTY_PAL_SWITCHING
#undef PARTY_PAL_TO_SOFTBOIL
#undef PARTY_PAL_NO_MON
#undef PARTY_PAL_UNUSED

#undef NICKNAME_POS_X
#undef NICKNAME_POS_Y
#undef GENDER_POS_X
#undef GENDER_POS_Y

#undef MON_WINDOW_HEIGHT
#undef MON_WINDOW_WIDTH