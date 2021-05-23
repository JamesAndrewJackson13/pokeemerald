.set LOCALID_RICH_BOY, 1
.set LOCALID_GENTLEMAN, 2
.set LOCALID_SCIENTIST, 3
.set LOCALID_STEVEN, 4
@ Grunt names numbered like their Trainer values (which are for the full Space Center)
.set LOCALID_GRUNT_6, 5
.set LOCALID_GRUNT_5, 6
.set LOCALID_GRUNT_7, 7
.set LOCALID_TABITHA, 8
.set LOCALID_MAXIE, 9
@ Added scientist for the Rotom quest
.set LOCALID_QUESTSCIENTIST, 10

MossdeepCity_SpaceCenter_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MossdeepCity_SpaceCenter_2F_OnFrame
	.byte 0

MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	compare VAR_MOSSDEEP_CITY_STATE, 2
	goto_if_eq MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_2
MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
	compare VAR_MOSSDEEP_SPACE_CENTER_STATE, 2
	goto_if_eq MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_5
MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_4:
	goto_if_set FLAG_QUEST_ROTOM_HAVEBEAT, MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_7
	return

MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_2:
	call MossdeepCity_SpaceCenter_2F_EventScript_MoveCivilians
	goto MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_1

MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_5:
	call MossdeepCity_SpaceCenter_2F_EventScript_MoveDefeatedGrunts
	goto MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_4

MossdeepCity_SpaceCenter_2F_MapScripts_MAP_SCRIPT_ON_TRANSITION_7:
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestHideRotomMachine
	return


MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock::
	waitmessage
	playse SE_PIN
	applymovement LOCALID_QUESTSCIENTIST, Common_Movement_ExclamationMark
	waitmovement LOCALID_QUESTSCIENTIST
	applymovement LOCALID_QUESTSCIENTIST, Common_Movement_Delay48
	waitmovement LOCALID_QUESTSCIENTIST
	faceplayer
	waitmovement LOCALID_QUESTSCIENTIST
	return


MossdeepCity_SpaceCenter_2F_EventScript_QuestHideRotomMachine::
	setmetatile 2, 1, 0x2B8, TRUE
	setmetatile 2, 2, 0x375, FALSE
	return


MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee::
	setflag FLAG_QUEST_ROTOM_HAVEBEAT
	setvar VAR_0x8004, SPECIES_EEVEE
	setvar VAR_0x8005, 15
	setvar VAR_0x8006, ITEM_SILK_SCARF
	call Global_GiftPokemonToPlayer
	compare VAR_RESULT, 1
	goto_if_gt MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_2
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_Text_1
	questmenucompletequest 14
	questmenutoastcompleteshow 14
MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_1:
	return

MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_2:
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_Text_0
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_1


MossdeepCity_SpaceCenter_2F_EventScript_QuestStartRotomFight::
	setfieldeffectargument 0, VAR_0x8004
	dofieldeffect FLDEFF_FIELD_MOVE_SHOW_MON_INIT
	playmoncry SPECIES_PLUSLE, 0
	waitmoncry
	waitfieldeffect FLDEFF_FIELD_MOVE_SHOW_MON
	setfieldeffectargument 0, VAR_0x8005
	dofieldeffect FLDEFF_FIELD_MOVE_SHOW_MON_INIT
	playmoncry SPECIES_MINUN, 0
	waitmoncry
	waitfieldeffect FLDEFF_FIELD_MOVE_SHOW_MON
	bufferpartymonnick 0, VAR_0x8004
	bufferpartymonnick 1, VAR_0x8005
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestStartRotomFight_Text_0
	return


MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base::
	locktarget
	compare VAR_MOSSDEEP_CITY_STATE, 0
	goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_2
	compare VAR_MOSSDEEP_CITY_STATE, 1
	goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_2
	faceplayer
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_19
MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1:
	turnobject LOCALID_QUESTSCIENTIST, DIR_NORTH
	release
	end

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_2:
	questmenucheckunlocked 14
	compare VAR_RESULT, 0
	goto_if_ne MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_7
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_16
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_17
	questmenuunlockquest 14
	questmenutoastunlockshow 14
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_18
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_7:
	questmenucheckcomplete 14
	compare VAR_RESULT, 0
	goto_if_ne MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_10
	goto_if_set FLAG_QUEST_ROTOM_HAVEBEAT, MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_11
	checkpartymon SPECIES_PLUSLE
	copyvar VAR_0x8004, VAR_RESULT
	checkpartymon SPECIES_MINUN
	compare VAR_RESULT, 0
	goto_if_ne MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_17
MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_16:
	goto_if_set FLAG_QUEST_ROTOM_HAVETRIED, MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_24
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_14
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_15
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_10:
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_0
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_1
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_11:
	faceplayer
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_15:
	firstinpartymon SPECIES_PLUSLE
	copyvar VAR_0x8004, VAR_RESULT
	firstinpartymon SPECIES_MINUN
	copyvar VAR_0x8005, VAR_RESULT
	goto_if_set FLAG_QUEST_ROTOM_HAVETRIED, MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_21
	setflag FLAG_QUEST_ROTOM_HAVETRIED
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_5
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_6
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestStartRotomFight
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_7
MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_20:
	setwildbattle SPECIES_ROTOM, 20, ITEM_MAX_ELIXIR
	playmoncry SPECIES_ROTOM, 0
	waitmoncry
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_CAUGHT
	goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_27
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_9
	goto_if_set FLAG_QUEST_ROTOM_HAVEFAILD, MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_31
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_11
MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_30:
	setflag FLAG_QUEST_ROTOM_HAVEFAILD
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_17:
	compare VAR_0x8004, 0
	goto_if_ne MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_15
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_16

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_21:
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_2
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_3
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestStartRotomFight
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_4
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_20

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_24:
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_12
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Shock
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_13
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_27:
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestHideRotomMachine
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_8
	call MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_1

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_31:
	msgbox MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_10
	goto MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_30


@MossdeepCity_SpaceCenter_2F_MapScripts:: @ 8223D58
@    map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_SpaceCenter_2F_OnTransition
@    map_script MAP_SCRIPT_ON_FRAME_TABLE, MossdeepCity_SpaceCenter_2F_OnFrame
@    .byte 0

@MossdeepCity_SpaceCenter_2F_OnTransition: @ 8223D63
@    compare VAR_MOSSDEEP_CITY_STATE, 2
@    call_if_eq MossdeepCity_SpaceCenter_2F_EventScript_MoveCivilians
@    compare VAR_MOSSDEEP_SPACE_CENTER_STATE, 2
@    call_if_eq MossdeepCity_SpaceCenter_2F_EventScript_MoveDefeatedGrunts
@    end

MossdeepCity_SpaceCenter_2F_EventScript_MoveCivilians:: @ 8223D7A
    clearflag FLAG_INTERACTED_WITH_STEVEN_SPACE_CENTER
    setobjectxyperm LOCALID_SCIENTIST, 5, 3
    setobjectmovementtype LOCALID_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
    setobjectxyperm LOCALID_RICH_BOY, 3, 2
    setobjectmovementtype LOCALID_RICH_BOY, MOVEMENT_TYPE_FACE_RIGHT
    setobjectxyperm LOCALID_GENTLEMAN, 7, 3
    setobjectmovementtype LOCALID_GENTLEMAN, MOVEMENT_TYPE_FACE_RIGHT
    setobjectxyperm LOCALID_QUESTSCIENTIST, 1, 3
    setobjectmovementtype LOCALID_QUESTSCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
    return

MossdeepCity_SpaceCenter_2F_EventScript_MoveDefeatedGrunts:: @ 8223D9F
    setobjectxyperm LOCALID_GRUNT_6, 11, 2
    setobjectxyperm LOCALID_GRUNT_7, 15, 2
    setobjectxyperm LOCALID_GRUNT_5, 13, 4
    return

MossdeepCity_SpaceCenter_2F_OnFrame: @ 8223DB5
    map_script_2 VAR_MOSSDEEP_SPACE_CENTER_STATE, 1, MossdeepCity_SpaceCenter_2F_EventScript_ThreeMagmaGrunts
    .2byte 0

MossdeepCity_SpaceCenter_2F_EventScript_ThreeMagmaGrunts:: @ 8223DBF
    playse SE_PIN
    applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
    waitmovement 0
    applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_Delay48
    waitmovement 0
    msgbox MossdeepCity_SpaceCenter_2F_Text_YoureOutnumberedTakeUsOn, MSGBOX_YESNO
    compare VAR_RESULT, YES
    goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_BattleThreeMagmaGrunts
    msgbox MossdeepCity_SpaceCenter_2F_Text_GoodAnswer, MSGBOX_DEFAULT
    closemessage
    applymovement OBJ_EVENT_ID_PLAYER, MossdeepCity_SpaceCenter_2F_Movement_PlayerExit
    waitmovement 0
    warp MAP_MOSSDEEP_CITY_SPACE_CENTER_1F, 255, 13, 1
    waitstate
    releaseall
    end

