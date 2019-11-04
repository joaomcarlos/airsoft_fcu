#ifndef _FIRE_CONTROL_CPP_
#define _FIRE_CONTROL_CPP_

#include "env.h"
#include "fire_control.h"

Pushbutton trigger_btn(trigger_btn_pin);
Pushbutton safe_btn(safe_btn_pin);
Pushbutton full_btn(full_btn_pin);

void display_fire_system_status()
{
    clear_and_reset();
    display_text("trigger_btn: " + String(trigger_btn.isPressed() ? "Pressed!" : "        "), 1, false, 0, 0);
    display_text("safe_btn: " + String(safe_btn.isPressed() ? "Pressed!" : "        "), 1, false, 0, 8);
    display_text("full_btn: " + String(full_btn.isPressed() ? "Pressed!" : "        "), 1, false, 0, 16);
}

#endif