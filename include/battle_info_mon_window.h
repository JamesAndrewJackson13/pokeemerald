#ifndef GUARD_BATTLE_MON_WINDOW_H
#define GUARD_BATTLE_MON_WINDOW_H

#include "global.h"

// For the different status ailment icons that can be shown
#define AILMENT_NONE  0
#define AILMENT_PSN   1
#define AILMENT_PRZ   2
#define AILMENT_SLP   3
#define AILMENT_FRZ   4
#define AILMENT_BRN   5
#define AILMENT_PKRS  6
#define AILMENT_FNT   7

// Tile nums
static const u8 sMainSlotTileNums[] = { 43, 44, 45,
                                        49, 33, 50,
                                        55, 56, 57 };

static const u8 sEmptySlotTileNums[] = { 21, 22, 23,
                                         30,  0, 31,
                                         37, 38, 39 };

struct BattleInfoBox
{
    const u8* spriteCoords;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
    u8 pallet;
    u8 windowWidth;
    u8 windowHeight;
    struct Pokemon* mon;
    struct BattlePokemon* battleMon;
    u8* bgGfxPtr;
};

void RenderMonWindow(struct BattleInfoBox* battleInfoBox, bool8 isSelected);
void CreateBattleMonSprites(struct BattleInfoBox* battleInfoBox);
void LoadPartyMenuAilmentGfx(void);
void LoadBattleInfoPokeballGfx(void);
void AnimateBattleInfoSlot(u8 animNum, struct BattleInfoBox* battleInfoBox);
void PartyPaletteBufferCopy(u8 offset);
bool8 AllocMonWindowBgGfx(u8 state, u16 size);
void FreeMonWindowData(void);



#endif //GUARD_BATTLE_MON_WINDOW_H