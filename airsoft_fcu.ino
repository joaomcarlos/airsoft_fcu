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

//Tasker tasker;

void setup()
{
  tasker.setTimeout(post_setup, 500);
}

void post_setup()
{
  init_display();
  init_accelarometer();

  pinMode(fire_pin, OUTPUT);

  info("A iniciar ...");
  //delay(3000);
  clear_and_reset();

  tasker.setInterval(update_accel, 99);
  tasker.setInterval(update_menu, 100);
  tasker.setInterval(display_accel, 100);
  //tasker.setInterval(blink, 500);
}

void loop()
{
  tasker.loop();
}

bool blinker = false;
void blink()
{
  info("blink!");
  digitalWrite(fire_pin, blinker ? HIGH : LOW);
  blinker = !blinker;
}
