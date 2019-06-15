#ifndef _TEXT_H
#define _TEXT_H

#include <Arduino.h>
#include "params.h"

void chrout(char ascii);
void printstr(char *string);
void box(byte x, byte y, byte w, byte h);
void clrrect(byte x, byte y, byte w, byte h, byte v);

#endif
