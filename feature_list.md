FEATURE LIST
===

# MAJOR EXPANSIONS
## Battle Engine Expansion
* Updates moves (including those from modern games
* Adds the physical special split
* Better handle various moves

## Pokemon Expansion
* Adds all pokemon through G8
* Adds additional forms to Pokemon
* Adds Mega-evolutions

## Item Expansion
* Modern items (Through G8)
* Update older items to act like modern games

## Improved Trainer Code
* Cleans up trainer code so it's easier to edit
* Adds extra features to trainers to make battles special
* SHINEY POKEMANZ OH EM GEE

## DexNav
* Port the DexNav from the remake versions of the game

## PokedexPlus
* Upgraded pokedex based on HG/SS
* Has a dark mode
* Lets you see every move the pokemon can learn
* Lets you see the different forms a pokemon can have
* Tells you how the pokemon can evolve

## Quests
* Lets you have "quests" in the game
* On finishing quests, players can be rewarded with items
* https://www.pokecommunity.com/showthread.php?t=434462

## Improved Option List
* Adds a better option list with more options
* Adds a mute option
* Adds an instant text function
* Adds an HP Bar speed option
* Adds an Exp bar speed option
* Adds an option to swap between imperial and metric units


# CORE GAME
## Cleaned Up the Save Ram
* Removed needless parts of Save Ram so other features can save data.

## Remove Japanese Grammer Tables (FEATURE_REMOVEJPNGRAMMERTABLES)
* Removes grammar tables and related function checks that aren't needed in the english version of the game

## Reset Size Function (FEATURE_RESETSIZEFUNCTION)
* Re-implements the otherwise missing `RESET_SIZE` text function

## Improved V-Blank (FEATURE_IMPROVEDVBLANK)
* Improves the vblank to properly put the CPU in low power mode while waiting for v-blank

## Save Memory Footer Changes (FEATURE_SAVEMEMORYFOOTER)
* Shrinks the save footer to exactly the size it needs to be, saving room in sram

## Fix Free Camera Interactions (FEATURE_FIXFREECAMINTERACTIONS)
* Fixes issues where the free-cam will cause water to ripple and grass to rustle

## Remove Union Room Code (FEATURE_REMOVEUNIONROOM)
* Removes the union room checks, since the wireless adapter can't be emulated and it causes pauses in-game (You also need to undo script changes listed here: https://github.com/pret/pokeemerald/wiki/Disabling-Union-Room-check-when-entering-Pok%C3%A9mon-Centers.)

## Add Check for Pokemon Species Script Command (FEATURE_CHECKFORPOKEMONSPECIESSCRIPT)
* Adds a new script command that will check if a particular pokemon species is in the party (by Vexio)
* https://www.pokecommunity.com/showpost.php?p=10213715&postcount=167

## Remove Pokemon Encryption (FEATURE_REMOVEPOKEENCRYPTION)
* Removes the encryption on pokemon data
* https://www.pokecommunity.com/showthread.php?p=10114674#post10114674

## Move Item Between Pokemon (FEATURE_MOVEITEMBETWEENPOKEMON)
* Move a held item between two Pokemon without needing to put things back in the bag
* https://www.pokecommunity.com/showthread.php?p=10120157#post10120157

## Portable PC (FEATURE_PORTABLEPC)
* Allow use of the PC from AAAAAAAAAAANYWHERE (via the main menu)
* https://www.pokecommunity.com/showpost.php?p=10127351&postcount=48

## Add `locktarget` Script Command (FEATURE_ADDLOCKTARGET)
* Adds a `locktarget` command that only locks the player down

## Display Mugshots / Other Images in the Overworld
* Lets you show pics when in the overworld.
* https://www.pokecommunity.com/showthread.php?p=10345947#post10345947

## Enable the Reset RTC Feature (FEATURE_ENABLERESETTRC)
* Enable the existing RTC Reset feature

## Dynamic Overworld Palettes (FEATURE_DYNAMICOVERWORLDPALETTES)
* Enables dynamic overworld palettes

## Implementing the “textcolor” script command from FRLG (FEATURE_FRLGTEXTCOLOR)
* Re-implements the textcolor script command from FRLG
* https://github.com/pret/pokeemerald/wiki/Implementing-the-%E2%80%9Ctextcolor%E2%80%9D-script-command-from-FRLG-and-give-object-events-their-own-text-colour

## Uniquely Shuffle Array (FEATURE_UNIQUELYSHUFFLEARRAY)
* Adds a method to uniquely shuffle arrays
* https://github.com/pret/pokeemerald/wiki/Uniquely-Shuffle-Array

## Spaw Invisible Player Flag(FEATURE_SPAWNINVISIBLEPLAYER)
* Lets you spawn the player as invisible, useful for cut scenes and the like
* When you want the player to appear again, you must use the applymovement command that includes a set_visible movement command.
* https://github.com/pret/pokeemerald/wiki/Spawn-Invisible-Player



# BASE GAME
## Alt Default Options (FEATURE_ALTDEFAULTOPTIONS)
* Changes default settings
  * optionsTextSpeed = ~~OPTIONS_TEXT_SPEED_MID~~=>**OPTIONS_TEXT_SPEED_FAST**
  * optionsSound = ~~OPTIONS_SOUND_MONO~~=>**OPTIONS_SOUND_STEREO**

## Pokemon Center Disregard Eggs (FEATURE_POKECENTERSDISREGARDEGGS)
* Make pokecenters disregard eggs during healing animations

## Lowercase After First Character Picked (FEATURE_LOWERCASEAFTERFIRSTCHAR)
* After the first character is picked, auto select the lowercase keyboard

## Don't Show Pokedex Entries Until Pokedex (FEATURE_DONTSHOWENTRIESUNTILPOKEDEX)
* Prevents the pokedex entries showing until the player actually has the dex

## Stair Warps (FEATURE_STAIRWARPS)
* Re-adds the FR/LG style stairs that warp you to new maps

## Allow Rotom to Change Form at Birch's Lab (FEATURE_ROTOMCHANGEFORM)
* Allows for Rotom's form to be changed via a box in Birch's Lab

## Expanded Money Cap (FEATURE_SETMONEYCAP) (FEATURE_MAXMONEY)
* Mo' money, mo' problems
* Set `FEATURE_SETMONEYCAP` to the number of digits you want the cap to be at
* Set `FEATURE_MAXMONEY` to the literal money cap
* Make sure the two aggree or else shit'll get weird

## Allow Rotom to Change Form at Professor Cozmo's House (FEATURE_DEOXYSCHANGEFORM)
* Allows for Deoxys' form to be changed in the field as per future games in the series
* https://www.pokecommunity.com/showpost.php?p=10259063&postcount=197

## Shuckle makes Berry Juice (FEATURE_SHUCKLEBERRYJUICE)
* Allows Shuckle to make Berry Juice when they hold a berry
* https://github.com/pret/pokeemerald/wiki/Shuckle-makes-Berry-Juice

## Allow Trade with FR/LG from Default (FEATURE_DEFAULTFRLGTRADE)
* Allow for trading with FRLG from the start of the game


# MENUS
## Remove the Contest Tab (FEATURE_REMOVECONTESTTAB)
* Removes the contest tab from the Pokemon Summary screens

## Color the Stat Effect of Nature (FEATURE_DIZZYNATURECOLORMOD)
* Enables Dizzy's nature mod which highlights the effect of natures on a pokemon's stats

## Show EVs / IVs in Pokemon Summary Screen (FEATURE_SHOWEVIVINSTATSCREEN)
* Let the player view their pokemon's EVs and IVs in the stat screen

## Make the Summary Screen Wrap (FEATURE_WRAPPINGSUMMARYSCREEN)
* When you reach the end of the summary screen, you will wrap back to the first/last pokemon

## Don't Ask to Overwrite Savegame (FEATURE_NOOVERWRITEWARNING)
* Don't prompt the user with `There is already a save file` confirmation screen when saving

## Let Pokemon Forget Any Move (FEATURE_FORGETANYMOVE)
* Let Pokemon forget any move, such as HM moves
* https://www.pokecommunity.com/showpost.php?p=10178662&postcount=110

## Let Players Set the Pokedex's Look (FEATURE_CHANGEDEXLOOK)
* You can change the look of the Pokedex; either Light or Dark mode.

## Hidden Power Type in Summary Screen (FEATURE_HIDDENPOWERTYPEINSUMMARYSCREEN)
* Shows what type Hidden Power is on the Summary Screen

## Pokemon Nickname via the Menu (FEATURE_NICKNAMEPOKEMONINMENU)
* Lets you change a Pokemon's nickname via the menu


# ITEMS
## Item Descriptions on Pick-Up
* A pop-up will show the first time you get an item to show what the item does
* On picking up items after the first time, the picture of the item is shown in the "stored in the bag" message box

## Sortable Bag
* Allows the bag to be sorted via the start button

## Use Med Items without Returning to the Bag (FEATURE_MULTIUSEMEDITEMS)
* Doesn't return to the bag when you use some medical items / rare candies

## Added a Proper Plural Give Item Function (FEATURE_PROPERPLURALGIVEITEM)
* Implements a proper plural give item script, to put the "s" at the end of the item

## Don't Consume TMs (FEATURE_DONTCONSUMETMS)
* Treat TMs the way future games will; basically as HMs

## Modern Style Everstone Nature Passing (FEATURE_MODERNEVERSTONEBREEDING)
* Updates the way nature passing works when pokemon hold an everstone

## LGPE-style Premier Balls (FEATURE_EXTRAPREMIERBALLS)
* Buying n*10 balls will get you n free premier balls

## Check How Much Ash Collected (FEATURE_CHECKASHCOLLECTED)
* Checking how much ash you collected with the Soot Sack

## Mysterious Cable (FEATURE_OLDCABLE)
* An item to replace all trade based evolutions
* Doesn't replace them; just lets you also use this item instead


# BATTLE
## Make Field Snow Cause Hail (FEATURE_SNOWCAUSESHAIL)
* Allow areas with snow to cause hail

## Whiteouts Uses Modern Money Loss Formula(FEATURE_MODERNWHITEOUTMONEYLOSS)
* Changes the amount of money lost on whiteout to be more like modern pokemon games (Also modify `battle_scripts_1.s`)

## Gen6 Style Exp. Share (FEATURE_MODERNEXPSHARE)
* Updates EXP SHARE to behave like it does post G6

## Allow More Trainers to have Intro Mug-Shots(FEATURE_CUSTOMBATTLEMUGSHOTS)
* Trainers outside the elite 4 can have battle mugshots (https://github.com/pret/pokeemerald/wiki/Custom-Battle-Mugshots)

## ~~Make HP Drain Faster(FEATURE_FASTERHPDRAINRATE [40])~~
* When defined, this will use an alternate hp drain formula.
* The smaller the value, the faster it will drain.
* 48 is aprox. the speed of vanilla.

## Set What Poké Ball Individual Trainer Uses(FEATURE_PERTRAINERPOKEBALL)
* Allow the pokeball used by a trainer to be set on a per-trainer basis
* https://www.pokecommunity.com/showpost.php?p=10050958&postcount=25

## Trainer class-based Poke Balls (FEATURE_TRAINERCLASSPOKEBALL)
* Give all trainers of a class a new default ball type

## Show Type Effectiveness In-Battle (FEATURE_SHOWTYPEEFFECTIVENESSINBATTLE)
* Changes the color of the move's type to alert players if it's super / not very effective

## Hidden Power Type Shown in Battle Screen (FEATUER_HIDDENPOWERTYPEINBATTLESCREEN)
* Shows what type Hidden Power is on the battle screen

## Weight AI Against Non-Effective Moves (FEATURE_WEIGHTAIAGAINSTNONEFFECTIVEMOVED)
* Re-weights some of the AI logic so it is less ok with non-effective moves


# MOVEMENT
## Auto-Running
* Toggle running on/off, so you don't have to hold a button

## Indoor Running (FEATURE_INDOORRUN)
* Allow running while indoors
* https://www.pokecommunity.com/showpost.php?p=9990848&postcount=14

## Faster Surfing (FEATURE_FASTSURFING)
* Allow players to "run" while swimming, to go at mach bike speed

## Pokemon Will Survive Poison in the Field(FEATURE_NOFIELDPSN)
* Makes it so Pokemon wont die from Poison in the field, they'll overcome it with 1hp

## ~~Pokemon Don't Lose Friendship when they Survive Poison(FEATURE_NOFIELDPSN_DONTLOSEFRIENDSHIP)~~
* Makes it so Pokemon whom "get over" psn in the field don't lose friendship (otherwise, they still lose it as if they had fainted)

## Black and White Repel (FEATURE_MODERNREPEL)
* Asks if you want to re-apply repel whenever one runs out


# BIKE
## Swap Bike Type via a Button (FEATURE_SWAPBIKEBUTTON)
* Makes the bike work like G4, letting you swap modes via the menu

# SHINY
## Allow Shinny Re-Rolls (FEATURE_ALLOWSHINYREROLLS)
* The critical feature; this allows for other features to do "shiny" re-rolling (Without this, the other features in this section wont work).

## Chain Fishing (FEATURE_CHAINFISHING)
* Makes it so that reeling in consecutive pokemon species increases the chance of being shiny

# DEBUG
## Debug Menu (FEATURE_DEBUGMENU)
* Enables the debug menu

## Skip the Main Intro (FEATURE_SKIPINTRO)
* Skips the Rayquaza intro, to speed up debugging

## Enable mGBA Print (FEATURE_MGBAPRINT)
* Enables mGBA printf