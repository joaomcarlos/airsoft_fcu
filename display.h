#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"
#include "log.h"

extern int display_ready;
extern Adafruit_SSD1306 display;

int is_display_ready();
void init_display();
void display_text(String text, int size = 1, bool selected = false, int x = -1, int y = -1);
void clear();
void clear_and_reset();
void draw();

#endif
