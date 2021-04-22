// HARDBAKED FEATURES
// * Extra SaveRam space
// * DexNav feature
// * Bag can be sorted
// * Auto-Run on clicking the R button
// FLAGS
#define FEATURE_SAVEMEMORYFOOTER  // Shrinks the save footer to exactly the size it needs to be, saving room in sram
#define FEATURE_SNOWCAUSESHAIL  // Allow areas with snow to cause hail
#define FEATURE_POKECENTERSDISREGARDEGGS  // Make pokecenters disregard eggs during healing animations
#define FEATURE_LOWERCASEAFTERFIRSTCHAR  // After the first character is picked, auto select the lowercase keyboard
#define FEATURE_REMOVECONTESTTAB // Removes the contest tab
#define FEATURE_INDOORRUN  // Allow running while indoors
#define FEATURE_MODERNWHITEOUTMONEYLOSS  // Changes the amount of money lost on whiteout to be more like modern pokemon games (Also modify `battle_scripts_1.s`)
#define FEATURE_IMPROVEDVBLANK  // Improves the vblank to properly put the CPU in low power mode while waiting for v-blank
#define FEATURE_FIXFREECAMINTERACTIONS  // Fixes issues where the free-cam will cause water to ripple and grass to rustle
#define FEATURE_DONTSHOWENTRIESUNTILPOKEDEX  // Prevents the pokedex entries showing until the player actually has the dex
#define FEATURE_REMOVEUNIONROOM  // Removes the union room checks, since the wireless adapter can't be emulated and it causes pauses in-game (You also need to undo script changes listed here: https://github.com/pret/pokeemerald/wiki/Disabling-Union-Room-check-when-entering-Pok%C3%A9mon-Centers.)
#define FEATURE_REMOVEJPNGRAMMERTABLES  // Removes grammar tables and related function checks that aren't needed in the english version of the game
#define FEATURE_DONTCONSUMETMS  // Treat TMs the way future games will; basically as HMs
#define FEATURE_RESETSIZEFUNCTION  // Re-implements the otherwise missing `RESET_SIZE` text function
#define FEATURE_PROPERPLURALGIVEITEM  // Implements a proper plural give item script, to put the "s" at the end of the item
#define FEATURE_MULTIUSEMEDITEMS  // Doesn't return to the bag when you use some medical items / rare candies
#define FEATURE_WRAPPINGSUMMARYSCREEN  // When you reach the end of the summary screen, you will wrap back to the first/last pokemon
#define FEATURE_SHOWEVIVINSTATSCREEN  // Let the player view their pokemon's EVs and IVs in the stat screen
#define FEATURE_DIZZYNATURECOLORMOD  // Enables Dizzy's nature mod which highlights the effect of natures on a pokemon's stats
#define FEATURE_NOFIELDPSN  // Makes it so Pokemon wont die from Poison in the field, they'll overcome it with 1hp
// #define FEATURE_NOFIELDPSN_DONTLOSEFRIENDSHIP  // Makes it so Pokemon whom "get over" psn in the field don't lose friendship (otherwise, they still lose it as if they had fainted)
#define FEATURE_FASTSURFING  // Allow players to "run" while swimming, to go at mach bike speed

// SHINY RELATED
#define FEATURE_ALLOWSHINYREROLLS  // The critical feature; this allows for other features to do "shiny" re-rolling. Without this, the other features in this section wont work
#define FEATURE_CHAINFISHING  // Makes it so that reeling in consecutive pokemon species increases the chance of being shiny

// VARS
#define FEATURE_FASTERHPDRAINRATE 40  // When defined, this will use an alternate hp drain formula. The smaller the value, the faster it will drain. 48 is aprox. the speed of vanilla.