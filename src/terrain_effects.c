#include "global.h"
#include "battle.h"
#include "terrain_effects.h"
#include "constants/moves.h"

// Font Macros
#define FONT_BIG (7)
#define FONT_SMALL (0)

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
static const u8 title_cantEscape[] = _("Can't Escape");
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
static const u8 title_forestsCurse[] = _("Forest's Curse");
static const u8 title_leechseed[] = _("Leech Seed");
static const u8 title_lockedOn[] = _("Locked-On");
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
static const u8 title_extremelyHarshSunlight[] = _("Extremely Harsh Sun");
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
static const u8 description_harshSunlight[] = _("The harsh sunlight\nweather condition.\nIt boosts the power of\nfire-type moves and\nlowers the power of\nwater-type moves.");
static const u8 description_rain[] = _("The rain weather\ncondition.  It boosts\nthe power of\nwater-type moves and\nlowers the power of\nfire-type moves.");
static const u8 description_sandstorm[] = _("The sandstorm\nweather condition.\nAt the end of each\nturn, it damages all\nPokémon that are not\nrock, ground, or\nsteel types.  It\nboosts the special\ndefense of rock-type\nPokémon.");
static const u8 description_hail[] = _("The hail weather\ncondition. At the end\nof each turn, it\ndamages all Pokémon\nthat are not ice types.");
static const u8 description_electricTerrain[] = _("For five turns,\nPokémon on the ground\nwon't fall asleep. The\npower of\nelectric-type moves\nis boosted.");
static const u8 description_grassyTerrain[] = _("For five turns,\nPokémon on the ground\nwill have a little HP\nrestored at the end of\neach turn.  The power\nof grass-type moves\nis boosted.");
static const u8 description_mistyTerrain[] = _("For five turns,\nPokémon on the ground\nwon't get any status\nconditions.  Damage\nfrom Dragon-type\nmoves is halved.");
static const u8 description_trickRoom[] = _("Slower Pokémon get\nto move first for five\nturns.");
static const u8 description_magicRoom[] = _("Pokémon can't use\ntheir held items for\nfive turns.");
static const u8 description_wonderRoom[] = _("Pokémon's defense\nand special defense\nare swapped for five\nturns.");
static const u8 description_criticalHitBoost[] = _("The Pokémon is more\nlikely to land\ncritical hits.");
static const u8 description_confusion[] = _("The Pokémon may hurt\nitself in its\nconfusion.  The\neffect lasts for one to\nfour turns.");
static const u8 description_infatuation[] = _("The Pokémon is less\nlikely to unleash an\nattack on Pokémon of\nthe opposite gender.");
static const u8 description_nightmare[] = _("The Pokémon will lose\nHP each turn as long as\nit is sleeping.");
static const u8 description_drowsy[] = _("The Pokémon will fall\nasleep at the end of the\nturn if it remains on\nthe battlefield.");
static const u8 description_encore[] = _("Due to Encore, the Pokémon can use only {STR_VAR_1} for three turns.");
static const u8 description_noAbility[] = _("The Pokémon's\nAbility loses its\neffect.");
static const u8 description_torment[] = _("The Pokémon can't use\nthe same move twice in a\nrow.");
static const u8 description_tailwind[] = _("The Speed stats of a\nPokémon and its allies\nare doubled for four\nturns.");
static const u8 description_luckyChant[] = _("The opponent's\nmoves won't result in a\ncritical hit.");
static const u8 description_grudge[] = _("If the Pokémon faints\nby an opponent's move,\nthe PP of that move will\nbe reduced to 0.");
static const u8 description_healBlock[] = _("For five turns, the\nPokémon is unable to\nrecover HP through\nits moves, ability, or\nheld item.");
static const u8 description_identified[] = _("The Pokémon will be hit\nby certain moves\nthat usually\nwouldn't affect it.\nIf the Pokémon's\nevasiveness has been\nboosted, the\nbenefits will be\nignored when it is\nattacked.");
static const u8 description_moveDisabled[] = _("For four turns, the Pokémon will be unable to use {STR_VAR_1}.");
static const u8 description_cantEscape[] = _("The Pokémon can't\nflee or be switched\nout.");
static const u8 description_lockOn[] = _("The next move used by\nthe Pokémon will be\nsure to hit the target\nthat was locked on to.");
static const u8 description_embargo[] = _("For five turns, the\nPokémon cannot use\nits held item and no\nitems can be used on it.");
static const u8 description_charge[] = _("The power of an\nelectric-type move\nwill be doubled if used\nin the turn after\ncharging.");
static const u8 description_gravity[] = _("Moves are more\nlikely to hit for\nfive turns.\nGround-type moves\nwill hit flying-type\nPokémon and Pokémon\nwith the Levitate\nAbility.  Moves that\nrequire action midair\ncan't be used.");
static const u8 description_mist[] = _("The Pokémon's stats\nwon't be lowered for\nfive turns.");
static const u8 description_safeguard[] = _("The Pokémon is\nprotected from\nstatus conditions\nfor five turns.");
static const u8 description_stealthRock[] = _("Pokémon that switch\ninto battle will take\ndamage.");
static const u8 description_stockpile[] = _("The Pokémon's\ndefense and special\ndefense stats go up\nwhile stockpiling.");
static const u8 description_taunt[] = _("The Pokémon can only\nuse moves that deal\ndamage.");
static const u8 description_telekinesis[] = _("For three turns,\nmoves will always hit\nthe Pokémon (except\nfor one-hit KO moves).\nAlso, ground-type moves\nwon't hit the\nPokémon.");
static const u8 description_magnetRise[] = _("Ground-type moves\nwon't hit the Pokémon\nfor five turns.");
static const u8 description_toxicSpikes[] = _("Pokémon will be\npoisoned upon\nswitching in, unless\nthey are flying-type\nor have the Ability\n'Levitate'.");
static const u8 description_mudsport[] = _("The power of\nelectric-type moves\nwill be halved for five\nturns.");
static const u8 description_wish[] = _("The Pokémon in this\nspot will have its HP\nrestored on the turn\nafter Wish was used.");
static const u8 description_stickyWeb[] = _("Pokémon that switch\ninto battle will have\ntheir Speed lowered.");
static const u8 description_ingrain[] = _("The Pokémon regains\nsome HP at the end of\neach turn.  Ingrained\nPokémon can't be\nswitched out.");
static const u8 description_curse[] = _("The Pokémon takes\ndamage at the end of\neach turn.");
static const u8 description_trickOrTreat[] = _("The Pokémon gains\nthe ghost type.");
static const u8 description_lightscreen[] = _("Damage from special\nmoves is halved for\nfive turns.");
static const u8 description_reflect[] = _("Damage from\nphysical moves is\nhalved for five turns.");
static const u8 description_imprison[] = _("If a Pokémon\naffected by Imprison\nknows any moves also\nknown by the user of\nImprison, it is\nprevented from using\nthose moves.");
static const u8 description_countingDown[] = _("All Pokémon in this\nbattle state will\nfaint after three\nturns.");
static const u8 description_spikes[] = _("Pokémon will take\ndamage upon\nswitching in, unless\nthey are flying-type\nor have the Ability\n'Levitate'.");
static const u8 description_watersport[] = _("The power of\nfire-type moves will\nbe halved for five\nturns.");
static const u8 description_destinyBond[] = _("If the Pokémon is\nknocked out by an\nopponent, that\nopponent will also\nfaint.");
static const u8 description_forestsCurse[] = _("The Pokémon gains\nthe grass type.");
static const u8 description_leechseed[] = _("Leech Seed steals\nsome of the Pokémon's\nHP at the end of each\nturn and gives it to the\nopponent.");
static const u8 description_lockedOn[] = _("Any move used\nagainst a locked-on\ntarget will be sure to\nhit.");
static const u8 description_bound[] = _("The Pokémon is bound\nand takes damage\nevery turn.");
static const u8 description_bide[] = _("The Pokémon endures\nattacks for two turns,\nthen strikes back to\ninflict double the\ndamage it has taken.");
static const u8 description_outrage[] = _("The Pokémon\nrampages and attacks\nfor two to three turns.\nIt then becomes\nconfused.");
static const u8 description_rampaging[] = _("The Pokémon\nrampages and attacks\nfor two to three turns.\nIt then becomes\nconfused.");
static const u8 description_petalDance[] = _("The Pokémon\nrampages and attacks\nfor two to three turns.\nIt then becomes\nconfused.");
static const u8 description_badlyPoisoned[] = _("The Pokémon is badly\npoisoned.  The\ndamage the Pokémon\ntakes from the poison\nworsens every turn.");
static const u8 description_futureattack[] = _("The Pokémon will take\ndamage two turns\nafter an attack was\nforeseen.");
static const u8 description_uproar[] = _("The Pokémon attacks\nand causes an uproar\nfor three turns.\nDuring this time, no\nPokémon can fall\nasleep.");
static const u8 description_aquaRing[] = _("The Pokémon is\nenveloped in a veil\nmade of water.  It\nregains some HP every\nturn.");
static const u8 description_autotomize[] = _("The Pokémon's\nweight is reduced, and\nits Speed is sharply\nincreased.");
static const u8 description_smackDown[] = _("The Pokémon has been\nknocked out of the\nsky and has fallen to\nthe ground.");
static const u8 description_fairyLock[] = _("No Pokémon can flee\nthe turn after\n'Fairy Lock' is used.");
static const u8 description_rainbow[] = _("The additional\neffects of moves are\nmore likely to occur.");
static const u8 description_swamp[] = _("Speed is reduced by\n75% in swampy\nconditions.");
static const u8 description_seaofFire[] = _("Pokémon that are not\nfire types will take\ndamage every turn.");
static const u8 description_extremelyHarshSunlight[] = _("The extremely harsh\nsunlight weather\ncondition.  It boosts\nthe power of\nfire-type moves and\nprotects from\nwater-type moves.");
static const u8 description_heavyRain[] = _("The heavy rain\nweather condition.\nIt boosts the power of\nwater-type moves and\nprotects Pokémon\nfrom fire-type\nmoves.");
static const u8 description_strongWinds[] = _("The strong winds\nweather condition.\nThe power of moves\nthat are super\neffective against\nflying-type Pokémon\nis decreased.");
static const u8 description_psychicTerrain[] = _("For five turns,\nPokémon on the ground\nwon't be hit by priority\nmoves.  The power of\npsychic-type moves\nis boosted.");
static const u8 description_throatChop[] = _("The Pokémon can't use\nany sound-based\nmoves for two turns.");
static const u8 description_auroraVeil[] = _("For five turns, the\npower of special and\nphysical moves will be\nhalved.");
static const u8 description_laserFocus[] = _("The next move used\nwill always result in a\ncritical hit.");
static const u8 description_tarShot[] = _("The Pokémon has been\nmade weaker to\nfire-type moves.");
static const u8 description_octolock[] = _("The Pokémon's\ndefense and special\ndefense are both\nlowered with each\npassing turn.");
static const u8 description_gMaxWildfire[] = _("Pokémon that are not\nfire types will take\ndamage every turn for\nfour turns.");
static const u8 description_gMaxVolcalith[] = _("Pokémon that are not\nrock types will take\ndamage every turn for\nfour turns.");
static const u8 description_gMaxSteelsurge[] = _("Pokémon that switch\ninto battle will take\ndamage.");
static const u8 description_gMaxVineLash[] = _("Pokémon that are not\ngrass types will take\ndamage every turn for\nfour turns.");
static const u8 description_gMaxCannonade[] = _("Pokémon that are not\nwater types will take\ndamage every turn for\nfour turns.");

