Global_GiftPokemonToPlayer::
	copyvar VAR_TEMP_1, VAR_0x8004
	copyvar VAR_TEMP_2, VAR_0x8005
	copyvar VAR_TEMP_3, VAR_0x8006
	bufferspeciesname 0, VAR_0x8004
	getstorestringvar 0, FALSE
	givemon VAR_0x8004, VAR_0x8005, VAR_0x8006
	compare VAR_RESULT, 1
	goto_if_gt Global_GiftPokemonToPlayer_2
	playfanfare MUS_OBTAIN_ITEM
	msgbox Global_GiftPokemonToPlayer_Text_0
	waitmessage
	waitfanfare
	compare VAR_RESULT, 0
	goto_if_eq Global_GiftPokemonToPlayer_5
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Global_GiftPokemonToPlayer_12
Global_GiftPokemonToPlayer_11:
	call Common_EventScript_TransferredToPC
	setvar VAR_RESULT, 1
Global_GiftPokemonToPlayer_4:
	copyvar VAR_0x8004, VAR_TEMP_1
	copyvar VAR_0x8005, VAR_TEMP_2
	copyvar VAR_0x8006, VAR_TEMP_3
	getstorestringvar 1, TRUE
	return

Global_GiftPokemonToPlayer_2:
	msgbox gText_NoMoreRoomForPokemon
	return

Global_GiftPokemonToPlayer_5:
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Global_GiftPokemonToPlayer_9
Global_GiftPokemonToPlayer_8:
	setvar VAR_RESULT, 0
	goto Global_GiftPokemonToPlayer_4

Global_GiftPokemonToPlayer_9:
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	getstorestringvar 1, FALSE
	goto Global_GiftPokemonToPlayer_8

Global_GiftPokemonToPlayer_12:
	call Common_EventScript_NameReceivedBoxMon
	getstorestringvar 1, FALSE
	goto Global_GiftPokemonToPlayer_11


Global_GiftPokemonToPlayer_Text_0:
	.string "{PLAYER} received the {STR_VAR_1}!$"
