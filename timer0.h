#ifndef _TIMER0_H
#define _TIMER0_H

#include <inttypes.h>

void Timer0Init(void);

uint16_t GetTimestampMillisFromNow(uint16_t t);
uint8_t TimestampInPast(uint16_t t);
uint16_t MillisSinceTimestamp(uint16_t *last_time);

void Wait(uint16_t);

#endif
