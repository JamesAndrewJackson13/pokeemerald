#ifndef TOAST_NOTIFICATIONS_H
#define TOAST_NOTIFICATIONS_H

void DrawToastBox(u8 left, u8 top, u8 width, u8 height);
void HideToastBox();

void DrawToastBoxText(u8* dst, u8 x, u8 y);

void ShowItemIconSprite(u16 item, bool8 firstTime, bool8 flash);
void DestroyItemIconSprite(void);

void DrawHeaderBox(u16 itemToShow, u8* dst, bool8 handleFlash, u8 numLinesToDraw);
void HideHeaderBox(bool8 hadItem, bool8 hadMsgBox);


void DrawItemHeaderBox(void);
void HideItemHeaderBox(void);

void DrawAutoRunBox(bool8);
void HideAutoRunBox(void);

#ifdef FEATURE_SWAPBIKEBUTTON
void DrawBikeHeaderBox(void);
void HideBikeHeaderBox(void);
#endif

#endif  // TOAST_NOTIFICATIONS_H