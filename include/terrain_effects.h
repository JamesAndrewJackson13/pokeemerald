#ifndef TERRAIN_EFFECTS_H
#define TERRAIN_EFFECTS_H

#include "global.h"

#define TERRAIN_EFFECT_HARSH_SUNLIGHT            0
#define TERRAIN_EFFECT_RAIN                      1
#define TERRAIN_EFFECT_SANDSTORM                 2
#define TERRAIN_EFFECT_HAIL                      3
#define TERRAIN_EFFECT_ELECTRIC_TERRAIN          4
#define TERRAIN_EFFECT_GRASSY_TERRAIN            5
#define TERRAIN_EFFECT_MISTY_TERRAIN             6
#define TERRAIN_EFFECT_TRICK_ROOM                7
#define TERRAIN_EFFECT_MAGIC_ROOM                8
#define TERRAIN_EFFECT_WONDER_ROOM               9
#define TERRAIN_EFFECT_CRITICAL_HIT_BOOST       10
#define TERRAIN_EFFECT_CONFUSION                11
#define TERRAIN_EFFECT_INFATUATION              12
#define TERRAIN_EFFECT_NIGHTMARE                13
#define TERRAIN_EFFECT_DROWSY                   14
#define TERRAIN_EFFECT_ENCORE                   15
#define TERRAIN_EFFECT_NO_ABILITY               16
#define TERRAIN_EFFECT_TORMENT                  17
#define TERRAIN_EFFECT_TAILWIND                 18
#define TERRAIN_EFFECT_LUCKY_CHANT              19
#define TERRAIN_EFFECT_GRUDGE                   20
#define TERRAIN_EFFECT_HEAL_BLOCK               21
#define TERRAIN_EFFECT_IDENTIFIED               22
#define TERRAIN_EFFECT_MOVE_DISABLED            23
#define TERRAIN_EFFECT_CANT_ESCAPE              24
#define TERRAIN_EFFECT_LOCK_ON                  25
#define TERRAIN_EFFECT_EMBARGO                  26
#define TERRAIN_EFFECT_CHARGE                   27
#define TERRAIN_EFFECT_GRAVITY                  28
#define TERRAIN_EFFECT_MIST                     29
#define TERRAIN_EFFECT_SAFEGUARD                30
#define TERRAIN_EFFECT_STEALTH_ROCK             31
#define TERRAIN_EFFECT_STOCKPILE                32
#define TERRAIN_EFFECT_TAUNT                    33
#define TERRAIN_EFFECT_TELEKINESIS              34
#define TERRAIN_EFFECT_MAGNET_RISE              35
#define TERRAIN_EFFECT_TOXIC_SPIKES             36
#define TERRAIN_EFFECT_MUDSPORT                 37
#define TERRAIN_EFFECT_WISH                     38
#define TERRAIN_EFFECT_STICKY_WEB               39
#define TERRAIN_EFFECT_INGRAIN                  40
#define TERRAIN_EFFECT_CURSE                    41
#define TERRAIN_EFFECT_TRICK_OR_TREAT           42
#define TERRAIN_EFFECT_LIGHTSCREEN              43
#define TERRAIN_EFFECT_REFLECT                  44
#define TERRAIN_EFFECT_IMPRISON                 45
#define TERRAIN_EFFECT_COUNTING_DOWN            46
#define TERRAIN_EFFECT_SPIKES                   47
#define TERRAIN_EFFECT_WATERSPORT               48
#define TERRAIN_EFFECT_DESTINY_BOND             49
#define TERRAIN_EFFECT_FORESTS_CURSE            50
#define TERRAIN_EFFECT_LEECHSEED                51
#define TERRAIN_EFFECT_LOCKED_ON                52
#define TERRAIN_EFFECT_BOUND                    53
#define TERRAIN_EFFECT_BIDE                     54
#define TERRAIN_EFFECT_OUTRAGE                  55
#define TERRAIN_EFFECT_RAMPAGING                56
#define TERRAIN_EFFECT_PETAL_DANCE              57
#define TERRAIN_EFFECT_BADLY_POISONED           58
#define TERRAIN_EFFECT_FUTUREATTACK             59
#define TERRAIN_EFFECT_UPROAR                   60
#define TERRAIN_EFFECT_AQUA_RING                61
#define TERRAIN_EFFECT_AUTOTOMIZE               62
#define TERRAIN_EFFECT_SMACK_DOWN               63
#define TERRAIN_EFFECT_FAIRY_LOCK               64
#define TERRAIN_EFFECT_RAINBOW                  65
#define TERRAIN_EFFECT_SWAMP                    66
#define TERRAIN_EFFECT_SEAOF_FIRE               67
#define TERRAIN_EFFECT_EXTREMELY_HARSH_SUNLIGHT 68
#define TERRAIN_EFFECT_HEAVY_RAIN               69
#define TERRAIN_EFFECT_STRONG_WINDS             70
#define TERRAIN_EFFECT_PSYCHIC_TERRAIN          71
#define TERRAIN_EFFECT_THROAT_CHOP              72
#define TERRAIN_EFFECT_AURORA_VEIL              73
#define TERRAIN_EFFECT_LASER_FOCUS              74
#define TERRAIN_EFFECT_TAR_SHOT                 75
#define TERRAIN_EFFECT_OCTOLOCK                 76
#define TERRAIN_EFFECT_G_MAX_WILDFIRE           77
#define TERRAIN_EFFECT_G_MAX_VOLCALITH          78
#define TERRAIN_EFFECT_G_MAX_STEELSURGE         79
#define TERRAIN_EFFECT_G_MAX_VINE_LASH          80
#define TERRAIN_EFFECT_G_MAX_CANNONADE          81
#define TERRAIN_EFFECT_NULL                    255
#define NUM_TERRAIN_EFFECT_DESCRIPTIONS         82

struct terrain_effect
{
    const u8* title;
    const u8* description;
};

void GetTerrainEffectDescriptions(u8 monIndex, u8* curTerrainEffects);
struct terrain_effect GetTerrainEffect(u8 terrainId);

#endif  // TERRAIN_EFFECTS_H