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
  tasker.setInterval(blink, 500);
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

void display_accel()
{
  clear();
  display_text("pitch:" + String(accel.pitch) + " roll:" + String(accel.roll));

  int r = display.height() / 3;
  int halfR = r / 2;
  int posX = map(accel.roll, -45, 45, halfR, display.width() - halfR);
  int posY = (display.height() / 2) + halfR;

  // draw ball and line
  display.drawCircle(posX, posY, r, WHITE);
  display.drawFastVLine(posX, posY - halfR, r, WHITE);
  display.drawFastHLine(posX - halfR, posY, r, WHITE);

  // draw absolute center
  display.drawFastVLine(display.width() / 2, 10, display.height(), WHITE);
  draw();
}
