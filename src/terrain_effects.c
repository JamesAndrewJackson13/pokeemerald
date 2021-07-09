#include "global.h"
#include "battle.h"
#include "terrain_effects.h"
#include "constants/moves.h"


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
static const u8 description_encore[] = _("Due to Encore, the Pokémon can use only {STR_VAR_1} for three turns.");
static const u8 description_noAbility[] = _("The Pokémon's Ability loses its effect.");
static const u8 description_torment[] = _("The Pokémon can't use the same move twice in a row.");
static const u8 description_tailwind[] = _("The Speed stats of a Pokémon and its allies are doubled for four turns.");
static const u8 description_luckyChant[] = _("The opponent's moves won't result in a critical hit.");
static const u8 description_grudge[] = _("If the Pokémon faints by an opponent's move, the PP of that move will be reduced to 0.");
static const u8 description_healBlock[] = _("For five turns, the Pokémon is unable to recover HP through its moves, ability, or held item.");
static const u8 description_identified[] = _("The Pokémon will be hit by certain moves that usually wouldn't affect it. If the Pokémon's evasiveness has been boosted, the benefits will be ignored when it is attacked.");
static const u8 description_moveDisabled[] = _("For four turns, the Pokémon will be unable to use {STR_VAR_2}.");
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
static const u8 description_lockedOn[] = _("Any move used against a locked-on target will be sure to hit.");
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
static const u8 description_swamp[] = _("Speed is reduced by 75% in swampy conditions.");
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



