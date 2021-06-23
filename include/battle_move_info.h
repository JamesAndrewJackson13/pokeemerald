#ifndef GUARD_MOVE_INFO_H
#define GUARD_MOVE_INFO_H

bool8 BattleMoveInfo_NeedsInit(void);
void BattleMoveInfo_Init(void);
void BattleMoveInfo_Clean(void);
bool8 BattleMoveInfo_LoadGraphics(void);
bool8 BattleMoveInfo_isLoaded(void);
void BattleMoveInfo_LoadAndPrep(void);
void BattleMoveInfo_ToggleSlider(void);
void BattleMoveInfo_Show(void);
void BattleMoveInfo_Hide(void);
void BattleMoveInfo_UpdateMove(u16 moveId);

#endif // GUARD_MOVE_INFO_H