#ifndef XEVENT_H
#define XEVENT_H

// Partial: only the events reconstructed code uses.
enum en_xEventTags
{
    eEventDisable = 2,
    eEventVisible = 3,
    eEventInvisible = 4,
    eEventReset = 10,
    eEventIncrement = 11,
    eEventDecrement = 12,
    eEventToggle = 15,
    eEventRun = 18,
    eEventStop = 19,
    eEventExpired = 20,
    eEventDestroy = 22,
    eEventPause = 23,
    eEventPlay = 24,
    eEventPlayMaybe = 26,
    eEventMount = 32,
    eEventDismount = 33,
    eEventOn = 38,
    eEventOff = 39,
    eEventPress = 55,
    eEventUnpress = 56,
    eEventHit = 58,
    eEventCollisionOn = 83,
    eEventCollisionOff = 84,
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
    eEventSwingerFollow = 206,
    eEventShaggyMount = 207,
    eEventShaggySwap = 209,
    eEventStartMoving = 226,
    eEventStopMoving = 227,
    eEventShaggyGrabbed = 232,
    eEventShaggyThrown = 233,
    eEventTranslucentOn = 252,
    eEventTranslucentOff = 253,
    eEventTranslucentToggle = 254,
    eEventDisableGroupContents = 262
};

#endif
