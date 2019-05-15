#ifndef LOG_SOURCE
#define LOG_SOURCE

#include <Arduino.h>
#include "display.h"
#include "log.h"

void info(String msg)
{
    if (Serial)
        Serial.println(msg);

    if (is_display_ready())
    {
        display_text(msg);
        draw();
    }
}

#endif
