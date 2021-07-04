#ifndef GUARD_BATTLE_INFO_UTILS_H
#define GUARD_BATTLE_INFO_UTILS_H

#include "global.h"

// For the different text colors that can be used
#define BATTLE_MON_TEXT_COLOR_DEFAULT           0
#define BATTLE_MON_TEXT_COLOR_UNUSED_1          1
#define BATTLE_MON_TEXT_COLOR_GENDER_SYMBOL     2
#define BATTLE_MON_TEXT_COLOR_SELECTION_ACTIONS 3
#define BATTLE_MON_TEXT_COLOR_FIELD_MOVES       4
#define BATTLE_MON_TEXT_COLOR_UNUSED_2          5
#define BATTLE_MON_TEXT_COLOR_HEADER            6

u8* GetFontColor(u8 which);
struct Pokemon* GetPokemonAtSlot(u8 slot);
u16* GetPalBuffer(void);
u8** GetTerrainEffectDescriptions(u8 monIndex);

#endif //GUARD_BATTLE_INFO_UTILS_H