MossdeepCity_SpaceCenter_2F_Movement_PlayerExit: @ 8223E07
    walk_up
    step_end

MossdeepCity_SpaceCenter_2F_EventScript_BattleThreeMagmaGrunts:: @ 8223E09
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt5Intro, MSGBOX_DEFAULT
    trainerbattle_no_intro TRAINER_GRUNT_SPACE_CENTER_5, MossdeepCity_SpaceCenter_2F_Text_Grunt5Defeat
    applymovement LOCALID_GRUNT_5, MossdeepCity_SpaceCenter_2F_Movement_Grunt5Defeated
    waitmovement 0
    applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFastestLeft
    waitmovement 0
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt6Intro, MSGBOX_DEFAULT
    trainerbattle_no_intro TRAINER_GRUNT_SPACE_CENTER_6, MossdeepCity_SpaceCenter_2F_Text_Grunt6Defeat
    applymovement LOCALID_GRUNT_6, MossdeepCity_SpaceCenter_2F_Movement_Grunt6Defeated
    waitmovement 0
    applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFastestRight
    waitmovement 0
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt7Intro, MSGBOX_DEFAULT
    trainerbattle_no_intro TRAINER_GRUNT_SPACE_CENTER_7, MossdeepCity_SpaceCenter_2F_Text_Grunt7Defeat
    applymovement LOCALID_GRUNT_7, MossdeepCity_SpaceCenter_2F_Movement_Grunt7Defeated
    waitmovement 0
    setvar VAR_MOSSDEEP_SPACE_CENTER_STATE, 2
    copyobjectxytoperm LOCALID_GRUNT_6
    copyobjectxytoperm LOCALID_GRUNT_7
    copyobjectxytoperm LOCALID_GRUNT_5
    releaseall
    end

MossdeepCity_SpaceCenter_2F_Movement_Grunt6Defeated: @ 8223E81
    lock_facing_direction
    walk_left
    unlock_facing_direction
    step_end

MossdeepCity_SpaceCenter_2F_Movement_Grunt5Defeated: @ 8223E85
    lock_facing_direction
    walk_down
    unlock_facing_direction
    step_end

MossdeepCity_SpaceCenter_2F_Movement_Grunt7Defeated: @ 8223E89
    lock_facing_direction
    walk_right
    unlock_facing_direction
    step_end

MossdeepCity_SpaceCenter_2F_EventScript_Scientist:: @ 8223E8D
    lock
    faceplayer
    goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_SpaceCenter_2F_EventScript_ScientistNormal
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_ScientistMagma
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_lt MossdeepCity_SpaceCenter_2F_EventScript_ScientistNormal
    goto MossdeepCity_SpaceCenter_2F_EventScript_ScientistMagma
    end

MossdeepCity_SpaceCenter_2F_EventScript_ScientistNormal:: @ 8223EB4
    msgbox MossdeepCity_SpaceCenter_2F_Text_MossdeepIdealForRockets, MSGBOX_DEFAULT
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_ScientistMagma:: @ 8223EBE
    msgbox MossdeepCity_SpaceCenter_2F_Text_WhyWouldMagmaStealRocketFuel, MSGBOX_DEFAULT
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_Gentleman:: @ 8223EC8
    lock
    faceplayer
    goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_SpaceCenter_2F_EventScript_GentlemanNormal
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_GentlemanMagma
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_lt MossdeepCity_SpaceCenter_2F_EventScript_GentlemanNormal
    goto MossdeepCity_SpaceCenter_2F_EventScript_GentlemanMagma
    end

MossdeepCity_SpaceCenter_2F_EventScript_GentlemanNormal:: @ 8223EEF
    msgbox MossdeepCity_SpaceCenter_2F_Text_WouldveLikedToBeAstronaut, MSGBOX_DEFAULT
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_GentlemanMagma:: @ 8223EF9
    msgbox MossdeepCity_SpaceCenter_2F_Text_MagmaCantGetAwayWithThis, MSGBOX_DEFAULT
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_RichBoy:: @ 8223F03
    lock
    faceplayer
    goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_SpaceCenter_2F_EventScript_RichBoyNormal
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_RichBoyMagma
    compare VAR_MOSSDEEP_CITY_STATE, 2
    goto_if_lt MossdeepCity_SpaceCenter_2F_EventScript_RichBoyNormal
    goto MossdeepCity_SpaceCenter_2F_EventScript_RichBoyMagma
    end

