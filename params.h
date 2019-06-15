#ifndef _PARAMS_H
#define _PARAMS_H

#include <Arduino.h>
//
// Timing settings for NTSC
//
#define _CYCLES_PER_US          (F_CPU / 1000000)
#define _TIME_HSYNC             4.7
#define _TIME_VSYNC             58.85
#define _TIME_ACTIVE            46
#define _CYCLES_VSYNC           ((_TIME_VSYNC * _CYCLES_PER_US) - 1)
#define _CYCLES_HSYNC           ((_TIME_HSYNC * _CYCLES_PER_US) - 1)
#define _NTSC_TIME_SCANLINE      63.55
#define _NTSC_TIME_OUTPUT_START  12
#define _NTSC_LINE_FRAME         262
#define _NTSC_LINE_START_VSYNC   0
#define _NTSC_LINE_STOP_VSYNC    3
#define _NTSC_LINE_DISPLAY       216
#define _NTSC_LINE_MID           ((_NTSC_LINE_FRAME - _NTSC_LINE_DISPLAY)/2 + _NTSC_LINE_DISPLAY/2 + _NTSC_LINE_STOP_VSYNC)
#define _NTSC_CYCLES_SCANLINE    ((_NTSC_TIME_SCANLINE * _CYCLES_PER_US) - 1)
#define _NTSC_CYCLES_OUTPUT_START ((_NTSC_TIME_OUTPUT_START * _CYCLES_PER_US) - 1)
//
// video = UART SPI TX
//
#define PORT_VID     PORTD
#define  DDR_VID     DDRD
#define VID_PIN     1
#define XCK0_DDR     DDRD
#define XCK0         4
//
// sync = OC1A
//
#define PORT_SYNC    PORTB
#define DDR_SYNC     DDRB
#define SYNC_PIN     1
//
// video buffer
//
#define VID_WIDTH    40
#define VID_HEIGHT   25
#define CELL_WIDTH   8
#define CELL_HEIGHT  8

#endif
