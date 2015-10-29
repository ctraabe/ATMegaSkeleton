#ifndef ARDUINO_H_
#define ARDUINO_H_

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

#endif  // ARDUINO_H_