// The raw strings, in case some other part of the game needs them
// static const u8 description_harshSunlight[] = _("The harsh sunlight weather condition. It boosts the power of fire-type moves and lowers the power of water-type moves.");
// static const u8 description_rain[] = _("The rain weather condition. It boosts the power of water-type moves and lowers the power of fire-type moves.");
// static const u8 description_sandstorm[] = _("The sandstorm weather condition. At the end of each turn, it damages all Pokémon that are not rock, ground, or steel types. It boosts the special defense of rock-type Pokémon.");
// static const u8 description_hail[] = _("The hail weather condition. At the end of each turn, it damages all Pokémon that are not ice types.");
// static const u8 description_electricTerrain[] = _("For five turns, Pokémon on the ground won't fall asleep. The power of electric-type moves is boosted.");
// static const u8 description_grassyTerrain[] = _("For five turns, Pokémon on the ground will have a little HP restored at the end of each turn. The power of grass-type moves is boosted.");
// static const u8 description_mistyTerrain[] = _("For five turns, Pokémon on the ground won't get any status conditions. Damage from Dragon-type moves is halved.");
// static const u8 description_trickRoom[] = _("Slower Pokémon get to move first for five turns.");
// static const u8 description_magicRoom[] = _("Pokémon can't use their held items for five turns.");
// static const u8 description_wonderRoom[] = _("Pokémon's defense and special defense are swapped for five turns.");
// static const u8 description_criticalHitBoost[] = _("The Pokémon is more likely to land critical hits.");
// static const u8 description_confusion[] = _("The Pokémon may hurt itself in its confusion. The effect lasts for one to four turns.");
// static const u8 description_infatuation[] = _("The Pokémon is less likely to unleash an attack on Pokémon of the opposite gender.");
// static const u8 description_nightmare[] = _("The Pokémon will lose HP each turn as long as it is sleeping.");
// static const u8 description_drowsy[] = _("The Pokémon will fall asleep at the end of the turn if it remains on the battlefield.");
// static const u8 description_encore[] = _("Due to Encore, the Pokémon can use only {STR_VAR_1} for three turns.");
// static const u8 description_noAbility[] = _("The Pokémon's Ability loses its effect.");
// static const u8 description_torment[] = _("The Pokémon can't use the same move twice in a row.");
// static const u8 description_tailwind[] = _("The Speed stats of a Pokémon and its allies are doubled for four turns.");
// static const u8 description_luckyChant[] = _("The opponent's moves won't result in a critical hit.");
// static const u8 description_grudge[] = _("If the Pokémon faints by an opponent's move, the PP of that move will be reduced to 0.");
// static const u8 description_healBlock[] = _("For five turns, the Pokémon is unable to recover HP through its moves, ability, or held item.");
// static const u8 description_identified[] = _("The Pokémon will be hit by certain moves that usually wouldn't affect it. If the Pokémon's evasiveness has been boosted, the benefits will be ignored when it is attacked.");
// static const u8 description_moveDisabled[] = _("For four turns, the Pokémon will be unable to use {STR_VAR_2}.");
// static const u8 description_cantEscape[] = _("The Pokémon can't flee or be switched out.");
// static const u8 description_lockOn[] = _("The next move used by the Pokémon will be sure to hit the target that was locked on to.");
// static const u8 description_embargo[] = _("For five turns, the Pokémon cannot use its held item and no items can be used on it.");
// static const u8 description_charge[] = _("The power of an electric-type move will be doubled if used in the turn after charging.");
// static const u8 description_gravity[] = _("Moves are more likely to hit for five turns. ground-type moves will hit flying-type Pokémon and Pokémon with the Levitate Ability. Moves that require action midair can't be used.");
// static const u8 description_mist[] = _("The Pokémon's stats won't be lowered for five turns.");
// static const u8 description_safeguard[] = _("The Pokémon is protected from status conditions for five turns.");
// static const u8 description_stealthRock[] = _("Pokémon that switch into battle will take damage.");
// static const u8 description_stockpile[] = _("The Pokémon's defense and special defense stats go up while stockpiling.");
// static const u8 description_taunt[] = _("The Pokémon can only use moves that deal damage.");
// static const u8 description_telekinesis[] = _("For three turns, moves used against the Pokémon will hit without fail, except for one-hit KO moves. Also, ground-type moves won't hit the Pokémon.");
// static const u8 description_magnetRise[] = _("Ground-type moves won't hit the Pokémon for five turns.");
// static const u8 description_toxicSpikes[] = _("Pokémon that switch into battle will be poisoned. flying-type Pokémon or Pokémon with the Ability 'Levitate' will not be affected.");
// static const u8 description_mudsport[] = _("The power of electric-type moves will be halved for five turns.");
// static const u8 description_wish[] = _("The Pokémon in this spot will have its HP restored on the turn after Wish was used.");
// static const u8 description_stickyWeb[] = _("Pokémon that switch into battle will have their Speed lowered.");
// static const u8 description_ingrain[] = _("The Pokémon regains some HP at the end of each turn. Ingrained Pokémon can't be switched out.");
// static const u8 description_curse[] = _("The Pokémon takes damage at the end of each turn.");
// static const u8 description_trickOrTreat[] = _("The Pokémon gains the ghost type.");
// static const u8 description_lightscreen[] = _("Damage from special moves is halved for five turns.");
// static const u8 description_reflect[] = _("Damage from physical moves is halved for five turns.");
// static const u8 description_imprison[] = _("If a Pokémon affected by Imprison knows any moves also known by the user of Imprison, it is prevented from using those moves.");
// static const u8 description_countingDown[] = _("All Pokémon in this battle state will faint after three turns.");
// static const u8 description_spikes[] = _("Pokémon that switch into battle will take damage. Flying-type Pokémon or Pokémon with the Levitate Ability will not be affected.");
// static const u8 description_watersport[] = _("The power of fire-type moves will be halved for five turns.");
// static const u8 description_destinyBond[] = _("If the Pokémon is knocked out by an opponent, that opponent will also faint.");
// static const u8 description_forestsCurse[] = _("The Pokémon gains the grass type.");
// static const u8 description_leechseed[] = _("Leech Seed steals some of the Pokémon's HP at the end of each turn and gives it to the opponent.");
// static const u8 description_lockedOn[] = _("Any move used against a locked-on target will be sure to hit.");
// static const u8 description_bound[] = _("The Pokémon is bound and takes damage every turn.");
// static const u8 description_bide[] = _("The Pokémon endures attacks for two turns, then strikes back to inflict double the damage it has taken.");
// static const u8 description_outrage[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
// static const u8 description_rampaging[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
// static const u8 description_petalDance[] = _("The Pokémon rampages and attacks for two to three turns. It then becomes confused.");
// static const u8 description_badlyPoisoned[] = _("The Pokémon is badly poisoned. The damage the Pokémon takes from the poison worsens every turn.");
// static const u8 description_futureattack[] = _("The Pokémon will take damage two turns after an attack was foreseen.");
// static const u8 description_uproar[] = _("The Pokémon attacks and causes an uproar for three turns. During this time, no Pokémon can fall asleep.");
// static const u8 description_aquaRing[] = _("The Pokémon is enveloped in a veil made of water. It regains some HP every turn.");
// static const u8 description_autotomize[] = _("The Pokémon's weight is reduced, and its Speed is sharply increased.");
// static const u8 description_smackDown[] = _("The Pokémon has been knocked out of the skyand has fallen to the ground.");
// static const u8 description_fairyLock[] = _("No Pokémon can flee the turn after Fairy Lock is used.");
// static const u8 description_rainbow[] = _("The additional effects of moves are more likely to occur.");
// static const u8 description_swamp[] = _("Speed is reduced by 75% in swampy conditions.");
// static const u8 description_seaofFire[] = _("Pokémon that are not fire types will take damage every turn.");
// static const u8 description_extremelyHarshSunlight[] = _("The extremely harsh sunlight weather condition. It boosts the power of fire-type moves and protects Pokémon from water-type moves.");
// static const u8 description_heavyRain[] = _("The heavy rain weather condition. It boosts the power of water-type moves and protects Pokémon from fire-type moves.");
// static const u8 description_strongWinds[] = _("The strong winds weather condition. The power of moves that are super effective against flying-type Pokémon is decreased.");
// static const u8 description_psychicTerrain[] = _("For five turns, Pokémon on the ground won't be hit by priority moves. The power of psychic-type moves is boosted.");
// static const u8 description_throatChop[] = _("The Pokémon can't use any sound-based moves for two turns.");
// static const u8 description_auroraVeil[] = _("For five turns, the power of special and physical moves will be halved.");
// static const u8 description_laserFocus[] = _("The next move used will always result in a critical hit.");
// static const u8 description_tarShot[] = _("The Pokémon has been made weaker to fire-type moves.");
// static const u8 description_octolock[] = _("The Pokémon's defense and special defense are both lowered with each passing turn.");
// static const u8 description_gMaxWildfire[] = _("Pokémon that are not fire types will take damage every turn for four turns.");
// static const u8 description_gMaxVolcalith[] = _("Pokémon that are not rock types will take damage every turn for four turns.");
// static const u8 description_gMaxSteelsurge[] = _("Pokémon that switch into battle will take damage.");
// static const u8 description_gMaxVineLash[] = _("Pokémon that are not grass types will take damage every turn for four turns.");
// static const u8 description_gMaxCannonade[] = _("Pokémon that are not water types will take damage every turn for four turns.");


