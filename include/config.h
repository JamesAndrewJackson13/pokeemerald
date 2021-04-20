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
#define FEATURE_SNOWCAUSESHAIL // Allow areas with snow to cause hail
#define FEATURE_POKECENTERSDISREGARDEGGS // Make pokecenters disregard eggs during healing animations
#define FEATURE_LOWERCASEAFTERFIRSTCHAR  // After the first character is picked, auto select the lowercase keyboard
#define FEATURE_REMOVECONTESTTAB // Removes the contest tab
#define FEATURE_INDOORRUN // Allow running while indoors
#define FEATURE_MODERNWHITEOUTMONEYLOSS // Changes the amount of money lost on whiteout to be more like modern pokemon games (Also modify `battle_scripts_1.s`)
#define FEATURE_IMPROVEDVBLANK // Improves the vblank to properly put the CPU in low power mode while waiting for v-blank
#define FEATURE_FIXFREECAMINTERACTIONS // Fixes issues where the free-cam will cause water to ripple and grass to rustle

// Various undefined behavior bugs may or may not prevent compilation with
// newer compilers. So always fix them when using a modern compiler.
#if MODERN || defined(BUGFIX)
#ifndef UBFIX
#define UBFIX
#endif
#endif

#endif // GUARD_CONFIG_H
