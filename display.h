#include <Arduino.h>
#include "env.h"

#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef LCD
#include "lcdgfx.h"

extern DisplaySSD1306_128x32_I2C display;
extern NanoCanvas1 canvas;
typedef bool (*DrawCallback)(void);
extern DrawCallback currentDrawCall;

#else

extern int display;

#endif

#include "log.h"

extern bool display_ready;
bool is_display_ready();
void init_display();
void display_text(String text, int size = 1, bool selected = false, int x = -1, int y = -1);
void display_draw_line(int x, int y, int w);
void clear();
void clear_and_reset();
void draw();
void set_draw_callback(DrawCallback cb);
void perform_draw_call();

#endif