#define terrainEffect(__WHICH__, __FONT_SIZE__) {.title=title_ ## __WHICH__, .description=description_ ## __WHICH__, .fontSize=__FONT_SIZE__}
static const struct terrain_effect const sTerrainEffects[NUM_TERRAIN_EFFECT_DESCRIPTIONS] =
{
    [TERRAIN_EFFECT_EXTREMELY_HARSH_SUNLIGHT] = terrainEffect(extremelyHarshSunlight, FONT_BIG),  // Not in the game yet
    [TERRAIN_EFFECT_HEAVY_RAIN] = terrainEffect(heavyRain, FONT_BIG),                             // Not in the game yet
    [TERRAIN_EFFECT_HARSH_SUNLIGHT] = terrainEffect(harshSunlight, FONT_BIG),
    [TERRAIN_EFFECT_RAIN] = terrainEffect(rain, FONT_BIG),
    [TERRAIN_EFFECT_SANDSTORM] = terrainEffect(sandstorm, FONT_SMALL),
    [TERRAIN_EFFECT_HAIL] = terrainEffect(hail, FONT_BIG),
    [TERRAIN_EFFECT_MAGIC_ROOM] = terrainEffect(magicRoom, FONT_BIG),
    [TERRAIN_EFFECT_TRICK_ROOM] = terrainEffect(trickRoom, FONT_BIG),
    [TERRAIN_EFFECT_WONDER_ROOM] = terrainEffect(wonderRoom, FONT_BIG),
    [TERRAIN_EFFECT_MUDSPORT] = terrainEffect(mudsport, FONT_BIG),
    [TERRAIN_EFFECT_WATERSPORT] = terrainEffect(watersport, FONT_BIG),
    [TERRAIN_EFFECT_GRAVITY] = terrainEffect(gravity, FONT_SMALL),
    [TERRAIN_EFFECT_GRASSY_TERRAIN] = terrainEffect(grassyTerrain, FONT_BIG),
    [TERRAIN_EFFECT_MISTY_TERRAIN] = terrainEffect(mistyTerrain, FONT_BIG),
    [TERRAIN_EFFECT_ELECTRIC_TERRAIN] = terrainEffect(electricTerrain, FONT_BIG),
    [TERRAIN_EFFECT_PSYCHIC_TERRAIN] = terrainEffect(psychicTerrain, FONT_BIG),
    [TERRAIN_EFFECT_FAIRY_LOCK] = terrainEffect(fairyLock, FONT_BIG),
    [TERRAIN_EFFECT_CRITICAL_HIT_BOOST] = terrainEffect(criticalHitBoost, FONT_BIG),
    [TERRAIN_EFFECT_CONFUSION] = terrainEffect(confusion, FONT_BIG),
    [TERRAIN_EFFECT_INFATUATION] = terrainEffect(infatuation, FONT_BIG),
    [TERRAIN_EFFECT_NIGHTMARE] = terrainEffect(nightmare, FONT_BIG),
    [TERRAIN_EFFECT_DROWSY] = terrainEffect(drowsy, FONT_BIG),
    [TERRAIN_EFFECT_ENCORE] = terrainEffect(encore, FONT_BIG),
    [TERRAIN_EFFECT_NO_ABILITY] = terrainEffect(noAbility, FONT_BIG),
    [TERRAIN_EFFECT_TORMENT] = terrainEffect(torment, FONT_BIG),
    [TERRAIN_EFFECT_TAILWIND] = terrainEffect(tailwind, FONT_BIG),
    [TERRAIN_EFFECT_LUCKY_CHANT] = terrainEffect(luckyChant, FONT_BIG),
    [TERRAIN_EFFECT_GRUDGE] = terrainEffect(grudge, FONT_BIG),
    [TERRAIN_EFFECT_HEAL_BLOCK] = terrainEffect(healBlock, FONT_BIG),
    [TERRAIN_EFFECT_IDENTIFIED] = terrainEffect(identified, FONT_SMALL),
    [TERRAIN_EFFECT_MOVE_DISABLED] = terrainEffect(moveDisabled, FONT_BIG),
    [TERRAIN_EFFECT_CANT_ESCAPE] = terrainEffect(cantEscape, FONT_BIG),
    [TERRAIN_EFFECT_LOCK_ON] = terrainEffect(lockOn, FONT_BIG),
    [TERRAIN_EFFECT_EMBARGO] = terrainEffect(embargo, FONT_BIG),
    [TERRAIN_EFFECT_CHARGE] = terrainEffect(charge, FONT_BIG),
    [TERRAIN_EFFECT_MIST] = terrainEffect(mist, FONT_BIG),
    [TERRAIN_EFFECT_SAFEGUARD] = terrainEffect(safeguard, FONT_BIG),
    [TERRAIN_EFFECT_STEALTH_ROCK] = terrainEffect(stealthRock, FONT_BIG),
    [TERRAIN_EFFECT_STOCKPILE] = terrainEffect(stockpile, FONT_BIG),
    [TERRAIN_EFFECT_TAUNT] = terrainEffect(taunt, FONT_BIG),
    [TERRAIN_EFFECT_TELEKINESIS] = terrainEffect(telekinesis, FONT_BIG),
    [TERRAIN_EFFECT_MAGNET_RISE] = terrainEffect(magnetRise, FONT_BIG),
    [TERRAIN_EFFECT_TOXIC_SPIKES] = terrainEffect(toxicSpikes, FONT_BIG),
    [TERRAIN_EFFECT_WISH] = terrainEffect(wish, FONT_BIG),
    [TERRAIN_EFFECT_STICKY_WEB] = terrainEffect(stickyWeb, FONT_BIG),
    [TERRAIN_EFFECT_INGRAIN] = terrainEffect(ingrain, FONT_BIG),
    [TERRAIN_EFFECT_CURSE] = terrainEffect(curse, FONT_BIG),
    [TERRAIN_EFFECT_TRICK_OR_TREAT] = terrainEffect(trickOrTreat, FONT_BIG),              // Not used yet
    [TERRAIN_EFFECT_LIGHTSCREEN] = terrainEffect(lightscreen, FONT_BIG),
    [TERRAIN_EFFECT_REFLECT] = terrainEffect(reflect, FONT_BIG),
    [TERRAIN_EFFECT_IMPRISON] = terrainEffect(imprison, FONT_BIG),
    [TERRAIN_EFFECT_COUNTING_DOWN] = terrainEffect(countingDown, FONT_BIG),
    [TERRAIN_EFFECT_SPIKES] = terrainEffect(spikes, FONT_BIG),
    [TERRAIN_EFFECT_DESTINY_BOND] = terrainEffect(destinyBond, FONT_BIG),
    [TERRAIN_EFFECT_FORESTS_CURSE] = terrainEffect(forestsCurse, FONT_BIG),               // Not used yet
    [TERRAIN_EFFECT_LEECHSEED] = terrainEffect(leechseed, FONT_BIG),
    [TERRAIN_EFFECT_LOCKED_ON] = terrainEffect(lockedOn, FONT_BIG),
    [TERRAIN_EFFECT_BOUND] = terrainEffect(bound, FONT_BIG),
    [TERRAIN_EFFECT_BIDE] = terrainEffect(bide, FONT_BIG),
    [TERRAIN_EFFECT_OUTRAGE] = terrainEffect(outrage, FONT_BIG),
    [TERRAIN_EFFECT_RAMPAGING] = terrainEffect(rampaging, FONT_BIG),
    [TERRAIN_EFFECT_PETAL_DANCE] = terrainEffect(petalDance, FONT_BIG),
    [TERRAIN_EFFECT_BADLY_POISONED] = terrainEffect(badlyPoisoned, FONT_BIG),
    [TERRAIN_EFFECT_FUTUREATTACK] = terrainEffect(futureattack, FONT_BIG),
    [TERRAIN_EFFECT_UPROAR] = terrainEffect(uproar, FONT_BIG),
    [TERRAIN_EFFECT_AQUA_RING] = terrainEffect(aquaRing, FONT_BIG),
    [TERRAIN_EFFECT_AUTOTOMIZE] = terrainEffect(autotomize, FONT_BIG),
    [TERRAIN_EFFECT_SMACK_DOWN] = terrainEffect(smackDown, FONT_BIG),
    [TERRAIN_EFFECT_RAINBOW] = terrainEffect(rainbow, FONT_BIG),                          // Not used yet
    [TERRAIN_EFFECT_SWAMP] = terrainEffect(swamp, FONT_BIG),                              // Not used yet
    [TERRAIN_EFFECT_SEAOF_FIRE] = terrainEffect(seaofFire, FONT_BIG),                     // Not used yet
    [TERRAIN_EFFECT_STRONG_WINDS] = terrainEffect(strongWinds, FONT_BIG),                 // Not used yet
    [TERRAIN_EFFECT_THROAT_CHOP] = terrainEffect(throatChop, FONT_BIG),
    [TERRAIN_EFFECT_AURORA_VEIL] = terrainEffect(auroraVeil, FONT_BIG),
    [TERRAIN_EFFECT_LASER_FOCUS] = terrainEffect(laserFocus, FONT_BIG),
    [TERRAIN_EFFECT_TAR_SHOT] = terrainEffect(tarShot, FONT_BIG),                         // Not used yet?
    [TERRAIN_EFFECT_OCTOLOCK] = terrainEffect(octolock, FONT_BIG),                        // Not used yet?
    [TERRAIN_EFFECT_G_MAX_WILDFIRE] = terrainEffect(gMaxWildfire, FONT_BIG),              // Not used yet
    [TERRAIN_EFFECT_G_MAX_VOLCALITH] = terrainEffect(gMaxVolcalith, FONT_BIG),            // Not used yet
    [TERRAIN_EFFECT_G_MAX_STEELSURGE] = terrainEffect(gMaxSteelsurge, FONT_BIG),          // Not used yet
    [TERRAIN_EFFECT_G_MAX_VINE_LASH] = terrainEffect(gMaxVineLash, FONT_BIG),             // Not used yet
    [TERRAIN_EFFECT_G_MAX_CANNONADE] = terrainEffect(gMaxCannonade, FONT_BIG),            // Not used yet
};

