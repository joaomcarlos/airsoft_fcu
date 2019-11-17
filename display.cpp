#include "env.h"

#ifndef DISPLAY_SOURCE
#define DISPLAY_SOURCE

#include "display.h"
#include "global.h"

bool display_ready = false;

DisplaySSD1306_128x32_I2C display(-1);
const int canvasWidth = 128;
const int canvasHeight = 32;
uint8_t canvasData[canvasWidth * (canvasHeight / 8)];
NanoCanvas1 canvas(canvasWidth, canvasHeight, canvasData);
DrawCallback currentDrawCall;

bool is_display_ready()
{
    return display_ready;
}

void init_display()
{
    display.begin();
    display.clear();
    canvas.setMode(CANVAS_MODE_TRANSPARENT);
    canvas.setFixedFont(ssd1306xled_font6x8);

    display_ready = 1;
    clear_and_reset();
    info("Conectado ao display!");
    tasker.setInterval(perform_draw_call, 73);
}

void display_text(String text, int size, bool selected, int x, int y)
{
    EFontSize fsize = FONT_SIZE_NORMAL;
    if (size == 2)
        fsize = FONT_SIZE_2X;

    if (selected)
    {
        canvas.fillRect(x, y, text.length() * 8, y + 8);
        canvas.setColor(BLACK);
    }
    else
        canvas.setColor(WHITE);
    //if (!(x == -1 && y == -1))
    canvas.printFixed(x, y, text.c_str(), selected ? STYLE_BOLD : STYLE_NORMAL);
    canvas.setColor(WHITE); // reset it back
}

void display_draw_line(int x, int y, int w)
{
    canvas.setColor(BLACK);
    canvas.drawHLine(x, y, x + w);
    canvas.setColor(WHITE);
}

void clear_and_reset()
{
    clear();
}

void clear()
{
    canvas.clear();
    canvas.setColor(WHITE);
}

void draw()
{
    display.drawCanvas(0, 0, canvas);
}

void perform_draw_call()
{
    if (currentDrawCall && currentDrawCall())
    {
        draw();
    }
}

void set_draw_callback(DrawCallback cb)
{
    currentDrawCall = cb;
}

#endif
