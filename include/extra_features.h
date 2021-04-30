#ifndef EXTRA_FEATURES_H
#define EXTRA_FEATURES_H

// Extra helpful functions (ONLY USE WITH EXTRA FEATURES)
#define EXM_FLASH_ACTIVE Overworld_GetFlashLevel() > 1 ? TRUE : FALSE

// MAJOR EXPANSION BRANCHES
// Branch defines: Used by other branches to detect each other.
// Each define must be here for each of RHH's branch you have pulled.
// e.g. If you have both the battle_engine and pokemon_expansion branch,
//      then both BATTLE_ENGINE and POKEMON_EXPANSION must be defined here.
#define BATTLE_ENGINE
#define POKEMON_EXPANSION
#define ITEM_EXPANSION

// HARDBAKED FEATURES
// * Extra SaveRam space
// * DexNav feature
// * Bag can be sorted
// * Auto-Run on clicking the R button
// * Item descriptions on pick-up
// * Trainers can run a script on sight (https://github.com/pret/pokeemerald/wiki/Trainer-Scripts)
// * PokedexPlus (https://www.pokecommunity.com/showthread.php?t=441996)
// * Quest Menu (https://www.pokecommunity.com/showthread.php?t=434462)
// FLAGS

// CORE GAME FEATURES - Behind the game improvements
#define FEATURE_REMOVEJPNGRAMMERTABLES  // Removes grammar tables and related function checks that aren't needed in the english version of the game
#define FEATURE_RESETSIZEFUNCTION  // Re-implements the otherwise missing `RESET_SIZE` text function
#define FEATURE_IMPROVEDVBLANK  // Improves the vblank to properly put the CPU in low power mode while waiting for v-blank
#define FEATURE_SAVEMEMORYFOOTER  // Shrinks the save footer to exactly the size it needs to be, saving room in sram
#define FEATURE_FIXFREECAMINTERACTIONS  // Fixes issues where the free-cam will cause water to ripple and grass to rustle
#define FEATURE_REMOVEUNIONROOM  // Removes the union room checks, since the wireless adapter can't be emulated and it causes pauses in-game (You also need to undo script changes listed here: https://github.com/pret/pokeemerald/wiki/Disabling-Union-Room-check-when-entering-Pok%C3%A9mon-Centers.)
#define FEATURE_CHECKFORPOKEMONSPECIESSCRIPT  // Adds a new script command that will check if a particular pokemon species is in the party (by Vexio)
#define FEATURE_REMOVEPOKEENCRYPTION  // Removes the encryption on pokemon data

// BASE GAME FEATURES - Tweaks to the general game
#define FEATURE_ALTDEFAULTOPTIONS  // Changes default settings
#define FEATURE_POKECENTERSDISREGARDEGGS  // Make pokecenters disregard eggs during healing animations
#define FEATURE_LOWERCASEAFTERFIRSTCHAR  // After the first character is picked, auto select the lowercase keyboard
#define FEATURE_DONTSHOWENTRIESUNTILPOKEDEX  // Prevents the pokedex entries showing until the player actually has the dex
#define FEATURE_STAIRWARPS  // Re-adds the FR/LG style stairs that warp you to new maps
#ifdef FEATURE_CHECKFORPOKEMONSPECIESSCRIPT
#define FEATURE_ROTOMCHANGEFORM  // Allows for Rotom's form to be changed via a box in Birch's Lab (Depends on FEATURE_CHECKFORPOKEMONSPECIESSCRIPT)
#endif

// MENU FEATURES - Changes to how various menus work
#define FEATURE_REMOVECONTESTTAB // Removes the contest tab
#define FEATURE_DIZZYNATURECOLORMOD  // Enables Dizzy's nature mod which highlights the effect of natures on a pokemon's stats
#define FEATURE_SHOWEVIVINSTATSCREEN  // Let the player view their pokemon's EVs and IVs in the stat screen
#define FEATURE_WRAPPINGSUMMARYSCREEN  // When you reach the end of the summary screen, you will wrap back to the first/last pokemon
#define FEATURE_NOOVERWRITEWARNING  // Don't prompt the user with `There is already a save file` confirmation screen when saving
#define FEATURE_FORGETANYMOVE  // Let Pokemon forget any move, such as HM moves
#define FEATURE_HIDDENPOWERTYPEINSUMMARYSCREEN  // Shows what type Hidden Power is on the Summary Screen
#define FEATURE_CHECKASHCOLLECTED  // Checking how much ash you collected with the Soot Sack
#define FEATURE_MOVEITEMBETWEENPOKEMON  // Move a held item between two Pokemon without needing to put things back in the bag

