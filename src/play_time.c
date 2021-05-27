#include "global.h"
#include "play_time.h"

#define MAX_TIME 0xCDFE5FF  // for 999h, 59m, 59s, 59vb

enum
{
    STOPPED,
    RUNNING,
    MAXED_OUT
};

static u8 sPlayTimeCounterState;
struct PlayTime gPlayTime;


void PlayTimeCounter_PullFromMemory(void)
{
    sPlayTimeCounterState = STOPPED;
    if (gSaveBlock2Ptr->playTime > MAX_TIME)
    {
        PlayTimeCounter_SetToMax();
    }
    else
    {
        gPlayTime.Maxed   = FALSE;
        gPlayTime.Hours   = gSaveBlock2Ptr->playTime / 216000;
        gPlayTime.Minutes = (gSaveBlock2Ptr->playTime / 3600) % 60;
        gPlayTime.Seconds = (gSaveBlock2Ptr->playTime / 60) % 60;
        gPlayTime.VBlanks = gSaveBlock2Ptr->playTime % 60;
    }
}

void PlayTimeCounter_Reset(void)
{
    sPlayTimeCounterState = STOPPED;

    gSaveBlock2Ptr->playTime = 0;
    gPlayTime.Hours = 0;
    gPlayTime.Minutes = 0;
    gPlayTime.Seconds = 0;
    gPlayTime.VBlanks = 0;
}

void PlayTimeCounter_Start(void)
{
    sPlayTimeCounterState = RUNNING;

    if (gPlayTime.Hours > 999)
        PlayTimeCounter_SetToMax();
}

void PlayTimeCounter_Stop(void)
{
    sPlayTimeCounterState = STOPPED;
}

void PlayTimeCounter_Update(void)
{
    gSaveBlock2Ptr->playTime++;
    if (sPlayTimeCounterState != RUNNING)
        return;

    gPlayTime.VBlanks++;

    if (gPlayTime.VBlanks < 60)
        return;

    gPlayTime.VBlanks = 0;
    gPlayTime.Seconds++;

    if (gPlayTime.Seconds < 60)
        return;

    gPlayTime.Seconds = 0;
    gPlayTime.Minutes++;

    if (gPlayTime.Minutes < 60)
        return;

    gPlayTime.Minutes = 0;
    gPlayTime.Hours++;

    if (gPlayTime.Hours > 999)
        PlayTimeCounter_SetToMax();
}

void PlayTimeCounter_SetToMax(void)
{
    sPlayTimeCounterState = MAXED_OUT;

    gPlayTime.Maxed = TRUE;
    gPlayTime.Hours = 999;
    gPlayTime.Minutes = 59;
    gPlayTime.Seconds = 59;
    gPlayTime.VBlanks = 59;
}
