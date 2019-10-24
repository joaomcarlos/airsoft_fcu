#include "../env.h"
#include <Arduino.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef LCD
#include "lcdgfx.h"

extern DisplaySSD1306_128x32_I2C display;

#else

extern int display;

#endif

#include "log.h"

extern int display_ready;
int is_display_ready();
void init_display();
void display_text(String text, int size = 1, bool selected = false, int x = -1, int y = -1);
void display_draw_line(int x, int y, int w);
void clear();
void clear_and_reset();
void draw();

#endif
