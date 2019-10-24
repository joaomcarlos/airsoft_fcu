#include "../env.h"

#ifndef DISPLAY_SOURCE
#define DISPLAY_SOURCE

#include "../include/display.h"

#ifdef LCD
int display_ready = 0;
DisplaySSD1306_128x32_I2C display(-1);

int is_display_ready()
{
    return display_ready == 1;
}

void init_display()
{
    display.begin();
    display_ready = 1;
    clear_and_reset();
    info("Conectado ao display!");
}

void display_text(String text, int size, bool selected, int x, int y)
{
    EFontSize fsize = FONT_SIZE_NORMAL;
    if (size == 2)
        fsize = FONT_SIZE_2X;

    if (selected)
        display.negativeMode();
    else
        display.positiveMode();
    //if (!(x == -1 && y == -1))
    display.printFixedN(x, y, text.c_str(), selected ? STYLE_BOLD : STYLE_NORMAL, fsize);
}

void display_draw_line(int x, int y, int w)
{
    display.drawHLine(x, y, x + w);
}

void clear_and_reset()
{
    clear();
    display.setFixedFont(ssd1306xled_font6x8);
    display.positiveMode();
}

void clear()
{
    display.clear();
}

void draw()
{
    //lcd_delay(500);
}

#else

int is_display_ready() { return 1; }
void init_display() {}
void display_text(String text, int size, bool selected, int x, int y)
{
    info(text);
}
void display_draw_line(int x, int y, int w) {}
void clear() {}
void clear_and_reset() {}
void draw() {}

#endif

#endif
