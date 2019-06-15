#include "gfx.h"

extern char videomem[VID_WIDTH*VID_HEIGHT];

//
// Plot lores pixels
//
void setpix(signed char x, signed char y)
{
  byte xChar, yChar, blockChar;
  
  if (x > 79 || x < 0 || y > 49 || y < 0)
    return;
  xChar = x >> 1;
  yChar = y >> 1;
  blockChar = 1 << ((x & 1) | ((y & 1) << 1));
  videomem[xChar + yChar * VID_WIDTH] |= blockChar;
}
void clrpix(signed char x, signed char y)
{
  byte xChar, yChar, blockChar;
  
  if (x > 79 || x < 0 || y > 49 || y < 0)
    return;
  xChar = x >> 1;
  yChar = y >> 1;
  blockChar = 1 << ((x & 1) | ((y & 1) << 1));
  videomem[xChar + yChar * VID_WIDTH] &= ~blockChar;
}
void xorpix(signed char x, signed char y)
{
  byte xChar, yChar, blockChar;
  
  if (x > 79 || x < 0 || y > 49 || y < 0)
    return;
  xChar = x >> 1;
  yChar = y >> 1;
  blockChar = 1 << ((x & 1) | ((y & 1) << 1));
  videomem[xChar + yChar * VID_WIDTH] ^= blockChar;
}
void line(signed char x1, signed char y1, signed char x2, signed char y2, void (*oppix)(signed char x, signed char y))
{
  signed char dx, dy, sx, sy, dx2, dy2, dd2, err;
  
  if (x2 >= x1)
  {
    dx = x2 - x1;
    sx = 1;
  }
  else
  {
    dx = x1 - x2;
    sx = -1;
  }
  if (y2 >= y1)
  {
    dy = y2 - y1;
    sy = 1;
  }
  else
  {
    dy = y1 - y2;
    sy = -1;
  }
  dx2 = dx << 1;
  dy2 = dy << 1;
  if (dx >= dy)
  {
    //
    // X major line
    //
    err = dx - dy2;
    dd2 = dx2 - dy2;
    while (x1!= x2)
    {
      oppix(x1, y1);
      x1 += sx;
      if (err < 0)
      {
        err += dd2;
         y1 += sy;
      }
      else
        err -= dy2;
    }
  }
  else
  {
    //
    // Y major line
    //
    err = dy - dx2;
    dd2 = dy2 - dx2;
    while (y1!= y2)
    {
      oppix(x1, y1);
      y1 += sy;
      if (err < 0)
      {
        err += dd2;
        x1 += sx;
      }
      else
        err -= dx2;
    }
  }
  oppix(x2, y2);
}
