#include "global.h"
#include "text.h"
#include "pokemon.h"
#include "dynamic_placeholder_text_util.h"


u8 GetHiddenPowerType(struct Pokemon* mon)
{
    u8 type = ((GetMonData(mon, MON_DATA_HP_IV) & 1) << 0)
        | ((GetMonData(mon, MON_DATA_ATK_IV) & 1) << 1)
        | ((GetMonData(mon, MON_DATA_DEF_IV) & 1) << 2)
        | ((GetMonData(mon, MON_DATA_SPEED_IV) & 1) << 3)
        | ((GetMonData(mon, MON_DATA_SPATK_IV) & 1) << 4)
        | ((GetMonData(mon, MON_DATA_SPDEF_IV) & 1) << 5);

    type = (15 * type) / 63 + 1;
    if (type >= TYPE_MYSTERY)
        type++;
    type |= 0xC0;

    return type;
}


u8* SplitTextOnWordsWithNewLines(u8 fontId, const u8* baseStr, s16 letterSpacing, u8 width)
{
    const u8* str;
    u8 stackIndex = 0;
    const u8* placeholderDynamicStack[10];
    u8 bufferIndex = 0;
    u8* strCursor = gStringVar4;
    bool8 isJapanese;
    int minGlyphWidth;
    u32(*func)(u16 glyphId, bool32 isJapanese);
    int localLetterSpacing;
    u8 lineWidth;
    u8 wordWidth;
    const u8* wordStart;
    u8 queueIndex = 0;
    u8 queueEnd = 0;
    const u8* wordSourceQueue[10];
    const u8* bufferPointer;
    int glyphWidth;
    u8 placeholderId;

    isJapanese = 0;
    minGlyphWidth = 0;

    func = GetFontWidthFunc(fontId);
    if (func == NULL)
        return 0;

    if (letterSpacing == -1)
        localLetterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
    else
        localLetterSpacing = letterSpacing;

    lineWidth = 0;
    bufferPointer = 0;

    // Add initial string
    placeholderDynamicStack[stackIndex++] = baseStr;
    wordStart = baseStr;
    wordWidth = 0;

    while (stackIndex > 0)
    {
        str = placeholderDynamicStack[--stackIndex];
        while (*str != EOS)
        {
            switch (*str)
            {
            case CHAR_NEWLINE:
            case CHAR_SPACE:
                queueIndex = 0;
                glyphWidth = func(*str, isJapanese);  // Size of the space
                if (lineWidth + wordWidth < width)
                {
                    // This line has room for another word
                    // Copy over the word
                    while (wordStart != str)
                    {
                        switch (*wordStart)
                        {
                        case(EOS):
                            wordStart = wordSourceQueue[queueIndex++];
                            break;
                        default:
                            *strCursor++ = *wordStart++;
                        }
                    }

                    if(lineWidth + wordWidth + glyphWidth < width)
                    {
                        // In this case, we know there's possibly room for more words
                        *strCursor++ = *str;
                        lineWidth += wordWidth + glyphWidth;
                    }
                    else
                    {
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                }
                else
                {
                    // The line is done, and the last word needs to be on a new line
                    if (lineWidth != 0)
                    {
                        // if handles edge case for when the  word is too long and the only word on the line (so no nl is needed)
                        *strCursor++ = CHAR_NEWLINE;
                    }
                    while (wordStart != str)
                    {
                        switch (*wordStart)
                        {
                        case(EOS):
                            wordStart = wordSourceQueue[queueIndex++];
                            break;
                        default:
                            *strCursor++ = *wordStart++;
                        }
                    }
                    if (wordWidth + glyphWidth > width)
                    {
                        // Handling edge case where the word+whitespace is so wide, it fills the new line
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                    else
                    {
                        // Standard case, make the line width the same as the word + whitespace width
                        *strCursor++ = *str;
                        lineWidth = wordWidth + glyphWidth;
                    }
                }
                if (*str == CHAR_NEWLINE)
                {
                    // Make sure we reset lineWidth if we just added a NL
                    lineWidth = 0;
                }
                str++; // This gets us past the whitespace
                wordWidth = 0;
                queueEnd = 0;
                break;
            case PLACEHOLDER_BEGIN:
                placeholderId = *++str;
                placeholderDynamicStack[stackIndex++] = str;
                str = GetExpandedPlaceholder(placeholderId);
                wordSourceQueue[queueEnd++] = str;
                break;
            case CHAR_DYNAMIC:
                if (bufferPointer == NULL)
                    bufferPointer = DynamicPlaceholderTextUtil_GetPlaceholderPtr(*++str);
                placeholderDynamicStack[stackIndex++] = str;
                str = bufferPointer;
                wordSourceQueue[queueEnd++] = str;
                bufferPointer = NULL;
                break;
            case EXT_CTRL_CODE_BEGIN:
                switch (*++str)
                {
                case EXT_CTRL_CODE_COLOR_HIGHLIGHT_SHADOW:
                    ++str;
                case EXT_CTRL_CODE_PLAY_BGM:
                case EXT_CTRL_CODE_PLAY_SE:
                    ++str;
                case EXT_CTRL_CODE_COLOR:
                case EXT_CTRL_CODE_HIGHLIGHT:
                case EXT_CTRL_CODE_SHADOW:
                case EXT_CTRL_CODE_PALETTE:
                case EXT_CTRL_CODE_PAUSE:
                case EXT_CTRL_CODE_ESCAPE:
                case EXT_CTRL_CODE_SHIFT_RIGHT_TEXT:
                case EXT_CTRL_CODE_SHIFT_LEFT_TEXT:
                case EXT_CTRL_CODE_SHIFT_UP:
                case EXT_CTRL_CODE_SHIFT_DOWN:
                    ++str;
                    break;
                case EXT_CTRL_CODE_SIZE:
                    func = GetFontWidthFunc(*++str);
                    if (func == NULL)
                        return 0;
                    if (letterSpacing == -1)
                        localLetterSpacing = GetFontAttribute(*str, FONTATTR_LETTER_SPACING);
                    break;
                case EXT_CTRL_CODE_CLEAR:  // Didn't update, unsure what this ctrl_code does
                    glyphWidth = *++str;
                    lineWidth += glyphWidth;
                    break;
                case EXT_CTRL_CODE_SKIP:  // Didn't update, unsure what this ctrl_code does
                    lineWidth = *++str;
                    break;
                case EXT_CTRL_CODE_CLEAR_TO:  // Didn't update, unsure what this ctrl_code does
                    if (*++str > lineWidth)
                        lineWidth = *str;
                    break;
                case EXT_CTRL_CODE_MIN_LETTER_SPACING:
                    minGlyphWidth = *++str;
                    break;
                case EXT_CTRL_CODE_JPN:
                    isJapanese = 1;
                    break;
                case EXT_CTRL_CODE_ENG:
                    isJapanese = 0;
                    break;
                case EXT_CTRL_CODE_RESET_SIZE:
#ifdef FEATURE_RESETSIZEFUNCTION
                    if (letterSpacing == -1)
                        localLetterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
                    else
                        localLetterSpacing = letterSpacing;
                    break;
#endif
                case EXT_CTRL_CODE_PAUSE_UNTIL_PRESS:
                case EXT_CTRL_CODE_WAIT_SE:
                case EXT_CTRL_CODE_FILL_WINDOW:
                default:
                    break;
                }
                break;
            case CHAR_KEYPAD_ICON:
            case CHAR_EXTRA_SYMBOL:
                if (*str == CHAR_EXTRA_SYMBOL)
                    glyphWidth = func(*++str | 0x100, isJapanese);
                else
                    glyphWidth = GetKeypadIconWidth(*++str);

                if (minGlyphWidth > 0)
                {
                    if (glyphWidth < minGlyphWidth)
                        glyphWidth = minGlyphWidth;
                    wordWidth += glyphWidth;
                }
                else
                {
                    if (isJapanese && str[1] != EOS)
                        glyphWidth += localLetterSpacing;
                    wordWidth += glyphWidth;
                }
                break;
            case CHAR_PROMPT_SCROLL:
            case CHAR_PROMPT_CLEAR:
                break;
            default:
                glyphWidth = func(*str, isJapanese);
                if (minGlyphWidth > 0)
                {
                    if (glyphWidth < minGlyphWidth)
                        glyphWidth = minGlyphWidth;
                    wordWidth += glyphWidth;
                }
                else
                {
                    if (isJapanese && str[1] != EOS)
                        glyphWidth += localLetterSpacing;
                    wordWidth += glyphWidth;
                }
                break;
            }
            ++str;
        }
    }
    if (lineWidth + wordWidth < width)
    {
        // This line has room for another word
        // Copy over the word
        while (wordStart != str)
        {
            switch (*wordStart)
            {
            case(EOS):
                wordStart = wordSourceQueue[queueIndex++];
                break;
            default:
                *strCursor++ = *wordStart++;
            }
        }

        if (lineWidth + wordWidth + glyphWidth < width)
        {
            // In this case, we know there's possibly room for more words
            *strCursor++ = *str;
            lineWidth += wordWidth + glyphWidth;
        }
        else
        {
            *strCursor++ = CHAR_NEWLINE;
            lineWidth = 0;
        }
    }
    else
    {
        // The line is done, and the last word needs to be on a new line
        if (lineWidth != 0)
        {
            // if handles edge case for when the  word is too long and the only word on the line (so no nl is needed)
            *strCursor++ = CHAR_NEWLINE;
        }
        while (wordStart != str)
        {
            switch (*wordStart)
            {
            case(EOS):
                wordStart = wordSourceQueue[queueIndex++];
                break;
            default:
                *strCursor++ = *wordStart++;
            }
        }
        if (wordWidth + glyphWidth > width)
        {
            // Handling edge case where the word+whitespace is so wide, it fills the new line
            *strCursor++ = CHAR_NEWLINE;
            lineWidth = 0;
        }
        else
        {
            // Standard case, make the line width the same as the word + whitespace width
            *strCursor++ = *str;
            lineWidth = wordWidth + glyphWidth;
        }
    }
    *strCursor++ = EOS;
    logU8String(gStringVar4);
    return gStringVar4;
}

u8* SplitTextWithNewLines(u8 fontId, const u8* baseStr, s16 letterSpacing, u8 width)
{
    const u8* str;
    u8 stackIndex = 0;
    const u8* placeholderDynamicStack[10];
    u8 bufferIndex = 0;
    u8* strCursor = gStringVar4;
    bool8 isJapanese;
    int minGlyphWidth;
    u32(*func)(u16 glyphId, bool32 isJapanese);
    int localLetterSpacing;
    u8 lineWidth;
    const u8* bufferPointer;
    int glyphWidth;
    u8 placeholderId;

    isJapanese = 0;
    minGlyphWidth = 0;

    func = GetFontWidthFunc(fontId);
    if (func == NULL)
        return 0;

    if (letterSpacing == -1)
        localLetterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
    else
        localLetterSpacing = letterSpacing;

    lineWidth = 0;
    bufferPointer = 0;

    // Add initial string
    placeholderDynamicStack[stackIndex] = baseStr;
    stackIndex++;

    while (stackIndex > 0)
    {
        str = placeholderDynamicStack[--stackIndex];
        while (*str != EOS)
        {
            switch (*str)
            {
            case CHAR_NEWLINE:
                lineWidth = 0;
                *strCursor++ = CHAR_NEWLINE;
                break;
            case PLACEHOLDER_BEGIN:
                placeholderId = *++str;
                placeholderDynamicStack[stackIndex++] = str;
                str = GetExpandedPlaceholder(placeholderId);
                break;
            case CHAR_DYNAMIC:
                if (bufferPointer == NULL)
                    bufferPointer = DynamicPlaceholderTextUtil_GetPlaceholderPtr(*++str);
                while (*bufferPointer != EOS)
                {
                    glyphWidth = func(*bufferPointer, isJapanese);
                    if (minGlyphWidth > 0)
                    {
                        if (glyphWidth < minGlyphWidth)
                            glyphWidth = minGlyphWidth;
                        if (glyphWidth + lineWidth > width)
                        {
                            *strCursor++ = CHAR_NEWLINE;
                            lineWidth = 0;
                        }
                        *strCursor++ = *bufferPointer;
                        lineWidth += glyphWidth;
                    }
                    else
                    {
                        if (isJapanese && str[1] != EOS)
                            glyphWidth += localLetterSpacing;
                        if (glyphWidth + lineWidth > width)
                        {
                            *strCursor++ = CHAR_NEWLINE;
                            lineWidth = 0;
                        }
                        *strCursor++ = *bufferPointer;
                        lineWidth += glyphWidth;
                    }
                    ++bufferPointer;
                }
                bufferPointer = 0;
                break;
            case EXT_CTRL_CODE_BEGIN:
                switch (*++str)
                {
                case EXT_CTRL_CODE_COLOR_HIGHLIGHT_SHADOW:
                    ++str;
                case EXT_CTRL_CODE_PLAY_BGM:
                case EXT_CTRL_CODE_PLAY_SE:
                    ++str;
                case EXT_CTRL_CODE_COLOR:
                case EXT_CTRL_CODE_HIGHLIGHT:
                case EXT_CTRL_CODE_SHADOW:
                case EXT_CTRL_CODE_PALETTE:
                case EXT_CTRL_CODE_PAUSE:
                case EXT_CTRL_CODE_ESCAPE:
                case EXT_CTRL_CODE_SHIFT_RIGHT_TEXT:
                case EXT_CTRL_CODE_SHIFT_LEFT_TEXT:
                case EXT_CTRL_CODE_SHIFT_UP:
                case EXT_CTRL_CODE_SHIFT_DOWN:
                    ++str;
                    break;
                case EXT_CTRL_CODE_SIZE:
                    func = GetFontWidthFunc(*++str);
                    if (func == NULL)
                        return 0;
                    if (letterSpacing == -1)
                        localLetterSpacing = GetFontAttribute(*str, FONTATTR_LETTER_SPACING);
                    break;
                case EXT_CTRL_CODE_CLEAR:  // Didn't update, unsure what this ctrl_code does
                    glyphWidth = *++str;
                    lineWidth += glyphWidth;
                    break;
                case EXT_CTRL_CODE_SKIP:  // Didn't update, unsure what this ctrl_code does
                    lineWidth = *++str;
                    break;
                case EXT_CTRL_CODE_CLEAR_TO:  // Didn't update, unsure what this ctrl_code does
                    if (*++str > lineWidth)
                        lineWidth = *str;
                    break;
                case EXT_CTRL_CODE_MIN_LETTER_SPACING:
                    minGlyphWidth = *++str;
                    break;
                case EXT_CTRL_CODE_JPN:
                    isJapanese = 1;
                    break;
                case EXT_CTRL_CODE_ENG:
                    isJapanese = 0;
                    break;
                case EXT_CTRL_CODE_RESET_SIZE:
#ifdef FEATURE_RESETSIZEFUNCTION
                    if (letterSpacing == -1)
                        localLetterSpacing = GetFontAttribute(fontId, FONTATTR_LETTER_SPACING);
                    else
                        localLetterSpacing = letterSpacing;
                    break;
#endif
                case EXT_CTRL_CODE_PAUSE_UNTIL_PRESS:
                case EXT_CTRL_CODE_WAIT_SE:
                case EXT_CTRL_CODE_FILL_WINDOW:
                default:
                    break;
                }
                break;
            case CHAR_KEYPAD_ICON:
            case CHAR_EXTRA_SYMBOL:
                if (*str == CHAR_EXTRA_SYMBOL)
                    glyphWidth = func(*++str | 0x100, isJapanese);
                else
                    glyphWidth = GetKeypadIconWidth(*++str);

                if (minGlyphWidth > 0)
                {
                    if (glyphWidth < minGlyphWidth)
                        glyphWidth = minGlyphWidth;
                    if (glyphWidth + lineWidth > width)
                    {
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                    *strCursor++ = *str;
                    lineWidth += glyphWidth;
                }
                else
                {
                    if (isJapanese && str[1] != EOS)
                        glyphWidth += localLetterSpacing;
                    if (glyphWidth + lineWidth > width)
                    {
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                    *strCursor++ = *str;
                    lineWidth += glyphWidth;
                }
                break;
            case CHAR_PROMPT_SCROLL:
            case CHAR_PROMPT_CLEAR:
                break;
            default:
                glyphWidth = func(*str, isJapanese);
                if (minGlyphWidth > 0)
                {
                    if (glyphWidth < minGlyphWidth)
                        glyphWidth = minGlyphWidth;
                    if (glyphWidth + lineWidth > width)
                    {
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                    *strCursor++ = *str;
                    lineWidth += glyphWidth;
                }
                else
                {
                    if (isJapanese && str[1] != EOS)
                        glyphWidth += localLetterSpacing;
                    if (glyphWidth + lineWidth > width)
                    {
                        *strCursor++ = CHAR_NEWLINE;
                        lineWidth = 0;
                    }
                    *strCursor++ = *str;
                    lineWidth += glyphWidth;
                }
                break;
            }
            ++str;
        }
    }
    *strCursor++ = EOS;
    return gStringVar4;
}