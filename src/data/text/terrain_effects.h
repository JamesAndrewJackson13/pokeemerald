#ifndef TERRAIN_EFFECTS_H
#define TERRAIN_EFFECTS_H

#include "global.h"

#define DESCRIPTION_HARSH_SUNLIGHT   0
#define DESCRIPTION_RAIN             1
#define DESCRIPTION_SANDSTORM        2
#define DESCRIPTION_HAIL             3
#define DESCRIPTION_ELECTRIC_TERRAIN 4


#define DESCRIPTION_HARSH_SUNLIGHT            0
#define DESCRIPTION_RAIN                      1
#define DESCRIPTION_SANDSTORM                 2
#define DESCRIPTION_HAIL                      3
#define DESCRIPTION_ELECTRIC_TERRAIN          4
#define DESCRIPTION_GRASSY_TERRAIN            5
#define DESCRIPTION_MISTY_TERRAIN             6
#define DESCRIPTION_TRICK_ROOM                7
#define DESCRIPTION_MAGIC_ROOM                8
#define DESCRIPTION_WONDER_ROOM               9
#define DESCRIPTION_CRITICAL_HIT_BOOST       10
#define DESCRIPTION_CONFUSION                11
#define DESCRIPTION_INFATUATION              12
#define DESCRIPTION_NIGHTMARE                13
#define DESCRIPTION_DROWSY                   14
#define DESCRIPTION_ENCORE                   15
#define DESCRIPTION_NO_ABILITY               16
#define DESCRIPTION_TORMENT                  17
#define DESCRIPTION_TAILWIND                 18
#define DESCRIPTION_LUCKY_CHANT              19
#define DESCRIPTION_GRUDGE                   20
#define DESCRIPTION_HEAL_BLOCK               21
#define DESCRIPTION_IDENTIFIED               22
#define DESCRIPTION_MOVE_DISABLED            23
#define DESCRIPTION_CANT_ESCAPE              24
#define DESCRIPTION_LOCK_ON                  25
#define DESCRIPTION_EMBARGO                  26
#define DESCRIPTION_CHARGE                   27
#define DESCRIPTION_GRAVITY                  28
#define DESCRIPTION_MIST                     29
#define DESCRIPTION_SAFEGUARD                30
#define DESCRIPTION_STEALTH_ROCK             31
#define DESCRIPTION_STOCKPILE                32
#define DESCRIPTION_TAUNT                    33
#define DESCRIPTION_TELEKINESIS              34
#define DESCRIPTION_MAGNET_RISE              35
#define DESCRIPTION_TOXIC_SPIKES             36
#define DESCRIPTION_MUDSPORT                 37
#define DESCRIPTION_WISH                     38
#define DESCRIPTION_STICKY_WEB               39
#define DESCRIPTION_INGRAIN                  40
#define DESCRIPTION_CURSE                    41
#define DESCRIPTION_TRICK_OR_TREAT           42
#define DESCRIPTION_LIGHTSCREEN              43
#define DESCRIPTION_REFLECT                  44
#define DESCRIPTION_IMPRISON                 45
#define DESCRIPTION_COUNTING_DOWN            46
#define DESCRIPTION_SPIKES                   47
#define DESCRIPTION_WATERSPORT               48
#define DESCRIPTION_DESTINY_BOND             49
#define DESCRIPTION_FORESTS_CURSE            50
#define DESCRIPTION_LEECHSEED                51
#define DESCRIPTION_LOCK_ON                  52
#define DESCRIPTION_BOUND                    53
#define DESCRIPTION_BIDE                     54
#define DESCRIPTION_OUTRAGE                  55
#define DESCRIPTION_RAMPAGING                56
#define DESCRIPTION_PETAL_DANCE              57
#define DESCRIPTION_BADLY_POISONED           58
#define DESCRIPTION_FUTUREATTACK             59
#define DESCRIPTION_UPROAR                   60
#define DESCRIPTION_AQUA_RING                61
#define DESCRIPTION_AUTOTOMIZE               62
#define DESCRIPTION_SMACK_DOWN               63
#define DESCRIPTION_FAIRY_LOCK               64
#define DESCRIPTION_RAINBOW                  65
#define DESCRIPTION_SWAMP                    66
#define DESCRIPTION_SEAOF_FIRE               67
#define DESCRIPTION_EXTREMELY_HARSH_SUNLIGHT 68
#define DESCRIPTION_HEAVY_RAIN               69
#define DESCRIPTION_STRONG_WINDS             70
#define DESCRIPTION_PSYCHIC_TERRAIN          71
#define DESCRIPTION_THROAT_CHOP              72
#define DESCRIPTION_AURORA_VEIL              73
#define DESCRIPTION_LASER_FOCUS              74
#define DESCRIPTION_TAR_SHOT                 75
#define DESCRIPTION_OCTOLOCK                 76
#define DESCRIPTION_G_MAX_WILDFIRE           77
#define DESCRIPTION_G_MAX_VOLCALITH          78
#define DESCRIPTION_G_MAX_STEELSURGE         79
#define DESCRIPTION_G_MAX_VINE_LASH          80
#define DESCRIPTION_G_MAX_CANNONADE          81
#define NUM_TERRAIN_EFFECT_DESCRIPTIONS      82

