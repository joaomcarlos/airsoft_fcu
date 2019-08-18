#include "env.h"

#include <Arduino.h>
#include <Tasker.h>
#include <SPI.h>
#include <EEPROM.h>
 
#include "include/log.h"
#include "include/display.h"
#include "include/accel.h"
#include "include/menu.h"

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

Tasker tasker;

void setup()
{
  #ifdef SERIAL_LOGGER
  Serial.begin(9600);
  #endif

  tasker.setTimeout(post_setup, 500);
}

void post_setup(){
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
void blink(){
  digitalWrite(fire_pin, blinker ? HIGH : LOW);
  blinker = !blinker;
}
