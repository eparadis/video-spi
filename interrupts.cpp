#include <Arduino.h>
#include "interrupts.h"
#include "scanlines.h"

extern void (*line_handler)(void);
extern int scanline;

//
// Handle scanline timing
//
ISR(TIMER1_COMPA_vect)
{
  sei();
  asm("sleep\n"); // for clock synchronization during active lines
}
ISR(TIMER1_COMPB_vect)
{
  line_handler();
  scanline++;
}

/*
 * Set up UART SPI master mode and timers for sync pulses
 */
void setupVideoInterrupts() {
  cli();
  UBRR0 = 0; // must be zero before enabling the transmitter
  XCK0_DDR  |= _BV(XCK0); // set XCK pin as output to enable master mode
  UCSR0C     = _BV (UMSEL00) | _BV (UMSEL01);  // SPI master mode
  DDR_VID   |= _BV(VID_PIN);
  DDR_SYNC  |= _BV(SYNC_PIN);
  PORT_VID  &= ~_BV(VID_PIN);
  PORT_SYNC |= _BV(SYNC_PIN);
  TCCR1A     = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11); // inverted fast pwm mode on timer 2
  TCCR1B     = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  ICR1       = _NTSC_CYCLES_SCANLINE;
  OCR1A      = _CYCLES_HSYNC;
  OCR1B      = _NTSC_CYCLES_OUTPUT_START - 79;
  TIMSK1     = _BV(OCIE1B);
  TIMSK0     = 0; // turn timer0 off!
  SMCR       = _BV(SE); // allow IDLE sleep mode
  sei();
}
