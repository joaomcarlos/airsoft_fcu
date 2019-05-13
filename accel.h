#ifndef ACCEL_H
#define ACCEL_H

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_ADXL345.h>
#include "accel.h"
#include "log.h"

#define CALIBRATE 0
struct AccelCalibrationData
{
    int x;
    int y;
    int z;
};

extern ADXL345 adxl;
extern AccelCalibrationData calibration;

void init_accelarometer();
void getAccel(int &x, int &y, int &z);

#endif