MossdeepCity_SpaceCenter_2F_EventScript_RichBoyNormal:: @ 8223F2A
    msgbox MossdeepCity_SpaceCenter_2F_Text_WishOrdinaryPeopleCouldGoIntoSpace, MSGBOX_DEFAULT
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_RichBoyMagma:: @ 8223F34
    msgbox MossdeepCity_SpaceCenter_2F_Text_DoesMagmaWantToGoToSpace, MSGBOX_DEFAULT
    release
    end

@ Battle for the below 3 grunts is handled in MossdeepCity_SpaceCenter_2F_EventScript_BattleThreeMagmaGrunts
MossdeepCity_SpaceCenter_2F_EventScript_Grunt6:: @ 8223F3E
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt6PostBattle
    end

MossdeepCity_SpaceCenter_2F_EventScript_Grunt7:: @ 8223F47
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt7PostBattle
    end

MossdeepCity_SpaceCenter_2F_EventScript_Grunt5:: @ 8223F50
    msgbox MossdeepCity_SpaceCenter_2F_Text_Grunt5PostBattle
    end

MossdeepCity_SpaceCenter_2F_EventScript_Tabitha:: @ 8223F59
    lock
    faceplayer
    msgbox MossdeepCity_SpaceCenter_2F_Text_WellTakeCareOfYou, MSGBOX_DEFAULT
    applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
    waitmovement 0
    release
    end

MossdeepCity_SpaceCenter_2F_EventScript_Maxie:: @ 8223F6F
    lockall
    msgbox MossdeepCity_SpaceCenter_2F_Text_MaxieDontInterfere, MSGBOX_DEFAULT
    releaseall
    end

MossdeepCity_SpaceCenter_2F_EventScript_Steven:: @ 8223F7A
    lockall
    goto_if_set FLAG_INTERACTED_WITH_STEVEN_SPACE_CENTER, MossdeepCity_SpaceCenter_2F_EventScript_ReadyForBattlePrompt
    setflag FLAG_INTERACTED_WITH_STEVEN_SPACE_CENTER
    msgbox MossdeepCity_SpaceCenter_2F_Text_StevenWhyStealRocketFuel, MSGBOX_DEFAULT
    msgbox MossdeepCity_SpaceCenter_2F_Text_MaxieUseFuelToEruptVolcano, MSGBOX_DEFAULT
    closemessage
    delay 30
    playse SE_NOT_EFFECTIVE
    switch VAR_FACING
    case DIR_SOUTH, MossdeepCity_SpaceCenter_2F_EventScript_StevenFightMovementSouth
    applymovement LOCALID_STEVEN, MossdeepCity_SpaceCenter_2F_Movement_StevenFight
    waitmovement 0
    releaseall
    end

MossdeepCity_SpaceCenter_2F_EventScript_StevenFightMovementSouth:: @ 8223FBA
    applymovement LOCALID_STEVEN, MossdeepCity_SpaceCenter_2F_Movement_StevenFightSouth
    waitmovement 0
    releaseall
    end

MossdeepCity_SpaceCenter_2F_Movement_StevenFight: @ 8223FC6
    lock_facing_direction
    walk_fast_up
    walk_fast_up
    unlock_facing_direction
    delay_16
    delay_16
    walk_slow_down
    walk_slow_down
    step_end

MossdeepCity_SpaceCenter_2F_Movement_StevenFightSouth: @ 8223FCF
    face_left
    lock_facing_direction
    walk_fast_right
    walk_fast_right
    unlock_facing_direction
    delay_16
    delay_16
    walk_slow_left
    walk_slow_left
    face_down
    step_end

MossdeepCity_SpaceCenter_2F_EventScript_ReadyForBattlePrompt:: @ 8223FDA
    applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
    waitmovement 0
    msgbox MossdeepCity_SpaceCenter_2F_Text_StevenAreYouReadyToBattle, MSGBOX_YESNO
    compare VAR_RESULT, YES
    goto_if_eq MossdeepCity_SpaceCenter_2F_EventScript_ChoosePartyForMultiBattle
    msgbox MossdeepCity_SpaceCenter_2F_Text_StevenHurryGetReadyQuickly, MSGBOX_DEFAULT
    closemessage
    applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFastestDown
    waitmovement 0
    releaseall
    end

