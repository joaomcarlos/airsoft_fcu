
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_ADXL345.h>

#include "log.h"
#include "display.h"
#include "accel.h"
#include "menu.h"

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define trigger_pin 7

void setup()
{
  Serial.begin(9600);
  init_display();
  init_accelarometer();

  pinMode(trigger_pin, OUTPUT);

  info("A iniciar ...");
  //delay(3000);
  clear_and_reset();
}

bool blinker = false;
void loop()
{
  update_accel();
  digitalWrite(trigger_pin, blinker ? HIGH : LOW);
  blinker = !blinker;

  update_menu();
  display_accel();
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
