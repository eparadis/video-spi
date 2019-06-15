#include "text.h"

extern char videomem[VID_WIDTH*VID_HEIGHT];
byte xpos=0;
byte ypos=0;

/*
 * Character output routines.
 */
void scrollscr(void)
{
  for (unsigned int i=VID_WIDTH; i < VID_WIDTH*VID_HEIGHT; i++)
    videomem[i-VID_WIDTH]=videomem[i];
  for (unsigned int i=VID_WIDTH*VID_HEIGHT - VID_WIDTH; i < VID_WIDTH*VID_HEIGHT; i++)
    videomem[i]=' ';
}
void clrscr(byte val)
{
  for (int i = VID_WIDTH*VID_HEIGHT-1; i >= 0; --i)
    videomem[i] = val;
}
void chrout(char ascii)
{
  switch (ascii) {
    case 10:
      xpos=0;
      ypos++;
      if (ypos>=VID_HEIGHT) {
       scrollscr(); 
       xpos=0;
       ypos=VID_HEIGHT-1;
      }
      break;
    case 12:
      clrscr(' ');
      xpos=0;
      ypos=0;
      break;      
    default:
    if (ascii >= ' ' && ascii < 128) {
      videomem[xpos+ypos*VID_WIDTH] = ascii;
      xpos++;
      if (xpos>=VID_WIDTH) {
       xpos=0;
       ypos++;
       if (ypos>=VID_HEIGHT) {
        scrollscr(); 
        xpos=0;
        ypos=VID_HEIGHT-1;
       }
      }
    }
   }  
}
void clrrect(byte x, byte y, byte w, byte h, byte v)
{
  byte i, j;
  char *vptr = &videomem[x + y * VID_WIDTH];
  for (j = 0; j < h; j++)
  {
    for (i = 0; i < w; i++)
      *vptr++ = v;
    vptr += VID_WIDTH - w;
  }  
}
void box(byte x, byte y, byte w, byte h)
{
  byte i, j;
  char *vptr = &videomem[x + y * VID_WIDTH];
  *vptr++ = 0x16;
  for (i = 1; i < w; i++)
    *vptr++ = 0x1A;
  *vptr = 0x1C;
  vptr += VID_WIDTH - w;
  for (j = 1; j < h; j++)
  {
    vptr[0] = 0x15;
    vptr[w] = 0x15;
    vptr += VID_WIDTH;
  }  
  *vptr++ = 0x13;
  for (i = 1; i < w; i++)
    *vptr++ = 0x1A;
  *vptr = 0x19;
}
void printstr(char *string)
{
 while(*string)
   chrout(*string++);
}
void gotoxy(int x, int y)
{
  if (x < 0) x = 0;
  if (x > VID_WIDTH-1) x = VID_WIDTH-1;
  if (y < 0) y = 0;
  if (y > VID_HEIGHT-1) y = VID_HEIGHT-1;
  xpos = x;
  ypos = y;
}
