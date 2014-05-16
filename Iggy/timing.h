#ifndef _TIMING_H_
#define _TIMING_H_

//-------------------------------------------------------------------------
//Setup timers
extern const double physicsRate;
extern const double oobillion;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
//
#endif