// ITEM FEATURES - Updates to how items work
#define FEATURE_MULTIUSEMEDITEMS  // Doesn't return to the bag when you use some medical items / rare candies
#define FEATURE_PROPERPLURALGIVEITEM  // Implements a proper plural give item script, to put the "s" at the end of the item
#define FEATURE_DONTCONSUMETMS  // Treat TMs the way future games will; basically as HMs
#define FEATURE_MODERNEVERSTONEBREEDING  // Updates the way nature passing works when pokemon hold an everstone
#define FEATURE_EXTRAPREMIERBALLS  // LGPE-style Premier Ball giveaway; buying n*10 balls will get you n free premier balls

// BATTLE FEATURES - Mods to battles
#define FEATURE_SNOWCAUSESHAIL  // Allow areas with snow to cause hail
#define FEATURE_MODERNWHITEOUTMONEYLOSS  // Changes the amount of money lost on whiteout to be more like modern pokemon games (Also modify `battle_scripts_1.s`)
#define FEATURE_MODERNEXPSHARE  // Updates EXP SHARE to behave like it does post G6
#define FEATURE_CUSTOMBATTLEMUGSHOTS  // Trainers outside the elite 4 can have battle mugshots (https://github.com/pret/pokeemerald/wiki/Custom-Battle-Mugshots)
// #define FEATURE_FASTERHPDRAINRATE 40  // When defined, this will use an alternate hp drain formula. The smaller the value, the faster it will drain. 48 is aprox. the speed of vanilla. (The better option menu makes this redundent)
#define FEATURE_PERTRAINERPOKEBALL  // Allow the pokeball used by a trainer to be set on a per-trainer basis
#define FEATURE_TRAINERCLASSPOKEBALL  // Give all trainers of a class a new default ball type
#define FEATURE_SHOWTYPEEFFECTIVENESSINBATTLE  // Changes the color of the move's type to alert players if it's super / not very effective
#define FEATUER_HIDDENPOWERTYPEINBATTLESCREEN  // Shows what type Hidden Power is on the battle screen

// MOVEMENT FEATURES - Move your body freely
#define FEATURE_INDOORRUN  // Allow running while indoors
#define FEATURE_FASTSURFING  // Allow players to "run" while swimming, to go at mach bike speed
#define FEATURE_NOFIELDPSN  // Makes it so Pokemon wont die from Poison in the field, they'll overcome it with 1hp
// #define FEATURE_NOFIELDPSN_DONTLOSEFRIENDSHIP  // Makes it so Pokemon whom "get over" psn in the field don't lose friendship (otherwise, they still lose it as if they had fainted)
#define FEATURE_MODERNREPEL  // Asks if you want to re-apply repel whenever one runs out

// BIKE RELATED - I want to ride my bike
#define FEATURE_SWAPBIKEBUTTON  // Makes the bike work like G4, letting you swap modes via the menu
#define EXM_PLAYER_ON_BIKE gPlayerAvatar.flags & (PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE)  // Macro that does the check for the bike

// SHINY RELATED - Make shiny pokemon work differently
#define FEATURE_ALLOWSHINYREROLLS  // The critical feature; this allows for other features to do "shiny" re-rolling. Without this, the other features in this section wont work
#define FEATURE_CHAINFISHING  // Makes it so that reeling in consecutive pokemon species increases the chance of being shiny

// DEBUG RELATED - HELLO, I AM ERROR
#define FEATURE_DEBUGMENU  // Enables debug mode
#define FEATURE_SKIPINTRO  // Skips the Rayquaza intro, to speed up debugging
#define FEATURE_MGBAPRINT  // Enables mGBA printf

#endif