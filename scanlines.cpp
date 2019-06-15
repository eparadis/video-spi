#include "scanlines.h"

extern int scanline;
extern char *videoptr;
extern char videomem[VID_WIDTH*VID_HEIGHT];
extern void (*line_handler)(void);
extern volatile byte vblank;

//
// Inactive scanlines
//
void blank_line(void)
{
  if (scanline == _NTSC_LINE_STOP_VSYNC)
  {
    OCR1A = _CYCLES_HSYNC;
  }
  else if ( scanline == _NTSC_LINE_MID - (VID_HEIGHT*CELL_HEIGHT)/2)
  {
    TIMSK1       = _BV(OCIE1A) | _BV(OCIE1B);
    videoptr     = videomem;
    line_handler = &active_line;
  }
  else if (scanline > _NTSC_LINE_FRAME)
  {
    OCR1A = _CYCLES_VSYNC;
    scanline = 0;
  }
}
//
// Active scanlines
//
void active_line(void)
{
  register char *charPtr;
  register int charCol;
  byte p ;
  static byte charRow = 0;  

  UDR0 = 0;
  UCSR0B = _BV(TXEN0); // this spits out an idle bit (white pixel) so move it off the left side of the screen
  charPtr = videoptr; 
  while ((UCSR0A & _BV (UDRE0)) == 0);
  UDR0 = 0;
  charCol = (int)&charROM + (charRow<<7); // overlap with some initialization
  while ((UCSR0A & _BV (UDRE0)) == 0);
  UDR0 = 0;
  while ((UCSR0A & _BV (UDRE0)) == 0);
  UDR0 = 0;
  while ((UCSR0A & _BV (UDRE0)) == 0);
  UDR0 = 0;
  p = VID_WIDTH;
  while ((UCSR0A & _BV (UDRE0)) == 0);
  UDR0 = 0;
  while (p--) // draw the character line
    UDR0 = pgm_read_byte_near(charCol + *charPtr++);
  while ((UCSR0A & _BV (TXC0)) == 0); // wait for pixels to shift out
  UCSR0B = 0;
  if (++charRow == CELL_HEIGHT)
  {
    charRow = 0;
    if (scanline == _NTSC_LINE_MID + (VID_HEIGHT*CELL_HEIGHT)/2)
    {
      TIMSK1 = _BV(OCIE1B);
      line_handler = &blank_line;
      vblank=1; // signal new frame
    }
    else
    {
      videoptr += VID_WIDTH;
    }
  }
}
