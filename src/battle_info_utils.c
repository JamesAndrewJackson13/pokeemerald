#include "global.h"
#include "text.h"
#include "battle.h"
#include "battle_info_utils.h"
#include "battle_move_effects.h"
#include "terrain_effects.h"
#include "constants/moves.h"

#define TEXT_COLOR_BG     0
#define TEXT_COLOR_FG     1
#define TEXT_COLOR_SHADOW 2

#define BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT           \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_LIGHT_GRAY,      \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_DARK_GRAY,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1          \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_GREEN,           \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL     \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_DYNAMIC_COLOR_2,       \
    [TEXT_COLOR_SHADOW] = TEXT_DYNAMIC_COLOR_3,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_FG]     = TEXT_COLOR_DARK_GRAY,       \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_GRAY,      \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES       \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_FG]     = TEXT_COLOR_BLUE,            \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_BLUE,      \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2          \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_WHITE,           \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_DARK_GRAY,       \
}

#define BATTLE_MON_TEXT_COLOR_ARRAY_HEADER            \
{                                                     \
    [TEXT_COLOR_BG]     = TEXT_COLOR_TRANSPARENT,     \
    [TEXT_COLOR_FG]     = TEXT_COLOR_DARK_GRAY,       \
    [TEXT_COLOR_SHADOW] = TEXT_COLOR_LIGHT_GRAY,      \
}

// Text colors for BG, FG, and Shadow in that order
static const u8 sFontColorTable[][3] =
{
    BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT,
    BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1,
    BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL,
    BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS,
    BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES,
    BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2,
    BATTLE_MON_TEXT_COLOR_ARRAY_HEADER,
};

u8* GetFontColor(u8 which)
{
    return sFontColorTable[which];
}

struct Pokemon* GetPokemonAtSlot(u8 slot)
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


static const u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];
u16* GetPalBuffer(void)
{
    return palBuffer;
}


#define howManyTerrainEffects 4
#define setTerrainEffect(__WHICH_EFFECT__) \
    curTerrainEffects[curIndex] = gTerrainEffects[__WHICH_EFFECT__]; \
    if(++curIndex == howManyTerrainEffects) \
        return curTerrainEffects; \

