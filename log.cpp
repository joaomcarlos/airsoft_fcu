#ifndef LOG_SOURCE
#define LOG_SOURCE

#include <Arduino.h>
#include "display.h"
#include "log.h"

void info(String msg)
{
    if (Serial)
        info(msg);

    if (displayReady())
    {
        queueText(msg);
        draw();
    }
}

#endif