struct terrain_effect
{
    u8* title;
    u8* description;
};


// Titles
static const u8 title_harshSunlight[] = _("Harsh Sunlight");
static const u8 title_rain[] = _("Rain");
static const u8 title_sandstorm[] = _("Sandstorm");
static const u8 title_hail[] = _("Hail");
static const u8 title_electricTerrain[] = _("Electric Terrain");
static const u8 title_grassyTerrain[] = _("Grassy Terrain");
static const u8 title_mistyTerrain[] = _("Misty Terrain");
static const u8 title_trickRoom[] = _("Trick Room");
static const u8 title_magicRoom[] = _("Magic Room");
static const u8 title_wonderRoom[] = _("Wonder Room");
static const u8 title_criticalHitBoost[] = _("Critical Hit Boost");
static const u8 title_confusion[] = _("Confusion");
static const u8 title_infatuation[] = _("Infatuation");
static const u8 title_nightmare[] = _("Nightmare");
static const u8 title_drowsy[] = _("Drowsy");
static const u8 title_encore[] = _("Encore");
static const u8 title_noAbility[] = _("No Ability");
static const u8 title_torment[] = _("Torment");
static const u8 title_tailwind[] = _("Tailwind");
static const u8 title_luckyChant[] = _("Lucky Chant");
static const u8 title_grudge[] = _("Grudge");
static const u8 title_healBlock[] = _("Heal Block");
static const u8 title_identified[] = _("Identified");
static const u8 title_moveDisabled[] = _("Move Disabled");
static const u8 title_cantEscape[] = _("Can’t Escape");
static const u8 title_lockOn[] = _("Lock-On");
static const u8 title_embargo[] = _("Embargo");
static const u8 title_charge[] = _("Charge");
static const u8 title_gravity[] = _("Gravity");
static const u8 title_mist[] = _("Mist");
static const u8 title_safeguard[] = _("Safeguard");
static const u8 title_stealthRock[] = _("Stealth Rock");
static const u8 title_stockpile[] = _("Stockpile");
static const u8 title_taunt[] = _("Taunt");
static const u8 title_telekinesis[] = _("Telekinesis");
static const u8 title_magnetRise[] = _("Magnet Rise");
static const u8 title_toxicSpikes[] = _("Toxic Spikes");
static const u8 title_mudsport[] = _("Mud Sport");
static const u8 title_wish[] = _("Wish");
static const u8 title_stickyWeb[] = _("Sticky Web");
static const u8 title_ingrain[] = _("Ingrain");
static const u8 title_curse[] = _("Curse");
static const u8 title_trickOrTreat[] = _("Trick-or-Treat");
static const u8 title_lightscreen[] = _("Light Screen");
static const u8 title_reflect[] = _("Reflect");
static const u8 title_imprison[] = _("Imprison");
static const u8 title_countingDown[] = _("Counting Down");
static const u8 title_spikes[] = _("Spikes");
static const u8 title_watersport[] = _("Water Sport");
static const u8 title_destinyBond[] = _("Destiny Bond");
static const u8 title_forestsCurse[] = _("Forest’s Curse");
static const u8 title_leechseed[] = _("Leech Seed");
// static const u8 title_lockOn[] = _("Lock-On");
static const u8 title_bound[] = _("Bound");
static const u8 title_bide[] = _("Bide");
static const u8 title_outrage[] = _("Outrage");
static const u8 title_rampaging[] = _("Rampaging");
static const u8 title_petalDance[] = _("Petal Dance");
static const u8 title_badlyPoisoned[] = _("Badly Poisoned");
static const u8 title_futureattack[] = _("Future Attack");
static const u8 title_uproar[] = _("Uproar");
static const u8 title_aquaRing[] = _("Aqua Ring");
static const u8 title_autotomize[] = _("Autotomize");
static const u8 title_smackDown[] = _("Smack Down");
static const u8 title_fairyLock[] = _("Fairy Lock");
static const u8 title_rainbow[] = _("Rainbow");
static const u8 title_swamp[] = _("Swamp");
static const u8 title_seaofFire[] = _("Sea of Fire");
static const u8 title_extremelyHarshSunlight[] = _("Extremely Harsh Sunlight");
static const u8 title_heavyRain[] = _("Heavy Rain");
static const u8 title_strongWinds[] = _("Strong Winds");
static const u8 title_psychicTerrain[] = _("Psychic Terrain");
static const u8 title_throatChop[] = _("Throat Chop");
static const u8 title_auroraVeil[] = _("Aurora Veil");
static const u8 title_laserFocus[] = _("Laser Focus");
static const u8 title_tarShot[] = _("Tar Shot");
static const u8 title_octolock[] = _("Octolock");
static const u8 title_gMaxWildfire[] = _("G-Max Wildfire");
static const u8 title_gMaxVolcalith[] = _("G-Max Volcalith");
static const u8 title_gMaxSteelsurge[] = _("G-Max Steelsurge");
static const u8 title_gMaxVineLash[] = _("G-Max Vine Lash");
static const u8 title_gMaxCannonade[] = _("G-Max Cannonade");

