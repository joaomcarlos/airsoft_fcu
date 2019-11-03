#include "env.h"

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>

#include "include/tasker.h"
#include "include/global.h"
#include "include/log.h"
#include "include/display.h"
#include "include/accel.h"
#include "include/menu.h"

void setup()
{
  delay(1500); // else it wont start with battery power
  init_display();
  init_accelarometer();

  pinMode(fire_pin, OUTPUT);

  //tasker.setInterval(update_accel, 10);
  tasker.setInterval(update_menu, 100);
  tasker.setInterval(perform_draw_call, 40);

  info("A iniciar ...");
  delay(1500);
  clear_and_reset();
}

void loop()
{
  update_accel(); // highest priority to avoid drift
  tasker.loop();
}

bool blinker = false;
void blink()
{
  info("blink!");
  digitalWrite(fire_pin, blinker ? HIGH : LOW);
  blinker = !blinker;
}
