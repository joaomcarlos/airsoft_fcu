#ifndef ACCEL_SOURCE
#define ACCEL_SOURCE

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

#include "../include/log.h"
#include "../include/display.h"
#include "../include/accel.h"

MPU6050 mpu(Wire);

GyroCalibrationData calibration;
AccelData accel;

void init_accelarometer()
{
    info("Accell Power On.");
    //delay(5000);
    Wire.begin();
    mpu.begin();

    reset_calibration();

    info("Conectado ao Accell!");
    info("Leu calibracao.");
    EEPROM.get(0, calibration);
    mpu.setGyroOffsets(calibration.x, calibration.y, calibration.z);
}

#define ACCELAROMETER_ORIENTATION_HORIZONTAL 0
#define ACCELAROMETER_ORIENTATION_VERTICAL 1
#define ACCELAROMETER_ORIENTATION ACCELAROMETER_ORIENTATION_VERTICAL

void update_accel()
{
    mpu.update();

    accel.raw_pitch = mpu.getGyroAngleZ();
    accel.raw_roll = mpu.getGyroAngleX();

#if (ACCELAROMETER_ORIENTATION == ACCELAROMETER_ORIENTATION_VERTICAL)
    accel.raw_roll -= 90;
#endif

    accel.pitch = accel.raw_pitch;
    accel.roll = accel.raw_roll;
}

void calibrate()
{
    info("A calibrar...");
    delay(2000);
    mpu.calcGyroOffsets();
    calibration = {mpu.getGyroXoffset(), mpu.getGyroYoffset(), mpu.getGyroZoffset()};
    EEPROM.put(0, calibration);
    info("Calibracao escrita para a EEPROM.");
    delay(1500);
}

void reset_calibration()
{
    /** manual rought reading
    X : -2.04
    Y : -0.03
    Z : -0.56
    */
    calibration = {-2.04, -0.03, -0.56};
    EEPROM.put(0, calibration);

    info("Calibracao a zero...");
    delay(1500);
}

void display_accel()
{
    clear_and_reset();
    display_text("pitch:" + String(accel.pitch) + " roll:" + String(accel.roll), 1, false, 0, 0);

#ifdef LCD
    int r = display.height() / 3;
    int halfR = r / 2;
    int posX = map(accel.roll, -45, 45, halfR, display.width() - halfR);
    int posY = (display.height() / 2) + halfR;

    // draw ball and line
    //display.DrawClosedCurve()
    //display.drawCircle(posX, posY, r, WHITE);
    //display.drawVLine(posX, posY - halfR, r, WHITE);
    display_draw_line(posX, posY - halfR, r);

    // draw absolute center
    //display.drawVLine(display.width() / 2, 10, display.height(), WHITE);
    draw();
#endif
}

#endif