MossdeepCity_SpaceCenter_2F_EventScript_ChoosePartyForMultiBattle:: @ 822400C
    applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFastestDown
    waitmovement 0
    special SavePlayerParty
    fadescreen FADE_TO_BLACK
    special ChooseHalfPartyForBattle
    waitstate
    compare VAR_RESULT, 0
    goto_if_ne MossdeepCity_SpaceCenter_2F_EventScript_DoStevenMultiBattle
    special LoadPlayerParty
    goto MossdeepCity_SpaceCenter_2F_EventScript_ReadyForBattlePrompt

MossdeepCity_SpaceCenter_2F_EventScript_DoStevenMultiBattle:: @ 8224032
    special ReducePlayerPartyToSelectedMons
    frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
    setvar VAR_0x8004, SPECIAL_BATTLE_STEVEN
    setvar VAR_0x8005, 0
    special DoSpecialTrainerBattle
    waitstate
    frontier_saveparty
    special LoadPlayerParty
    switch VAR_RESULT
    case 1, MossdeepCity_SpaceCenter_2F_EventScript_DefeatedMaxieTabitha
    fadescreen FADE_TO_BLACK
    special SetCB2WhiteOut
    waitstate

MossdeepCity_SpaceCenter_2F_EventScript_DefeatedMaxieTabitha:: @ 8224071
    msgbox MossdeepCity_SpaceCenter_2F_Text_MaxieWeFailedIsAquaAlsoMisguided, MSGBOX_DEFAULT
    closemessage
    delay 20
    applymovement LOCALID_MAXIE, Common_Movement_WalkInPlaceFastestLeft
    applymovement LOCALID_TABITHA, Common_Movement_WalkInPlaceFastestDown
    waitmovement 0
    delay 60
    applymovement LOCALID_MAXIE, Common_Movement_WalkInPlaceFastestUp
    applymovement LOCALID_TABITHA, Common_Movement_WalkInPlaceFastestRight
    waitmovement 0
    delay 20
    msgbox MossdeepCity_SpaceCenter_2F_Text_MaxieWeWillGiveUp, MSGBOX_DEFAULT
    closemessage
    setvar VAR_MOSSDEEP_CITY_STATE, 3
    fadedefaultbgm
    fadescreen FADE_TO_BLACK
    setvar VAR_MOSSDEEP_SPACE_CENTER_STATE, 3
    setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_TEAM_MAGMA
    setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_TEAM_MAGMA
    setflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
    removeobject LOCALID_GRUNT_6
    removeobject LOCALID_GRUNT_7
    removeobject LOCALID_GRUNT_5
    removeobject LOCALID_TABITHA
    removeobject LOCALID_MAXIE
    setobjectxy LOCALID_SCIENTIST, 5, 6
    turnobject LOCALID_SCIENTIST, DIR_SOUTH
    setobjectxy LOCALID_GENTLEMAN, 11, 8
    turnobject LOCALID_GENTLEMAN, DIR_NORTH
    setobjectxy LOCALID_RICH_BOY, 6, 2
    turnobject LOCALID_RICH_BOY, DIR_NORTH
    setobjectxy LOCALID_QUESTSCIENTIST, 2, 2
    turnobject LOCALID_QUESTSCIENTIST, DIR_NORTH
    call MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayer
    fadescreen FADE_FROM_BLACK
    msgbox MossdeepCity_SpaceCenter_2F_Text_StevenThankYouComeSeeMeAtHome, MSGBOX_DEFAULT
    closemessage
    fadescreen FADE_TO_BLACK
    setflag FLAG_DEFEATED_MAGMA_SPACE_CENTER
    setflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_STEVEN
    removeobject LOCALID_STEVEN
    setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_MAGMA_NOTE
    clearflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_STEVEN
    setvar VAR_STEVENS_HOUSE_STATE, 1
    setflag FLAG_HIDE_MOSSDEEP_CITY_SCOTT
    removeobject LOCALID_SCIENTIST
    setobjectxyperm LOCALID_SCIENTIST, 5, 6
    setobjectmovementtype LOCALID_SCIENTIST, MOVEMENT_TYPE_WANDER_AROUND
    addobject LOCALID_SCIENTIST
    fadescreen FADE_FROM_BLACK
    end

MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayer:: @ 8224131
    switch VAR_FACING
    case DIR_SOUTH, MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayerSouth
    case DIR_WEST, MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayerWest
    return

MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayerSouth:: @ 822414D
    turnobject LOCALID_STEVEN, DIR_NORTH
    return

MossdeepCity_SpaceCenter_2F_EventScript_StevenFacePlayerWest:: @ 8224152
    turnobject LOCALID_STEVEN, DIR_EAST
    return

