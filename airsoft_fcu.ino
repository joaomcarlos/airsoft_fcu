#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_ADXL345.h>
#include <Tasker.h>

#include "log.h"
#include "display.h"
#include "accel.h"
#include "menu.h"

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define trigger_pin 9 // input
#define fire_pin 7 // output

Tasker tasker;

void setup()
{
  Serial.begin(9600);
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

bool blinker = false;
void loop()
{
  tasker.loop();
}

void blink(){
  digitalWrite(fire_pin, blinker ? HIGH : LOW);
  blinker = !blinker;
}
