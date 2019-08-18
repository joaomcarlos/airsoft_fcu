#ifndef _ACCEL_H_
#define _ACCEL_H_

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_ADXL345.h>
#include "accel.h"
#include "log.h"

struct AccelCalibrationData
{
    double pitch;
    double roll;
};

struct AccelData
{
    int x;
    int y;
    int z;
    double raw_pitch;
    double raw_roll;
    double pitch;
    double roll;
};

extern ADXL345 adxl;
extern AccelCalibrationData calibration;
extern AccelData accel;

void init_accelarometer();
void update_accel();
void calibrate();
void reset_calibration();
void display_accel();

#endif
