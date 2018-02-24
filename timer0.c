#include "timer0.h"

#include <avr/io.h>
#include <util/atomic.h>


// =============================================================================
// Private data:

#define TIMER0_DIVIDER (64)
#define F_OCR0A (1000)

// The following is not declared static so that it will be visible to timer0.S.
volatile uint16_t ms_timestamp_ = 0;


// =============================================================================
// Public functions:

// This function initializes TIMER0. This timer triggers the interrupt "TIMER0
// COMPA" at 1kHz.
void Timer0Init(void)
{
  // Waveform generation mode bits:
  TCCR0B = (0 << WGM02) | (1 << WGM01);
  TCCR0A = (0 << WGM00);
  // Compare match output A mode bits:
  TCCR0A |= (0 << COM0A1) | (0 << COM0A0);
  // Compare match output B mode bits:
  TCCR0A |= (0 << COM0B1) | (0 << COM0B0);
  // Force output compare A bit:
  TCCR0B |= (0 << FOC0A);
  // Force output compare B bit:
  TCCR0B |= (0 << FOC0B);
  // Clock select bits:
  switch (TIMER0_DIVIDER)
  {
    case 1:
      TCCR0B |= 0<<CS02 | 0<<CS01 | 1<<CS00;
      break;
    case 8:
      TCCR0B |= 0<<CS02 | 1<<CS01 | 0<<CS00;
      break;
    case 64:
      TCCR0B |= 0<<CS02 | 1<<CS01 | 1<<CS00;
      break;
    case 256:
      TCCR0B |= 1<<CS02 | 0<<CS01 | 0<<CS00;
      break;
    case 1024:
      TCCR0B |= 1<<CS02 | 0<<CS01 | 1<<CS00;
      break;
    case 0:
    default:
      TCCR0B |= 0<<CS02 | 0<<CS01 | 0<<CS00;
      break;
  }
  // Overflow interrupt enable bit:
  TIMSK0 |= (0 << TOIE0);
  // Output compare match A
  TIMSK0 |= (1 << OCIE0A);  // Output compare match interrupt enable.
  OCR0A = F_CPU / TIMER0_DIVIDER / F_OCR0A;  // Output compare register.
  // Output compare match B
  TIMSK0 |= (0 << OCIE0B);  // Output compare match interrupt enable.
  OCR0B = 0;  // Output compare register.
}

// -----------------------------------------------------------------------------
// This function returns the current timestamp.
uint16_t GetTimestamp(void)
{
  uint16_t ms_timestamp;
  ATOMIC_BLOCK(ATOMIC_FORCEON) { ms_timestamp = ms_timestamp_; }
  return ms_timestamp;
}

// -----------------------------------------------------------------------------
// This function returns a timestamp corresponding to "t" ms in the future. This
// timestamp can be checked against the current timestamp to see if a certain
// amount of time has passed. This function works for durations up to 65535 ms.
uint16_t GetTimestampMillisFromNow(uint16_t t)
{
  uint16_t ms_timestamp;
  ATOMIC_BLOCK(ATOMIC_FORCEON) { ms_timestamp = ms_timestamp_; }
  return ms_timestamp + t + 1;
}

// -----------------------------------------------------------------------------
// This function compares a timestamp to the current timestamp and returns TRUE
// if the timestamp is in the past. This function works for durations up to
// 32767 ms.
uint8_t TimestampInPast(uint16_t t)
{
  int16_t ms_timestamp;
  ATOMIC_BLOCK(ATOMIC_FORCEON) { ms_timestamp = (int16_t)ms_timestamp_; }
  return ((int16_t)t - ms_timestamp) < 0;
}

// -----------------------------------------------------------------------------
// This function returns the amount of time that has elapsed since the timestamp
// "last_time" has occurred. This function works for time periods up to 65535
// ms.
uint16_t MillisSinceTimestamp(uint16_t t)
{
  uint16_t ms_timestamp;
  ATOMIC_BLOCK(ATOMIC_FORCEON) { ms_timestamp = ms_timestamp_; }
  return ms_timestamp - t;
}

// -----------------------------------------------------------------------------
// This function delays execution of the program for "t" ms. Functions triggered
// by interrupts will still execute during this period. This function works for
// time periods up to 32767 ms.
void Wait(uint16_t w)
{
  uint16_t timestamp = GetTimestampMillisFromNow(w);
  while (!TimestampInPast(timestamp));
}
