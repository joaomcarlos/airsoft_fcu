#ifndef LOG_SOURCE
#define LOG_SOURCE

#include <Arduino.h>
#include "../include/display.h"
#include "../include/log.h"

void info(String msg)
{
    #ifdef LOGGER

    #ifdef SERIAL_LOGGER
    if (Serial){
        Serial.println(msg);
    }
    #endif
    
    if (is_display_ready())
    {
        display_text(msg);
        draw();
    }
    #endif
}

#endif
