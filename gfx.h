#ifndef _GFX_H
#define _GFX_H

#include <Arduino.h>
#include "params.h"

void setpix(signed char x, signed char y);
void clrpix(signed char x, signed char y);
void xorpix(signed char x, signed char y);

void line(signed char x1, signed char y1, signed char x2, signed char y2, void (*oppix)(signed char x, signed char y));

#endif
