#ifndef DISPLAY_SOURCE
#define DISPLAY_SOURCE

#include "display.h"

int display_ready = 0;
Adafruit_SSD1306 display(128, 32, &Wire, -1);

int is_display_ready()
{
    return display_ready == 1;
}

void init_display()
{
    while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C /* Address 0x3C for 128x32 */))
    {
        info("SSD1306 allocation failed");
        delay(100);
    }
    display_ready = 1;
    clear_and_reset();
    info("Conectado ao display!");
}

void display_text(String text, int size, bool selected, int x, int y)
{
    display.cp437(true);
    display.setTextSize(size);
    if (selected)
        display.setTextColor(BLACK, WHITE);
    else
        display.setTextColor(WHITE);

    if (!(x == -1 && y == -1))
        display.setCursor(x, y);
    display.println(text.c_str());
}

void clear_and_reset()
{
    clear();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

void clear()
{
    display.clearDisplay();
    display.setCursor(0, 0); // Start at top-left corner
}

void draw()
{
    display.display();
}

#endif
