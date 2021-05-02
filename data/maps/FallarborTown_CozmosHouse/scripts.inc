FallarborTown_CozmosHouse_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_LOAD
	map_script MAP_SCRIPT_ON_TRANSITION, FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_LOAD:
	goto_if_set FLAG_SHOW_METEORITE, FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_LOAD_1
	setflag FLAG_TEMP_A
	return

FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_LOAD_1:
	setmetatile 9, 4, 0x35B, FALSE
	setmetatile 9, 5, 0x363, TRUE
	return

FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	goto_if_set FLAG_RECEIVED_TM27, FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

FallarborTown_CozmosHouse_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
	setflag FLAG_SHOW_METEORITE
	return


FallarborTown_CozmosHouse_EventScript_ProfCozmo::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM27, FallarborTown_CozmosHouse_EventScript_ProfCozmo_2
	checkitem ITEM_METEORITE, 1
	compare VAR_RESULT, 0
	goto_if_ne FallarborTown_CozmosHouse_EventScript_ProfCozmo_5
	goto FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow
FallarborTown_CozmosHouse_EventScript_ProfCozmo_1:
	release
	end

FallarborTown_CozmosHouse_EventScript_ProfCozmo_2:
	msgbox FallarborTown_CozmosHouse_EventScript_ProfCozmo_Text_0
	goto FallarborTown_CozmosHouse_EventScript_ProfCozmo_1

FallarborTown_CozmosHouse_EventScript_ProfCozmo_5:
	goto FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite
	goto FallarborTown_CozmosHouse_EventScript_ProfCozmo_1


FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow::
	msgbox FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow_Text_0
	return


FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch::
	msgbox FallarborTown_CozmosHouse_EventScript_ProfCozmo_Text_0
	return


FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite::
	goto_if_set FLAG_TEMP_2, FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_2
	goto FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow
	msgbox FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_1, MSGBOX_YESNO
FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_1:
	compare VAR_RESULT, 0
	goto_if_ne FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_5
	setflag FLAG_TEMP_2
	msgbox FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_3
	return

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_2:
	msgbox FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_0, MSGBOX_YESNO
	goto FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_1

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_5:
	msgbox FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_2
	giveitem ITEM_TM27
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_9
FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_8:
	setvar VAR_0x8004, ITEM_METEORITE
	call Common_EventScript_PlayerHandedOverTheItem
	setflag FLAG_RECEIVED_TM27
	goto FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch
	return

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_9:
	goto Common_EventScript_ShowBagIsFull
	goto FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_8


FallarborTown_CozmosHouse_EventScript_CozmosWife::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM27, FallarborTown_CozmosHouse_EventScript_CozmosWife_2
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_CozmosHouse_EventScript_CozmosWife_3
	msgbox FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_2
FallarborTown_CozmosHouse_EventScript_CozmosWife_1:
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmosWife_2:
	msgbox FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_0
	goto FallarborTown_CozmosHouse_EventScript_CozmosWife_1

FallarborTown_CozmosHouse_EventScript_CozmosWife_3:
	msgbox FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_1
	goto FallarborTown_CozmosHouse_EventScript_CozmosWife_1


FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms::
	lock
	checkpartymon SPECIES_DEOXYS, TRUE
	compare VAR_RESULT, 0
	goto_if_ne FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_2
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_12
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_1:
	release
	end

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_2:
	countpartymon SPECIES_DEOXYS, TRUE
	compare VAR_RESULT, 1
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_6
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_1, MSGBOX_YESNO
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_16
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_28:
	special ChoosePartyMon
	waitstate
	compare VAR_0x8004, PARTY_SIZE
	goto_if_gt FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_33
	ispartymonatindexoftype VAR_0x8004, SPECIES_DEOXYS, TRUE
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_40
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_27:
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_28
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_5:
	bufferpartymonnick 0, VAR_0x8004
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_3
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_11:
	multichoice 0, 0, MULTI_DEOXYS_FORMS, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_25
	copyvar VAR_0x8005, VAR_RESULT
	specialvar VAR_RESULT, TryChangeDeoxysForm
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_30
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10:
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_11
	playmoncry SPECIES_DEOXYS, 0
	switch VAR_0x8005
	case 0, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_20
	case 1, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_21
	case 2, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_22
	case 3, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_23
FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_18:
	waitmoncry
	waitbuttonpress
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_1

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_6:
	firstinpartymon SPECIES_DEOXYS, TRUE
	copyvar VAR_0x8004, VAR_RESULT
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_0, MSGBOX_YESNO
	compare VAR_RESULT, 0
	goto_if_eq FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_13
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_5

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_13:
	release
	end

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_16:
	release
	end

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_20:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_8
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_18

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_21:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_9
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_18

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_22:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_10
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_18

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_23:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_11
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_18

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_25:
	release
	end

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_30:
	switch VAR_0x8005
	case 0, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_36
	case 1, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_37
	case 2, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_38
	case 3, FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_39
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_33:
	release
	end

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_36:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_4
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_37:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_5
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_38:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_6
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_39:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_7
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_10

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_40:
	msgbox FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_2
	goto FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_27


FallarborTown_CozmosHouse_EventScript_ProfCozmo_Text_0:
	.string "PROF. COZMO: Oh, I can't believe it.\n"
	.string "This is really, really great!\p"
	.string "This is really going to help my research!$"

FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow_Text_0:
	.string "PROF. COZMO: Oh…\n"
	.string "I never should have let myself be\l"
	.string "conned into telling TEAM MAGMA where\l"
	.string "you can find METEORITES…\p"
	.string "That METEORITE from METEOR FALLS…\n"
	.string "It's never going to be mine now…$"

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_0:
	.string "PROF. COZMO: Please, may I have that\n"
	.string "METEORITE?\p"
	.string "I'm not asking for it for free.\n"
	.string "How about in exchange for this TM?$"

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_1:
	.string "Oh!\n"
	.string "Hah?\p"
	.string "That item…\p"
	.string "Could it be?\p"
	.string "Is it the METEORITE that TEAM MAGMA\n"
	.string "took from METEOR FALLS?\p"
	.string "Please, may I have it?\p"
	.string "I'm not asking for it for free.\n"
	.string "How about in exchange for this TM?$"

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_2:
	.string "PROF. COZMO: This TM, it represents\n"
	.string "my feeling of gratitude.\l"
	.string "Please use it!$"

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite_Text_3:
	.string "PROF. COZMO: Oh, but…\n"
	.string "I'm crushed with disappointment…$"

FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_0:
	.string "Look at PROF. COZMO…\n"
	.string "He's so happy! I think it's cute.$"

FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_1:
	.string "Poor PROF. COZMO…\n"
	.string "He's so depressed… I feel sorry for him$"

FallarborTown_CozmosHouse_EventScript_CozmosWife_Text_2:
	.string "PROF. COZMO went off to METEOR FALLS\n"
	.string "on ROUTE 114 with some people from\l"
	.string "TEAM MAGMA.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_0:
	.string "Deoxys is reacting to the meteorite...\p"
	.string "Would you like to bring the Deoxys in\n"
	.string "your party closer to the meteorite?$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_1:
	.string "Deoxys is reacting to the meteorite...\p"
	.string "Would you like to bring one of the\n"
	.string "Deoxys in your party closer to the\l"
	.string "meteorite?$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_2:
	.string "Please select a Deoxys.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_3:
	.string "Which form should {STR_VAR_1} take?$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_4:
	.string "{STR_VAR_1} is already in Normal\n"
	.string "form.\p"
	.string "Select a new form.\p"
	.string "$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_5:
	.string "{STR_VAR_1} is already in Attack\n"
	.string "form.\p"
	.string "Select a new form.\p"
	.string "$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_6:
	.string "{STR_VAR_1} is already in Defense\n"
	.string "form.\p"
	.string "Select a new form.\p"
	.string "$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_7:
	.string "{STR_VAR_1} is already in Speed\n"
	.string "form.\p"
	.string "Select a new form.\p"
	.string "$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_8:
	.string "{STR_VAR_1} shifted into Normal\n"
	.string "form.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_9:
	.string "{STR_VAR_1} shifted into Attack\n"
	.string "form.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_10:
	.string "{STR_VAR_1} shifted into Defense\n"
	.string "form.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_11:
	.string "{STR_VAR_1} shifted into Speed\n"
	.string "form.$"

FallarborTown_CozmosHouse_EventScript_Deoxys_ChangeForms_Text_12:
	.string "A strange meteorite shines inside the\n"
	.string "case.$"