#define handleFieldStatus(__WHICH__) \
if (gFieldStatuses & STATUS_FIELD_##__WHICH__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleMonStatus1(__WHICH__, __STATUS__) \
if (gBattleMons[monIndex].status1 & STATUS1_##__STATUS__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleMonStatus2(__WHICH__, __STATUS__) \
if (gBattleMons[monIndex].status2 & STATUS2_##__STATUS__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleMonStatus3(__WHICH__, __STATUS__) \
if (gStatuses3[monIndex] & STATUS3_##__STATUS__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleDisableStruct(__WHICH__, __STRUCT_ATTR__) \
if (gDisableStructs[monIndex].##__STRUCT_ATTR__## != 0) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleSideStatus(__WHICH__) \
if (sideStatus & SIDE_STATUS_##__WHICH__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

#define handleSideTimers(__WHICH__, __TIMER__) \
if (gSideTimers[sideStatus].##__TIMER__) \
{ \
    setTerrainEffect(DESCRIPTION_##__WHICH__); \
} \

u8** GetTerrainEffectDescriptions(u8 monIndex)
{
    u8 curIndex = 0;
    struct terrain_effect curTerrainEffects[howManyTerrainEffects] = { NULL, NULL, NULL, NULL };
    u32 sideStatus = gSideStatuses[GET_BATTLER_SIDE(monIndex)];

    // First, check the weather states
    if (gBattleWeather & WEATHER_ANY)
    {
        if (gBattleWeather & WEATHER_SUN_ANY)
        {
            curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_HARSH_SUNLIGHT];
        }
        else if (gBattleWeather & WEATHER_RAIN_ANY)
        {
            curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_RAIN];
        }
        else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
        {
            curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_SANDSTORM];
        }
        else if (gBattleWeather & WEATHER_HAIL_ANY)
        {
            curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_HAIL];
        }
        ++curIndex;
    }
    // Next, check if there's any flags set in gFieldStatuses
    if (gFieldStatuses)
    {
        if (gFieldStatuses & STATUS_TERRAIN_ANY)
        {
            if (gFieldStatuses & STATUS_FIELD_GRASSY_TERRAIN)
            {
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_GRASSY_TERRAIN];
            }
            else if (gFieldStatuses & STATUS_FIELD_MISTY_TERRAIN)
            {
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_ELECTRIC_TERRAIN];
            }
            else if (gFieldStatuses & EFFECT_ELECTRIC_TERRAIN)
            {
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_MISTY_TERRAIN];
            }
            else if (gFieldStatuses & EFFECT_PSYCHIC_TERRAIN)
            {
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_PSYCHIC_TERRAIN];
            }
            ++curIndex;
        }
        handleFieldStatus(MAGIC_ROOM);
        handleFieldStatus(TRICK_ROOM);
        handleFieldStatus(WONDER_ROOM);
        handleFieldStatus(MUDSPORT);
        handleFieldStatus(WATERSPORT);
        handleFieldStatus(GRAVITY);
        handleFieldStatus(FAIRY_LOCK);
    }

    handleMonStatus1(BADLY_POISONED, TOXIC_POISON);

    if (gBattleMons[monIndex].status2)
    {
        // For whatever reason, there's a different description for each type of rampage. So we need to check for the locked move
        if (gBattleMons[gEffectBattler].status2 & STATUS2_MULTIPLETURNS && gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE)
        {
            switch (gLockedMoves[gEffectBattler])
            {
            case MOVE_OUTRAGE:
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_OUTRAGE];
                break;
            case MOVE_THRASH:
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_RAMPAGING];
                break;
            case MOVE_PETAL_DANCE:
                curTerrainEffects[curIndex] = gTerrainEffects[DESCRIPTION_PETAL_DANCE];
                break;
            }
            if (++curIndex == howManyTerrainEffects)
                return curTerrainEffects;
        }
        handleMonStatus2(CRITICAL_HIT_BOOST, FOCUS_ENERGY);
        handleMonStatus2(IDENTIFIED, FORESIGHT);
        handleMonStatus2(CANT_ESCAPE, ESCAPE_PREVENTION);
        handleMonStatus2(CONFUSION, CONFUSION);
        handleMonStatus2(INFATUATION, INFATUATION);
        handleMonStatus2(NIGHTMARE, NIGHTMARE);
        handleMonStatus2(TORMENT, TORMENT);
        handleMonStatus2(CURSE, CURSED);
        handleMonStatus2(DESTINY_BOND, DESTINY_BOND);
        handleMonStatus2(BOUND, WRAPPED);
        handleMonStatus2(BIDE, BIDE);
        handleMonStatus2(UPROAR, UPROAR);
    }

    if (gStatuses3[monIndex])
    {
        handleMonStatus3(DROWSY, YAWN);
        handleMonStatus3(NO_ABILITY, GASTRO_ACID);
        handleMonStatus3(LOCK_ON, ALWAYS_HITS);
        handleMonStatus3(CHARGE, CHARGED_UP);
        handleMonStatus3(INGRAIN, ROOTED);
        handleMonStatus3(GRUDGE, GRUDGE);
        handleMonStatus3(HEAL_BLOCK, HEAL_BLOCK);
        handleMonStatus3(EMBARGO, EMBARGO);
        handleMonStatus3(TELEKINESIS, TELEKINESIS);
        handleMonStatus3(MAGNET_RISE, MAGNET_RISE);
        handleMonStatus3(IMPRISON, IMPRISONED_OTHERS);
        handleMonStatus3(LEECHSEED, LEECHSEED);
        handleMonStatus3(AQUA_RING, AQUA_RING);
        handleMonStatus3(SMACK_DOWN, SMACKED_DOWN);
        handleMonStatus3(LASER_FOCUS, LASER_FOCUS);
    }

    if (sideStatus)
    {
        handleSideStatus(TAILWIND);
        handleSideStatus(LUCKY_CHANT);
        handleSideStatus(SAFEGUARD);
        handleSideStatus(STEALTH_ROCK);
        handleSideStatus(TOXIC_SPIKES);
        handleSideStatus(STICKY_WEB);
        handleSideStatus(LIGHTSCREEN);
        handleSideStatus(REFLECT);
        handleSideStatus(SPIKES);
        handleSideStatus(FUTUREATTACK);
        handleSideStatus(AURORA_VEIL);
    }

    handleSideTimers(MIST, mistTimer);

    handleDisableStruct(ENCORE, encoredMove);
    handleDisableStruct(MOVE_DISABLED, disabledMove);
    handleDisableStruct(STOCKPILE, stockpileCounter);
    handleDisableStruct(TAUNT, tauntTimer);
    handleDisableStruct(COUNTING_DOWN, perishSongTimer);
    handleDisableStruct(AUTOTOMIZE, autotomizeCount);
    handleDisableStruct(THROAT_CHOP, throatChopTimer);

    if (gWishFutureKnock.wishCounter[gBattlerAttacker] != 0)
    {
        setTerrainEffect(DESCRIPTION_WISH);
    }


    return curTerrainEffects;
}
#undef setTerrainEffect

#undef BATTLE_MON_TEXT_COLOR_ARRAY_DEFAULT
#undef BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_1
#undef BATTLE_MON_TEXT_COLOR_ARRAY_GENDER_SYMBOL
#undef BATTLE_MON_TEXT_COLOR_ARRAY_SELECTION_ACTIONS
#undef BATTLE_MON_TEXT_COLOR_ARRAY_FIELD_MOVES
#undef BATTLE_MON_TEXT_COLOR_ARRAY_UNUSED_2
#undef BATTLE_MON_TEXT_COLOR_ARRAY_HEADER

#undef TEXT_COLOR_BG
#undef TEXT_COLOR_FG
#undef TEXT_COLOR_SHADOW