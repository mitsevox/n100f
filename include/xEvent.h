#ifndef XEVENT_H
#define XEVENT_H

// Only the events the reconstructed code compares against. The values come
// from the target; the names follow bfbb, since the reference DWARF has no
// event enumeration.
enum en_xEventTags
{
    eEventReset = 10,
    eEventToggle = 15,
    eEventOn = 38,
    eEventOff = 39
};

#endif