MossdeepCity_SpaceCenter_2F_EventScript_MaxieTrainer:: @ 8224157
    trainerbattle TRAINER_BATTLE_SET_TRAINER_A, TRAINER_MAXIE_MOSSDEEP, 0, MossdeepCity_SpaceCenter_2F_Text_JustWantToExpandLand, MossdeepCity_SpaceCenter_2F_Text_JustWantToExpandLand
    end

MossdeepCity_SpaceCenter_2F_EventScript_TabithaTrainer:: @ 8224166
    trainerbattle TRAINER_BATTLE_SET_TRAINER_B, TRAINER_TABITHA_MOSSDEEP, 0, MossdeepCity_SpaceCenter_Text_TabithaDefeat, MossdeepCity_SpaceCenter_Text_TabithaDefeat
    end

MossdeepCity_SpaceCenter_2F_EventScript_RivalRayquazaCall:: @ 8224175
    lockall
    checkplayergender
    compare VAR_RESULT, MALE
    call_if_eq MossdeepCity_SpaceCenter_2F_EventScript_MayRayquazaCall
    compare VAR_RESULT, FEMALE
    call_if_eq MossdeepCity_SpaceCenter_2F_EventScript_BrendanRayquazaCall
    closemessage
    clearflag FLAG_DEFEATED_MAGMA_SPACE_CENTER
    releaseall
    end

MossdeepCity_SpaceCenter_2F_EventScript_MayRayquazaCall:: @ 8224193
    pokenavcall MatchCall_Text_MayRayquazaCall
    waitmessage
    return

MossdeepCity_SpaceCenter_2F_EventScript_BrendanRayquazaCall:: @ 822419A
    pokenavcall MatchCall_Text_BrendanRayquazaCall
    waitmessage
    return

MossdeepCity_SpaceCenter_2F_Text_MossdeepIdealForRockets: @ 82241A1
    .string "Mossdeep has mainly sunny weather,\n"
    .string "and its winds are stable.\p"
    .string "It's an ideal location for launching\n"
    .string "rockets.$"

MossdeepCity_SpaceCenter_2F_Text_WhyWouldMagmaStealRocketFuel: @ 822420C
    .string "Why would Team Magma want to steal\n"
    .string "our rocket fuel in the first place?$"

MossdeepCity_SpaceCenter_2F_Text_WouldveLikedToBeAstronaut: @ 8224253
    .string "If only I was a little younger, I would\n"
    .string "have liked being an astronaut...\p"
    .string "No... It's not too late!\n"
    .string "I can and will try!$"

MossdeepCity_SpaceCenter_2F_Text_MagmaCantGetAwayWithThis: @ 82242C5
    .string "If Team Magma takes the rocket fuel,\n"
    .string "I won't be able to go to space!\p"
    .string "They can't be allowed to get away\n"
    .string "with such an outrage!$"

MossdeepCity_SpaceCenter_2F_Text_WishOrdinaryPeopleCouldGoIntoSpace: @ 8224342
    .string "I wish ordinary people could go into\n"
    .string "space one day...$"

MossdeepCity_SpaceCenter_2F_Text_DoesMagmaWantToGoToSpace: @ 8224376
    .string "Team Magma...\n"
    .string "Do they want to go to space, too?$"

MossdeepCity_SpaceCenter_2F_Text_YoureOutnumberedTakeUsOn: @ 82243A4
    .string "What's wrong with you?\p"
    .string "You're outnumbered three to one,\n"
    .string "but you still want to take us on?$"

MossdeepCity_SpaceCenter_2F_Text_GoodAnswer: @ 82243FE
    .string "Good answer!\n"
    .string "That's what a smart person'll do!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt5Intro: @ 822442D
    .string "A reckless go-getter, are you?\n"
    .string "Okay, I'll go first!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt5Defeat: @ 8224461
    .string "I lost!\n"
    .string "But!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt6Intro: @ 822446E
    .string "It's too soon to be relieved!\n"
    .string "I'm up next!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt6Defeat: @ 8224499
    .string "I lost, too!\n"
    .string "But!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt7Intro: @ 82244AB
    .string "I bet you want to take a break.\n"
    .string "But I'm not about to let you do that!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt7Defeat: @ 82244F1
    .string "We should've taken a break...\n"
    .string "That's what we should've done...$"

MossdeepCity_SpaceCenter_2F_Text_Grunt6PostBattle: @ 822452C
    .string "Three of us here, and look at the sorry\n"
    .string "mess we got ourselves into.$"

MossdeepCity_SpaceCenter_2F_Text_Grunt7PostBattle: @ 8224570
    .string "We three losing like this...\n"
    .string "We look worse than usual by triple!$"

