#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <avr/pgmspace.h>

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

void PinMode(uint8_t, uint8_t);
void DigitalWrite(uint8_t, uint8_t);
int DigitalRead(uint8_t);

extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2  5
#define TIMER2A 6
#define TIMER2B 7

#define TIMER3A 8
#define TIMER3B 9
#define TIMER3C 10
#define TIMER4A 11
#define TIMER4B 12
#define TIMER4C 13
#define TIMER4D 14
#define TIMER5A 15
#define TIMER5B 16
#define TIMER5C 17

#define ARDUINO_LED_PIN LED_BUILTIN
#define ADRUINO_LED_PORT digitalPinToPort(ARDUINO_LED_PIN)
#define ADRUINO_LED_BIT_MASK digitalPinToBitMask(ARDUINO_LED_PIN)
#define ARDUINO_LED_ENABLE *portModeRegister(ADRUINO_LED_PORT) |= ADRUINO_LED_BIT_MASK
#define ARDUINO_LED_ON *portOutputRegister(ADRUINO_LED_PORT) |= ADRUINO_LED_BIT_MASK
#define ARDUINO_LED_OFF *portOutputRegister(ADRUINO_LED_PORT) &= ~ADRUINO_LED_BIT_MASK
#define ARDUINO_LED_TOGGLE *portOutputRegister(ADRUINO_LED_PORT) ^= ADRUINO_LED_BIT_MASK

#include "pins_arduino.h"


// =============================================================================
// From wiring_digital.c:

#ifdef ARDUINO_MAIN

void PinMode(uint8_t pin, uint8_t mode)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg, *out;

  if (port == NOT_A_PIN) return;

  // JWS: can I let the optimizer do this?
  reg = portModeRegister(port);
  out = portOutputRegister(port);

  if (mode == INPUT) { 
    uint8_t oldSREG = SREG;
                cli();
    *reg &= ~bit;
    *out &= ~bit;
    SREG = oldSREG;
  } else if (mode == INPUT_PULLUP) {
    uint8_t oldSREG = SREG;
                cli();
    *reg &= ~bit;
    *out |= bit;
    SREG = oldSREG;
  } else {
    uint8_t oldSREG = SREG;
                cli();
    *reg |= bit;
    SREG = oldSREG;
  }
}

void DigitalWrite(uint8_t pin, uint8_t val)
{
  // uint8_t timer = digitalPinToTimer(pin);
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out;

  if (port == NOT_A_PIN) return;

  // If the pin that support PWM output, we need to turn it off
  // before doing a digital write.
  // if (timer != NOT_ON_TIMER) turnOffPWM(timer);

  out = portOutputRegister(port);

  uint8_t oldSREG = SREG;
  cli();

  if (val == LOW) {
    *out &= ~bit;
  } else {
    *out |= bit;
  }

  SREG = oldSREG;
}

int DigitalRead(uint8_t pin)
{
  // uint8_t timer = digitalPinToTimer(pin);
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);

  if (port == NOT_A_PIN) return LOW;

  // If the pin that support PWM output, we need to turn it off
  // before getting a digital reading.
  // if (timer != NOT_ON_TIMER) turnOffPWM(timer);

  if (*portInputRegister(port) & bit) return HIGH;
  return LOW;
}

#endif  // ARDUINO_MAIN
// =============================================================================


#endif  // ARDUINO_H_
