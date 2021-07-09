#ifndef CUSTOM_UTILS_H
#define CUSTOM_UTILS_H

u8 GetHiddenPowerType(struct Pokemon* mon);
u8* SplitTextWithNewLines(u8 fontId, const u8* baseStr, s16 letterSpacing, u8 width);
u8* SplitTextOnWordsWithNewLines(u8 fontId, const u8* str, s16 letterSpacing, u8 width);

#endif