MossdeepCity_SpaceCenter_2F_Text_Grunt5PostBattle: @ 82245AF
    .string "Are we being useful to our leader\n"
    .string "at all?$"

MossdeepCity_SpaceCenter_2F_Text_WellTakeCareOfYou: @ 82245D9
    .string "Hehehe!\p"
    .string "We come all the way here to get some\n"
    .string "fuel, and we're interfered with again!\p"
    .string "If you're going to mess with us too,\n"
    .string "we'll take care of you at the same time!$"

MossdeepCity_SpaceCenter_2F_Text_MaxieDontInterfere: @ 822467B
    .string "Maxie: Clear out of the way!\n"
    .string "Don't you dare interfere!$"

MossdeepCity_SpaceCenter_2F_Text_StevenWhyStealRocketFuel: @ 82246B2
    .string "Steven: Team Magma...\p"
    .string "What's the point of stealing rocket\n"
    .string "fuel?$"

MossdeepCity_SpaceCenter_2F_Text_MaxieUseFuelToEruptVolcano: @ 82246F0
    .string "Maxie: Fufufu... Since you're so\n"
    .string "curious, you deserve an explanation.\p"
    .string "We're going to jettison the entire\n"
    .string "load into Mt.\pChimney!\p"
    .string "With Groudon gone, we have no need\n"
    .string "for that slag heap of a mountain!\p"
    .string "So we'll use the fuel's power to make\n"
    .string "the volcano erupt!\p"
    .string "It will be savage!$"

MossdeepCity_SpaceCenter_2F_Text_StevenAreYouReadyToBattle: @ 82247FF
    .string "Steven: {PLAYER}{KUN}!\n"
    .string "You're going to help me?\p"
    .string "Let's go into battle together!\n"
    .string "Are you ready?$"

MossdeepCity_SpaceCenter_2F_Text_StevenHurryGetReadyQuickly: @ 8224854
    .string "Steven: Then, hurry!\n"
    .string "Get ready quickly!$"

MossdeepCity_SpaceCenter_2F_Text_JustWantToExpandLand: @ 822487C
    .string "All I want...\n"
    .string "I just want to expand the land mass...$"

MossdeepCity_SpaceCenter_Text_TabithaDefeat: @ 82248AD
    .string "I'm with our leader...$"

MossdeepCity_SpaceCenter_2F_Text_MaxieWeFailedIsAquaAlsoMisguided: @ 82248C2
    .string "Maxie: We failed to make the volcano\n"
    .string "erupt...\p"
    .string "We failed to control Groudon after\n"
    .string "we had awoken it...\p"
    .string "Is our goal to expand the land\n"
    .string "misguided?\p"
    .string "... ... ... ... ... ...\n"
    .string "... ... ... ... ... ...\p"
    .string "If...\n"
    .string "If we, Team Magma, are wrong...\l"
    .string "... ... ... ... ... ...\p"
    .string "Then might Team Aqua's goal to expand\n"
    .string "the sea also be equally misguided?$"

MossdeepCity_SpaceCenter_2F_Text_MaxieWeWillGiveUp: @ 82249DC
    .string "Maxie: All right...\n"
    .string "We will give up on the fuel...\p"
    .string "There appear to be more important\n"
    .string "matters that I must examine...$"

