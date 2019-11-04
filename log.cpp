#ifndef LOG_SOURCE
#define LOG_SOURCE
#define DISPLAY_MAX_LINES 4

#include <Arduino.h>
#include "display.h"
#include "log.h"

#ifdef LOGGER
#ifdef SERIAL_LOGGER
void serial_log(String msg)
{
    if (!Serial)
        Serial.begin(9600);
    if (Serial)
        Serial.println(msg);
}
#endif

String display_log_buffer[DISPLAY_MAX_LINES];
void info(String msg)
{
    int i;
    for (i = 0; i < DISPLAY_MAX_LINES - 1; i++)
    {
        display_log_buffer[i] = display_log_buffer[i + 1];
    }
    display_log_buffer[DISPLAY_MAX_LINES - 1] = msg;

    clear_and_reset();
    for (i = 0; i < DISPLAY_MAX_LINES; i++)
    {
        display_text(display_log_buffer[i], 1, false, 0, i * FONT_SIZE);
    }
    draw();

#ifdef SERIAL_LOGGER
    serial_log("info -> " + msg);
#endif
}
#else
void info(String msg) {} // do nothing
#endif

#endif
