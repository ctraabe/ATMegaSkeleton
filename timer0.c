#include "timer0.h"

#include <avr/io.h>


// ============================================================================+
// Private data:

#define TIMER0_DIVIDER (64)
#define F_OCR0A (1000)

volatile uint16_t ms_timestamp_ = 0;


// ============================================================================+
// Public functions:

// This function initializes TIMER0. This timer triggers the interrupt "TIMER0
// COMPA" at 1kHz.
void Timer0Init(void)
{
  // Clear TIMER0 registers.
  TCCR0A = 0;
  TCCR0B = 0;
  TIMSK0 = 0;
  // Waveform generation mode bits:
  TCCR0B |= (0 << WGM02);
  TCCR0A |= (1 << WGM01);
  TCCR0A |= (0 << WGM00);
  // Compare match output A mode bits:
  TCCR0A |= (0 << COM0A1);
  TCCR0A |= (0 << COM0A0);
  // Compare match output B mode bits:
  TCCR0B |= (0 << COM0B1);
  TCCR0B |= (0 << COM0B0);
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
  // Clear the timer.
  TCNT0 = 0;
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