// Descriptions
static const u8 description_harshSunlight[] = _("The harsh sunlight weather condition. It boosts the power of fire-type moves and lowers the power of water-type moves.");
static const u8 description_rain[] = _("The rain weather condition. It boosts the power of water-type moves and lowers the power of fire-type moves.");
static const u8 description_sandstorm[] = _("The sandstorm weather condition. At the end of each turn, it damages all Pokémon that are not rock, ground, or steel types. It boosts the special defense of rock-type Pokémon.");
static const u8 description_hail[] = _("The hail weather condition. At the end of each turn, it damages all Pokémon that are not ice types.");
static const u8 description_electricTerrain[] = _("For five turns, Pokémon on the ground won't fall asleep. The power of electric-type moves is boosted.");
static const u8 description_grassyTerrain[] = _("For five turns, Pokémon on the ground will have a little HP restored at the end of each turn. The power of grass-type moves is boosted.");
static const u8 description_mistyTerrain[] = _("For five turns, Pokémon on the ground won't get any status conditions. Damage from Dragon-type moves is halved.");
static const u8 description_trickRoom[] = _("Slower Pokémon get to move first for five turns.");
static const u8 description_magicRoom[] = _("Pokémon can't use their held items for five turns.");
static const u8 description_wonderRoom[] = _("Pokémon's defense and special defense are swapped for five turns.");
static const u8 description_criticalHitBoost[] = _("The Pokémon is more likely to land critical hits.");
static const u8 description_confusion[] = _("The Pokémon may hurt itself in its confusion. The effect lasts for one to four turns.");
static const u8 description_infatuation[] = _("The Pokémon is less likely to unleash an attack on Pokémon of the opposite gender.");
static const u8 description_nightmare[] = _("The Pokémon will lose HP each turn as long as it is sleeping.");
static const u8 description_drowsy[] = _("The Pokémon will fall asleep at the end of the turn if it remains on the battlefield.");
static const u8 description_encore[] = _("Due to Encore, the Pokémon can use only {STRVAR1} for three turns.");
static const u8 description_noAbility[] = _("The Pokémon's Ability loses its effect.");
static const u8 description_torment[] = _("The Pokémon can't use the same move twice in a row.");
static const u8 description_tailwind[] = _("The Speed stats of a Pokémon and its allies are doubled for four turns.");
static const u8 description_luckyChant[] = _("The opponent's moves won't result in a critical hit.");
static const u8 description_grudge[] = _("If the Pokémon faints by an opponent's move, the PP of that move will be reduced to 0.");
static const u8 description_healBlock[] = _("For five turns, the Pokémon is unable to recover HP through its moves, ability, or held item.");
static const u8 description_identified[] = _("The Pokémon will be hit by certain moves that usually wouldn't affect it. If the Pokémon's evasiveness has been boosted, the benefits will be ignored when it is attacked.");
static const u8 description_moveDisabled[] = _("For four turns, the Pokémon will be unable to use {STRVAR2}.");
static const u8 description_cantEscape[] = _("The Pokémon can't flee or be switched out.");
static const u8 description_lockOn[] = _("The next move used by the Pokémon will be sure to hit the target that was locked on to.");
static const u8 description_embargo[] = _("For five turns, the Pokémon cannot use its held item and no items can be used on it.");
static const u8 description_charge[] = _("The power of an electric-type move will be doubled if used in the turn after charging.");
static const u8 description_gravity[] = _("Moves are more likely to hit for five turns. ground-type moves will hit flying-type Pokémon and Pokémon with the Levitate Ability. Moves that require action midair can't be used.");
static const u8 description_mist[] = _("The Pokémon's stats won't be lowered for five turns.");
static const u8 description_safeguard[] = _("The Pokémon is protected from status conditions for five turns.");
static const u8 description_stealthRock[] = _("Pokémon that switch into battle will take damage.");
static const u8 description_stockpile[] = _("The Pokémon's defense and special defense stats go up while stockpiling.");
static const u8 description_taunt[] = _("The Pokémon can only use moves that deal damage.");
static const u8 description_telekinesis[] = _("For three turns, moves used against the Pokémon will hit without fail, except for one-hit KO moves. Also, ground-type moves won't hit the Pokémon.");
static const u8 description_magnetRise[] = _("Ground-type moves won't hit the Pokémon for five turns.");
static const u8 description_toxicSpikes[] = _("Pokémon that switch into battle will be poisoned. flying-type Pokémon or Pokémon with the Ability 'Levitate' will not be affected.");
static const u8 description_mudsport[] = _("The power of electric-type moves will be halved for five turns.");
static const u8 description_wish[] = _("The Pokémon in this spot will have its HP restored on the turn after Wish was used.");
static const u8 description_stickyWeb[] = _("Pokémon that switch into battle will have their Speed lowered.");
static const u8 description_ingrain[] = _("The Pokémon regains some HP at the end of each turn. Ingrained Pokémon can't be switched out.");
static const u8 description_curse[] = _("The Pokémon takes damage at the end of each turn.");
static const u8 description_trickOrTreat[] = _("The Pokémon gains the ghost type.");
static const u8 description_lightscreen[] = _("Damage from special moves is halved for five turns.");
static const u8 description_reflect[] = _("Damage from physical moves is halved for five turns.");
static const u8 description_imprison[] = _("If a Pokémon affected by Imprison knows any moves also known by the user of Imprison, it is prevented from using those moves.");
static const u8 description_countingDown[] = _("All Pokémon in this battle state will faint after three turns.");
static const u8 description_spikes[] = _("Pokémon that switch into battle will take damage. Flying-type Pokémon or Pokémon with the Levitate Ability will not be affected.");
static const u8 description_watersport[] = _("The power of fire-type moves will be halved for five turns.");
static const u8 description_destinyBond[] = _("If the Pokémon is knocked out by an opponent, that opponent will also faint.");
static const u8 description_forestsCurse[] = _("The Pokémon gains the grass type.");
static const u8 description_leechseed[] = _("Leech Seed steals some of the Pokémon's HP at the end of each turn and gives it to the opponent.");
// static const u8 description_lockOn[] = _("Any move used against a locked-on target will be sure to hit.");
static const u8 description_bound[] = _("The Pokémon is bound and takes damage every turn.");
static const u8 description_bide[] = _("The Pokémon endures attacks for two turns, then strikes back to inflict double the damage it has taken.");
static const u8 description_outrage[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
static const u8 description_rampaging[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
static const u8 description_petalDance[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
static const u8 description_badlyPoisoned[] = _("The Pokémon is badly poisoned. The damage the Pokémon takes from the poison worsens every turn.");
static const u8 description_futureattack[] = _("The Pokémon will take damage two turns after an attack was foreseen.");
static const u8 description_uproar[] = _("The Pokémon attacks and causes an uproar for three turns. During this time, no Pokémon can fall asleep.");
static const u8 description_aquaRing[] = _("The Pokémon is enveloped in a veil made of water. It regains some HP every turn.");
static const u8 description_autotomize[] = _("The Pokémon's weight is reduced, and its Speed is sharply increased.");
static const u8 description_smackDown[] = _("The Pokémon has been knocked out of the skyand has fallen to the ground.");
static const u8 description_fairyLock[] = _("No Pokémon can flee the turn after Fairy Lock is used.");
static const u8 description_rainbow[] = _("The additional effects of moves are more likely to occur.");
static const u8 description_swamp[] = _("Speed is reduced by 75\% in swampy conditions.");
static const u8 description_seaofFire[] = _("Pokémon that are not fire types will take damage every turn.");
static const u8 description_extremelyHarshSunlight[] = _("The extremely harsh sunlight weather condition. It boosts the power of fire-type moves and protects Pokémon from water-type moves.");
static const u8 description_heavyRain[] = _("The heavy rain weather condition. It boosts the power of water-type moves and protects Pokémon from fire-type moves.");
static const u8 description_strongWinds[] = _("The strong winds weather condition. The power of moves that are super effective against flying-type Pokémon is decreased.");
static const u8 description_psychicTerrain[] = _("For five turns, Pokémon on the ground won't be hit by priority moves. The power of psychic-type moves is boosted.");
static const u8 description_throatChop[] = _("The Pokémon can't use any sound-based moves for two turns.");
static const u8 description_auroraVeil[] = _("For five turns, the power of special and physical moves will be halved.");
static const u8 description_laserFocus[] = _("The next move used will always result in a critical hit.");
static const u8 description_tarShot[] = _("The Pokémon has been made weaker to fire-type moves.");
static const u8 description_octolock[] = _("The Pokémon's defense and special defense are both lowered with each passing turn.");
static const u8 description_gMaxWildfire[] = _("Pokémon that are not fire types will take damage every turn for four turns.");
static const u8 description_gMaxVolcalith[] = _("Pokémon that are not rock types will take damage every turn for four turns.");
static const u8 description_gMaxSteelsurge[] = _("Pokémon that switch into battle will take damage.");
static const u8 description_gMaxVineLash[] = _("Pokémon that are not grass types will take damage every turn for four turns.");
static const u8 description_gMaxCannonade[] = _("Pokémon that are not water types will take damage every turn for four turns.");



#define terrainEffect(__WHICH__) {.title=title_##__WHICH__##, .description=description_##__WHICH__##}
const struct terrain_effect const gTerrainEffects[NUM_TERRAIN_EFFECT_DESCRIPTIONS] =
{
    [DESCRIPTION_EXTREMELY_HARSH_SUNLIGHT] = terrainEffect(extremelyHarshSunlight),  // Not in the game yet
    [DESCRIPTION_HEAVY_RAIN] = terrainEffect(heavyRain),                             // Not in the game yet
    [DESCRIPTION_HARSH_SUNLIGHT] = terrainEffect(harshSunlight),
    [DESCRIPTION_RAIN] = terrainEffect(rain),
    [DESCRIPTION_SANDSTORM] = terrainEffect(sandstorm),
    [DESCRIPTION_HAIL] = terrainEffect(hail),
    [DESCRIPTION_MAGIC_ROOM] = terrainEffect(magicRoom),
    [DESCRIPTION_TRICK_ROOM] = terrainEffect(trickRoom),
    [DESCRIPTION_WONDER_ROOM] = terrainEffect(wonderRoom),
    [DESCRIPTION_MUDSPORT] = terrainEffect(mudsport),
    [DESCRIPTION_WATERSPORT] = terrainEffect(watersport),
    [DESCRIPTION_GRAVITY] = terrainEffect(gravity),
    [DESCRIPTION_GRASSY_TERRAIN] = terrainEffect(grassyTerrain),
    [DESCRIPTION_MISTY_TERRAIN] = terrainEffect(mistyTerrain),
    [DESCRIPTION_ELECTRIC_TERRAIN] = terrainEffect(electricTerrain),
    [DESCRIPTION_PSYCHIC_TERRAIN] = terrainEffect(psychicTerrain),
    [DESCRIPTION_FAIRY_LOCK] = terrainEffect(fairyLock),
    [DESCRIPTION_CRITICAL_HIT_BOOST] = terrainEffect(criticalHitBoost),
    [DESCRIPTION_CONFUSION] = terrainEffect(confusion),
    [DESCRIPTION_INFATUATION] = terrainEffect(infatuation),
    [DESCRIPTION_NIGHTMARE] = terrainEffect(nightmare),
    [DESCRIPTION_DROWSY] = terrainEffect(drowsy),
    [DESCRIPTION_ENCORE] = terrainEffect(encore),
    [DESCRIPTION_NO_ABILITY] = terrainEffect(noAbility),
    [DESCRIPTION_TORMENT] = terrainEffect(torment),
    [DESCRIPTION_TAILWIND] = terrainEffect(tailwind),
    [DESCRIPTION_LUCKY_CHANT] = terrainEffect(luckyChant),
    [DESCRIPTION_GRUDGE] = terrainEffect(grudge),
    [DESCRIPTION_HEAL_BLOCK] = terrainEffect(healBlock),
    [DESCRIPTION_IDENTIFIED] = terrainEffect(identified),
    [DESCRIPTION_MOVE_DISABLED] = terrainEffect(moveDisabled),
    [DESCRIPTION_CANT_ESCAPE] = terrainEffect(cantEscape),
    [DESCRIPTION_LOCK_ON] = terrainEffect(lockOn),
    [DESCRIPTION_EMBARGO] = terrainEffect(embargo),
    [DESCRIPTION_CHARGE] = terrainEffect(charge),
    [DESCRIPTION_MIST] = terrainEffect(mist),
    [DESCRIPTION_SAFEGUARD] = terrainEffect(safeguard),
    [DESCRIPTION_STEALTH_ROCK] = terrainEffect(stealthRock),
    [DESCRIPTION_STOCKPILE] = terrainEffect(stockpile),
    [DESCRIPTION_TAUNT] = terrainEffect(taunt),
    [DESCRIPTION_TELEKINESIS] = terrainEffect(telekinesis),
    [DESCRIPTION_MAGNET_RISE] = terrainEffect(magnetRise),
    [DESCRIPTION_TOXIC_SPIKES] = terrainEffect(toxicSpikes),
    [DESCRIPTION_WISH] = terrainEffect(wish),
    [DESCRIPTION_STICKY_WEB] = terrainEffect(stickyWeb),
    [DESCRIPTION_INGRAIN] = terrainEffect(ingrain),
    [DESCRIPTION_CURSE] = terrainEffect(curse),
    [DESCRIPTION_TRICK_OR_TREAT] = terrainEffect(trickOrTreat),  // Not used yet
    [DESCRIPTION_LIGHTSCREEN] = terrainEffect(lightscreen),
    [DESCRIPTION_REFLECT] = terrainEffect(reflect),
    [DESCRIPTION_IMPRISON] = terrainEffect(imprison),
    [DESCRIPTION_COUNTING_DOWN] = terrainEffect(countingDown),
    [DESCRIPTION_SPIKES] = terrainEffect(spikes),
    [DESCRIPTION_DESTINY_BOND] = terrainEffect(destinyBond),
    [DESCRIPTION_FORESTS_CURSE] = terrainEffect(forestsCurse),    // Not used yet
    [DESCRIPTION_LEECHSEED] = terrainEffect(leechseed),
    // [DESCRIPTION_LOCK_ON] = terrainEffect(lockOn),
    [DESCRIPTION_BOUND] = terrainEffect(bound),
    [DESCRIPTION_BIDE] = terrainEffect(bide),
    [DESCRIPTION_OUTRAGE] = terrainEffect(outrage),
    [DESCRIPTION_RAMPAGING] = terrainEffect(rampaging),
    [DESCRIPTION_PETAL_DANCE] = terrainEffect(petalDance),
    [DESCRIPTION_BADLY_POISONED] = terrainEffect(badlyPoisoned),
    [DESCRIPTION_FUTUREATTACK] = terrainEffect(futureattack),
    [DESCRIPTION_UPROAR] = terrainEffect(uproar),
    [DESCRIPTION_AQUA_RING] = terrainEffect(aquaRing),
    [DESCRIPTION_AUTOTOMIZE] = terrainEffect(autotomize),
    [DESCRIPTION_SMACK_DOWN] = terrainEffect(smackDown),
    [DESCRIPTION_RAINBOW] = terrainEffect(rainbow),                          // Not used yet
    [DESCRIPTION_SWAMP] = terrainEffect(swamp),                              // Not used yet
    [DESCRIPTION_SEAOF_FIRE] = terrainEffect(seaofFire),                     // Not used yet
    [DESCRIPTION_STRONG_WINDS] = terrainEffect(strongWinds),                 // Not used yet
    [DESCRIPTION_THROAT_CHOP] = terrainEffect(throatChop),
    [DESCRIPTION_AURORA_VEIL] = terrainEffect(auroraVeil),
    [DESCRIPTION_LASER_FOCUS] = terrainEffect(laserFocus),
    [DESCRIPTION_TAR_SHOT] = terrainEffect(tarShot),                         // Not used yet?
    [DESCRIPTION_OCTOLOCK] = terrainEffect(octolock),                        // Not used yet?
    [DESCRIPTION_G_MAX_WILDFIRE] = terrainEffect(gMaxWildfire),              // Not used yet
    [DESCRIPTION_G_MAX_VOLCALITH] = terrainEffect(gMaxVolcalith),            // Not used yet
    [DESCRIPTION_G_MAX_STEELSURGE] = terrainEffect(gMaxSteelsurge),          // Not used yet
    [DESCRIPTION_G_MAX_VINE_LASH] = terrainEffect(gMaxVineLash),             // Not used yet
    [DESCRIPTION_G_MAX_CANNONADE] = terrainEffect(gMaxCannonade),            // Not used yet
};
#undef terrainEffect
#endif  // TERRAIN_EFFECTS_H