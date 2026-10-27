#ifndef XEVENT_H
#define XEVENT_H

// Partial: only the events reconstructed code uses.
enum en_xEventTags
{
    eEventVisible = 3,
    eEventInvisible = 4,
    eEventReset = 10,
    eEventToggle = 15,
    eEventStop = 19,
    eEventOn = 38,
    eEventOff = 39,
    eEventPress = 55,
    eEventUnpress = 56,
    eEventCollision_Visible_On = 85,
    eEventCollision_Visible_Off = 86,
    eEventAnimPlay = 195,
    eEventAnimPlayLoop = 196,
    eEventAnimStop = 197,
    eEventAnimPause = 198,
    eEventAnimResume = 199,
    eEventAnimTogglePause = 200,
    eEventAnimPlayRandom = 201,
    eEventAnimPlayMaybe = 202,
    eEventShaggySwap = 209,
    eEventShaggyGrabbed = 232,
    eEventShaggyThrown = 233
};

#endif