MossdeepCity_SpaceCenter_2F_Text_StevenThankYouComeSeeMeAtHome: @ 8224A4A
    .string "Steven: Whew, that was too tense.\n"
    .string "{PLAYER}{KUN}, thank you.\p"
    .string "I have something to give you as\n"
    .string "thanks for your support.\p"
    .string "Please come see me at home\n"
    .string "after this.\p"
    .string "Oh, yes, I don't live in Rustboro City.\n"
    .string "I live right here on this island.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_Text_0:
	.string "Scientist: Oh, it seems you don't have\n"
	.string "room for anymore pokemon.\p"
	.string "Come back when you have freed space in\n"
	.string "your PC or party.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestGiveEevee_Text_1:
	.string "Scientist: A Pokémaniac researcher\n"
	.string "friend gave {STR_VAR_2} to me as a\l"
	.string "gift.\p"
	.string "But, I don't have the time to take care\n"
	.string "of them.\p"
	.string "I'm certain they'll be much happier with\n"
	.string "you.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestStartRotomFight_Text_0:
	.string "{STR_VAR_1} and {STR_VAR_2}\n"
	.string "shock the machine!$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_0:
	.string "{SIZE 0}...all systems seem to be good.\p"
	.string "Everything is within limits...{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_1:
	.string "Oh! {PLAYER}{KUN}!\n"
	.string "Thanks again for your help.\p"
	.string "I hope you, Rotom, and Eevee are all\n"
	.string "doing well.\p"
	.string "With the computers finally fixed, I have\n"
	.string "a lot of calculations to catch up on.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_2:
	.string "Scientist:{SIZE 0}...I sure hope this whole mess\n"
	.string "doesn't put us too behind schedule...{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_3:
	.string "Oh! Back again!\n"
	.string "Ready to give this another try?\p"
	.string "Lets try the reset again and flush that\n"
	.string "Pokemon out in the open...$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_4:
	.string "Scientist: Here they come!$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_5:
	.string "Scientist:{SIZE 0}“Random overflow translation\n"
	.string "on memory error”?\p"
	.string "What's that supposed to mean...?{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_6:
	.string "Oh! It's you!\n"
	.string "I see you have some Pokémon with you.\p"
	.string "Lets not waste any time, and give this\n"
	.string "reset a try...$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_7:
	.string "Scientist: Aaah! S-S-Something's\n"
	.string "coming out of the server!$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_8:
	.string "Scientist: Ah, finally, the computers\n"
	.string "seem to be returning to normal.\p"
	.string "Thank you so much for helping me out.\p"
	.string "As promised, I have a special Pokémon\n"
	.string "to give you as thanks.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_9:
	.string "The Rotom retreated back into the\n"
	.string "machine.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_10:
	.string "Scientist: Hmmm, retreated again.\p"
	.string "A Pokémon that can control machines...\p"
	.string "I have to admit, there's a lot of\n"
	.string "possibilities there.\p"
	.string "But, that shouldn't include messing\n"
	.string "with the center's computer systems!\p"
	.string "Talk to me whenever you'd like to\n"
	.string "attempt capturing them again.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_11:
	.string "Scientist: My goodness!\p"
	.string "So, a Pokémon was causing all these\n"
	.string "isues?\p"
	.string "I've never heard of such a thing.\p"
	.string "Well, it's gone back into hiding.\p"
	.string "It looks like you'll have to capture\n"
	.string "them to make sure they can't escape\l"
	.string "back into the system.\p"
	.string "Talk to me whenever you'd like to try\n"
	.string "again.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_12:
	.string "Scientist: {SIZE 0}...A Pokémon that lives in\n"
	.string "machines? Facinating...{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_13:
	.string "Oh! Welcome back!\n"
	.string "Are you ready for another try?\p"
	.string "You'll need to get your Plusle and Minum\n"
	.string "in your party first.\p"
	.string "Once you do, come back and speak to me.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_14:
	.string "Scientist: {SIZE 0}...It just doesn't make\n"
	.string "sense...{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_15:
	.string "Oh! It's you!\n"
	.string "Welcome back.\p"
	.string "Have you had any luck finding Pokémon\n"
	.string "that can help?\p"
	.string "Remember, I need one that's positively\n"
	.string "charged and one that's negatively\l"
	.string "charged.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_16:
	.string "Scientist: {SIZE 0}...and I've already tried\n"
	.string "twisting all the knobs.\p"
	.string "If I don't figure this out soon, we'll be\n"
	.string "back to using abacuses and slide\l"
	.string "rulers...{RESET_SIZE}$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_17:
	.string "Oh! A trainer!\n"
	.string "Perfect timing.\p"
	.string "I could really use some help with\n"
	.string "something.\p"
	.string "I'm trying to fix an issue with our\n"
	.string "computer system.\p"
	.string "It just started going haywire today,\n"
	.string "like it has a mind of its own.\p"
	.string "I really need to trip the manual\n"
	.string "overload full system reset circuit.\p"
	.string "But we don't have the necessary\n"
	.string "equipment to induce such a powerful\l"
	.string "electrical current on site.\p"
	.string "Do you have any Pokémon that could\n"
	.string "help?\p"
	.string "The equipment here is very sensitive,\n"
	.string "so it can't just be any electric type.\p"
	.string "They'd need to specifically have a high\n"
	.string "positive and negative charge.$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_18:
	.string "If you find any Pokémon and bring them\n"
	.string "here to help, I can reward you with a\l"
	.string "rare Pokémon!$"

MossdeepCity_SpaceCenter_2F_EventScript_QuestScientist_Base_Text_19:
	.string "Scientist: I wish Team Magma would\n"
	.string "leave us alone\p"
	.string "They're distracting me from my work!$"
