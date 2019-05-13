
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
// https://github.com/pololu/pushbutton-arduino

void setup()
{
  Serial.begin(9600);
  init_display();
  init_accelarometer();

  pinMode(trigger_pin, OUTPUT);

  info("A iniciar...");
  delay(1000);
}

bool blinker = false;
void loop()
{
  update_menu_opts();
  if (sel_btn.getSingleDebouncedPress())
  {
    int res = menu("Main Menu", "Option1;Option2;Option3;Option4;Option5;Option6;Option7;Option8");
    if (res == 0)
      menu("Sub menu opt 1", "Sub opt 1-1;Sub opt 1-2");
    if (res == 1)
      menu("Sub menu opt 2", "Sub opt 2-1;Sub opt 2-2");
  }

  int x, y, z;
  getAccel(x, y, z);
  displayAccel(x, y, z);
  delay(100);
  digitalWrite(trigger_pin, blinker ? HIGH : LOW);
  blinker = !blinker;
}

void displayAccel(int x, int y, int z)
{
  clear();
  info("Menu Opt: " + String(menu_opt));
  info("x: " + String(x));
  info("y: " + String(y));
  draw();
}
