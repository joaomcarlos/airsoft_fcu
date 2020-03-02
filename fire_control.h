#ifndef FIRE_CONTROL_H
#define FIRE_CONTROL_H

#include <Arduino.h>
#include <Pushbutton.h>
#include "display.h"

void init_fire_control();
void update_fire_control();
bool draw_fire_system_status();
void update_fire_control();
void update_fire_control_selector();
int calc_y_in_curve(int x);
void reset_shot_cycle();
void trigger_on();
void trigger_off();
void shoot();
#endif
