#include "main.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "arduino.h"
#include "timer0.h"


int16_t main(void)
{
  ARDUINO_LED_ENABLE;
  Timer0Init();

  sei();  // Enable interrupts

  // Main loop
  uint16_t timer = GetTimestampMillisFromNow(250);
  for (;;)  // Preferred over while(1)
  {
    while(!TimestampInPast(timer)) continue;
    timer += 250;
    ARDUINO_LED_TOGGLE;
  }
}