#define terrainEffect(__WHICH__) {.title=title_ ## __WHICH__, .description=description_ ## __WHICH__}
static const struct terrain_effect const sTerrainEffects[NUM_TERRAIN_EFFECT_DESCRIPTIONS] =
{
    [TERRAIN_EFFECT_EXTREMELY_HARSH_SUNLIGHT] = terrainEffect(extremelyHarshSunlight),  // Not in the game yet
    [TERRAIN_EFFECT_HEAVY_RAIN] = terrainEffect(heavyRain),                             // Not in the game yet
    [TERRAIN_EFFECT_HARSH_SUNLIGHT] = terrainEffect(harshSunlight),
    [TERRAIN_EFFECT_RAIN] = terrainEffect(rain),
    [TERRAIN_EFFECT_SANDSTORM] = terrainEffect(sandstorm),
    [TERRAIN_EFFECT_HAIL] = terrainEffect(hail),
    [TERRAIN_EFFECT_MAGIC_ROOM] = terrainEffect(magicRoom),
    [TERRAIN_EFFECT_TRICK_ROOM] = terrainEffect(trickRoom),
    [TERRAIN_EFFECT_WONDER_ROOM] = terrainEffect(wonderRoom),
    [TERRAIN_EFFECT_MUDSPORT] = terrainEffect(mudsport),
    [TERRAIN_EFFECT_WATERSPORT] = terrainEffect(watersport),
    [TERRAIN_EFFECT_GRAVITY] = terrainEffect(gravity),
    [TERRAIN_EFFECT_GRASSY_TERRAIN] = terrainEffect(grassyTerrain),
    [TERRAIN_EFFECT_MISTY_TERRAIN] = terrainEffect(mistyTerrain),
    [TERRAIN_EFFECT_ELECTRIC_TERRAIN] = terrainEffect(electricTerrain),
    [TERRAIN_EFFECT_PSYCHIC_TERRAIN] = terrainEffect(psychicTerrain),
    [TERRAIN_EFFECT_FAIRY_LOCK] = terrainEffect(fairyLock),
    [TERRAIN_EFFECT_CRITICAL_HIT_BOOST] = terrainEffect(criticalHitBoost),
    [TERRAIN_EFFECT_CONFUSION] = terrainEffect(confusion),
    [TERRAIN_EFFECT_INFATUATION] = terrainEffect(infatuation),
    [TERRAIN_EFFECT_NIGHTMARE] = terrainEffect(nightmare),
    [TERRAIN_EFFECT_DROWSY] = terrainEffect(drowsy),
    [TERRAIN_EFFECT_ENCORE] = terrainEffect(encore),
    [TERRAIN_EFFECT_NO_ABILITY] = terrainEffect(noAbility),
    [TERRAIN_EFFECT_TORMENT] = terrainEffect(torment),
    [TERRAIN_EFFECT_TAILWIND] = terrainEffect(tailwind),
    [TERRAIN_EFFECT_LUCKY_CHANT] = terrainEffect(luckyChant),
    [TERRAIN_EFFECT_GRUDGE] = terrainEffect(grudge),
    [TERRAIN_EFFECT_HEAL_BLOCK] = terrainEffect(healBlock),
    [TERRAIN_EFFECT_IDENTIFIED] = terrainEffect(identified),
    [TERRAIN_EFFECT_MOVE_DISABLED] = terrainEffect(moveDisabled),
    [TERRAIN_EFFECT_CANT_ESCAPE] = terrainEffect(cantEscape),
    [TERRAIN_EFFECT_LOCK_ON] = terrainEffect(lockOn),
    [TERRAIN_EFFECT_EMBARGO] = terrainEffect(embargo),
    [TERRAIN_EFFECT_CHARGE] = terrainEffect(charge),
    [TERRAIN_EFFECT_MIST] = terrainEffect(mist),
    [TERRAIN_EFFECT_SAFEGUARD] = terrainEffect(safeguard),
    [TERRAIN_EFFECT_STEALTH_ROCK] = terrainEffect(stealthRock),
    [TERRAIN_EFFECT_STOCKPILE] = terrainEffect(stockpile),
    [TERRAIN_EFFECT_TAUNT] = terrainEffect(taunt),
    [TERRAIN_EFFECT_TELEKINESIS] = terrainEffect(telekinesis),
    [TERRAIN_EFFECT_MAGNET_RISE] = terrainEffect(magnetRise),
    [TERRAIN_EFFECT_TOXIC_SPIKES] = terrainEffect(toxicSpikes),
    [TERRAIN_EFFECT_WISH] = terrainEffect(wish),
    [TERRAIN_EFFECT_STICKY_WEB] = terrainEffect(stickyWeb),
    [TERRAIN_EFFECT_INGRAIN] = terrainEffect(ingrain),
    [TERRAIN_EFFECT_CURSE] = terrainEffect(curse),
    [TERRAIN_EFFECT_TRICK_OR_TREAT] = terrainEffect(trickOrTreat),              // Not used yet
    [TERRAIN_EFFECT_LIGHTSCREEN] = terrainEffect(lightscreen),
    [TERRAIN_EFFECT_REFLECT] = terrainEffect(reflect),
    [TERRAIN_EFFECT_IMPRISON] = terrainEffect(imprison),
    [TERRAIN_EFFECT_COUNTING_DOWN] = terrainEffect(countingDown),
    [TERRAIN_EFFECT_SPIKES] = terrainEffect(spikes),
    [TERRAIN_EFFECT_DESTINY_BOND] = terrainEffect(destinyBond),
    [TERRAIN_EFFECT_FORESTS_CURSE] = terrainEffect(forestsCurse),               // Not used yet
    [TERRAIN_EFFECT_LEECHSEED] = terrainEffect(leechseed),
    [TERRAIN_EFFECT_LOCKED_ON] = terrainEffect(lockedOn),
    [TERRAIN_EFFECT_BOUND] = terrainEffect(bound),
    [TERRAIN_EFFECT_BIDE] = terrainEffect(bide),
    [TERRAIN_EFFECT_OUTRAGE] = terrainEffect(outrage),
    [TERRAIN_EFFECT_RAMPAGING] = terrainEffect(rampaging),
    [TERRAIN_EFFECT_PETAL_DANCE] = terrainEffect(petalDance),
    [TERRAIN_EFFECT_BADLY_POISONED] = terrainEffect(badlyPoisoned),
    [TERRAIN_EFFECT_FUTUREATTACK] = terrainEffect(futureattack),
    [TERRAIN_EFFECT_UPROAR] = terrainEffect(uproar),
    [TERRAIN_EFFECT_AQUA_RING] = terrainEffect(aquaRing),
    [TERRAIN_EFFECT_AUTOTOMIZE] = terrainEffect(autotomize),
    [TERRAIN_EFFECT_SMACK_DOWN] = terrainEffect(smackDown),
    [TERRAIN_EFFECT_RAINBOW] = terrainEffect(rainbow),                          // Not used yet
    [TERRAIN_EFFECT_SWAMP] = terrainEffect(swamp),                              // Not used yet
    [TERRAIN_EFFECT_SEAOF_FIRE] = terrainEffect(seaofFire),                     // Not used yet
    [TERRAIN_EFFECT_STRONG_WINDS] = terrainEffect(strongWinds),                 // Not used yet
    [TERRAIN_EFFECT_THROAT_CHOP] = terrainEffect(throatChop),
    [TERRAIN_EFFECT_AURORA_VEIL] = terrainEffect(auroraVeil),
    [TERRAIN_EFFECT_LASER_FOCUS] = terrainEffect(laserFocus),
    [TERRAIN_EFFECT_TAR_SHOT] = terrainEffect(tarShot),                         // Not used yet?
    [TERRAIN_EFFECT_OCTOLOCK] = terrainEffect(octolock),                        // Not used yet?
    [TERRAIN_EFFECT_G_MAX_WILDFIRE] = terrainEffect(gMaxWildfire),              // Not used yet
    [TERRAIN_EFFECT_G_MAX_VOLCALITH] = terrainEffect(gMaxVolcalith),            // Not used yet
    [TERRAIN_EFFECT_G_MAX_STEELSURGE] = terrainEffect(gMaxSteelsurge),          // Not used yet
    [TERRAIN_EFFECT_G_MAX_VINE_LASH] = terrainEffect(gMaxVineLash),             // Not used yet
    [TERRAIN_EFFECT_G_MAX_CANNONADE] = terrainEffect(gMaxCannonade),            // Not used yet
};

struct terrain_effect GetTerrainEffect(u8 terrainId)
{
    return sTerrainEffects[terrainId];
}



#define howManyTerrainEffects 4
#define setTerrainEffect(__WHICH_EFFECT__) \
    curTerrainEffects[curIndex] = __WHICH_EFFECT__; \
    logUnsigned(curIndex);\
    logUnsigned(curTerrainEffects[curIndex]);\
    if(++curIndex == howManyTerrainEffects) \
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


void GetTerrainEffectDescriptions(u8 monIndex, u8*curTerrainEffects)
{
    logDebug("GetTerrainEffectDescriptions");
    u8 curIndex = 0;
    u8 i;
    u32 sideStatus = gSideStatuses[GET_BATTLER_SIDE(monIndex)];

    // Null out the existing array
    for (i = 0; i < howManyTerrainEffects; i++)
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
            if (++curIndex == howManyTerrainEffects)
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