struct terrain_effect GetTerrainEffect(u8 terrainId)
{
    return sTerrainEffects[terrainId];
}

#define setTerrainEffect(__WHICH_EFFECT__) \
    curTerrainEffects[curIndex] = __WHICH_EFFECT__; \
    if(++curIndex == NUM_TERRAIN_EFFECTS) \
        return; \

#define handleFieldStatus(__WHICH__) \
if (gFieldStatuses & STATUS_FIELD_ ## __WHICH__) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \

#define handleMonStatus1(__WHICH__, __STATUS__) \
if (gBattleMons[monIndex].status1 & STATUS1_ ## __STATUS__) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \

#define handleMonStatus2(__WHICH__, __STATUS__) \
if (gBattleMons[monIndex].status2 & STATUS2_ ## __STATUS__) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \

#define handleMonStatus3(__WHICH__, __STATUS__) \
if (gStatuses3[monIndex] & STATUS3_ ## __STATUS__) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \

#define handleDisableStruct(__WHICH__, __STRUCT_ATTR__) \
if (gDisableStructs[monIndex] ## __STRUCT_ATTR__ != 0) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \

#define handleSideStatus(__WHICH__) \
if (sideStatus & SIDE_STATUS_ ## __WHICH__) \
{ \
    setTerrainEffect(TERRAIN_EFFECT_ ## __WHICH__); \
} \


void GetActiveTerrainEffects(u8 monIndex, u8*curTerrainEffects)
{
    u8 curIndex = 0;
    u8 i;
    u32 sideStatus = gSideStatuses[GET_BATTLER_SIDE(monIndex)];

    // Null out the existing array
    for (i = 0; i < NUM_TERRAIN_EFFECTS; i++)
    {
        curTerrainEffects[i] = TERRAIN_EFFECT_NULL;
    }

    // First, check the weather states
    if (gBattleWeather & WEATHER_ANY)
    {
        if (gBattleWeather & WEATHER_SUN_ANY)
        {
            curTerrainEffects[curIndex] = TERRAIN_EFFECT_HARSH_SUNLIGHT;
        }
        else if (gBattleWeather & WEATHER_RAIN_ANY)
        {
            curTerrainEffects[curIndex] = TERRAIN_EFFECT_RAIN;
        }
        else if (gBattleWeather & WEATHER_SANDSTORM_ANY)
        {
            curTerrainEffects[curIndex] = TERRAIN_EFFECT_SANDSTORM;
        }
        else if (gBattleWeather & WEATHER_HAIL_ANY)
        {
            curTerrainEffects[curIndex] = TERRAIN_EFFECT_HAIL;
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
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_GRASSY_TERRAIN;
            }
            else if (gFieldStatuses & STATUS_FIELD_MISTY_TERRAIN)
            {
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_ELECTRIC_TERRAIN;
            }
            else if (gFieldStatuses & STATUS_FIELD_ELECTRIC_TERRAIN)
            {
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_MISTY_TERRAIN;
            }
            else if (gFieldStatuses & STATUS_FIELD_PSYCHIC_TERRAIN)
            {
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_PSYCHIC_TERRAIN;
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
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_OUTRAGE;
                break;
            case MOVE_THRASH:
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_RAMPAGING;
                break;
            case MOVE_PETAL_DANCE:
                curTerrainEffects[curIndex] = TERRAIN_EFFECT_PETAL_DANCE;
                break;
            }
            if (++curIndex == NUM_TERRAIN_EFFECTS)
                return;
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
        handleMonStatus3(LOCKED_ON, ALWAYS_HITS);
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

    if (gSideTimers[sideStatus].mistTimer)
    {
        setTerrainEffect(TERRAIN_EFFECT_MIST);
    }

    if (gDisableStructs[monIndex].encoredMove != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_ENCORE);
    }
    if (gDisableStructs[monIndex].disabledMove != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_MOVE_DISABLED);
    }
    if (gDisableStructs[monIndex].stockpileCounter != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_STOCKPILE);
    }
    if (gDisableStructs[monIndex].tauntTimer != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_TAUNT);
    }
    if (gDisableStructs[monIndex].perishSongTimer != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_COUNTING_DOWN);
    }
    if (gDisableStructs[monIndex].autotomizeCount != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_AUTOTOMIZE);
    }
    if (gDisableStructs[monIndex].throatChopTimer != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_THROAT_CHOP);
    }

    if (gWishFutureKnock.wishCounter[gBattlerAttacker] != 0)
    {
        setTerrainEffect(TERRAIN_EFFECT_WISH);
    }

    for (i = 0; i < gBattlersCount; i++)
    {
        if (i != monIndex && gStatuses3[i] & STATUS3_ALWAYS_HITS && gDisableStructs[i].battlerWithSureHit == monIndex)
        {
            setTerrainEffect(TERRAIN_EFFECT_LOCK_ON);
            break;
        }
    }
}
#undef setTerrainEffect
#undef setTerrainEffect
#undef handleFieldStatus
#undef handleMonStatus1
#undef handleMonStatus2
#undef handleMonStatus3
#undef handleDisableStruct
#undef handleSideStatus
#undef handleSideTimers


#undef terrainEffect