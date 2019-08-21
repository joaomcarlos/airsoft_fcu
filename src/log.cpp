#ifndef LOG_SOURCE
#define LOG_SOURCE

#include <Arduino.h>
#include "../include/display.h"
#include "../include/log.h"

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

void info(String msg)
{
    if (is_display_ready())
    {
        display_text(msg);
        draw();
    }

#ifdef SERIAL_LOGGER
    serial_log("info -> " + msg);
#endif
}
#else
void info(String msg) {} // do nothing
#endif

#endif
