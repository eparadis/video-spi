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
 *
 * Organized into separate files by Ed Paradis
 */
#include <Wire.h>
#include "charROM.h"
#include "params.h"
#include "text.h"
#include "scanlines.h"
#include "interrupts.h"

char videomem[VID_WIDTH*VID_HEIGHT];
extern volatile byte vblank;

void rxData(int numBytes) {
  while(Wire.available()) {
    chrout(Wire.read());
  }
}

void setup(void)
{
  // sleep for a bit to increase the chances of the bootloader working for reprogramming
  delay(2000);

  setupVideoInterrupts();

  // set up TWI
  Wire.onReceive(rxData);
  Wire.begin(0x40);

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

/*
 * Run main loop
 */
void loop(void)
{
  if (!vblank) { // only update during a vblank
    return;
  }

  // do work here

  vblank = 0; // only update once per vblank
}
