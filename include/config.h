#ifndef GUARD_CONFIG_H
#define GUARD_CONFIG_H

// In the Generation 3 games, Asserts were used in various debug builds.
// Ruby/Sapphire and Emerald do not have these asserts while Fire Red
// still has them in the ROM. This is because the developers forgot
// to define NDEBUG before release, however this has been changed as
// Ruby's actual debug build does not use the AGBPrint features.
#define NDEBUG

// To enable print debugging, comment out "#define NDEBUG". This allows
// the various AGBPrint functions to be used. (See include/gba/isagbprint.h).
// Some emulators support a debug console window: uncomment NoCashGBAPrint()
// and NoCashGBAPrintf() in libisagbprn.c to use no$gba's own proprietary
// printing system. Use NoCashGBAPrint() and NoCashGBAPrintf() like you
// would normally use AGBPrint() and AGBPrintf().

#define ENGLISH

#ifdef ENGLISH
#define UNITS_IMPERIAL
#else
#define UNITS_METRIC
#endif

// Uncomment to fix some identified minor bugs
#define BUGFIX

// EXTRA FEATURES
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
// VARS
#define FEATURE_FASTERHPDRAINRATE 40  // When defined, this will use an alternate hp drain formula. The smaller the value, the faster it will drain. 48 is aprox. the speed of vanilla.
// END EXTRA FEATURES

// Various undefined behavior bugs may or may not prevent compilation with
// newer compilers. So always fix them when using a modern compiler.
#if MODERN || defined(BUGFIX)
#ifndef UBFIX
#define UBFIX
#endif
#endif

#endif // GUARD_CONFIG_H
