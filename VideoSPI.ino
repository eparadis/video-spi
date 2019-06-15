/*
 * Output NTSC video using the UART in SPI master mode on Arduino Uno.
 * 40x25 character buffer is rasterized and scanned out in real-time.
 * 320X200 pixels out of 1000 RAM bytes and 1K Flash character ROM.
 * VIDEO on pin 1 (470 ohm resistor), SYNC on pin 9 (1K ohm resistor).
 *
 * Blatantly using code from:
 * TVout                 - http://playground.arduino.cc/Main/TVout
 * VGAout                - http://www.gammon.com.au/forum/?id=11608
 * Color TVout using SPI - http://www.hackster.io/janost/avr-videoblaster
 *
 * Dave Schmenk
 */
#include "charROM.h"
#include "params.h"
#include "text.h"
#include "scanlines.h"

char videomem[VID_WIDTH*VID_HEIGHT];
//
// rendering values
//
char *videoptr = videomem;
int scanline=0;
void (*line_handler)(void) = &blank_line;
volatile byte vblank=0;
/*
 * Set up UART SPI master mode and timers for sync pulses
 */
void setup(void)
{
  // sleep for a bit to increase the chances of the bootloader working for reprogramming
  delay(2000);
  
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
  //
  // Clear the video buffer and print out sample text
  //
  chrout(12);
  printstr("0123456789012345678901234567890123456789");
  printstr("\n             Video SPI Demo\n\n");
  for (byte i = ' '; i < 128; i++)
    chrout(i);
  box(0, 9, 19, 15);
  clrrect(20,9, 19, 15, 0);
  box(20,9,19,15);
}
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
 * Run main loop
 */
int now = 0;
int earlier = 0;
byte chargen = 0;
void loop(void)
{
  if (!vblank) { // only update during a vblank
    return;
  }

  now += 1;
  if( now - earlier > 15) { // approx two chars a second (~30 fps / 15 fpc = ~2 cps)
    earlier = now;
    chargen = (chargen + 1) % 96;
    chrout(' ' + chargen);
  }

  vblank = 0; // only update once per vblank
}
