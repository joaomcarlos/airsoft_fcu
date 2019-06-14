#ifndef ACCEL_SOURCE
#define ACCEL_SOURCE

#include "log.h"
#include "display.h"
#include "accel.h"

ADXL345 adxl;
AccelCalibrationData calibration;
AccelData accel;

void init_accelarometer()
{
    info("ADXL Power On.");
    //delay(5000);
    adxl = ADXL345();
    adxl.powerOn();
    //adxl.setRangeSetting(2);
    info("Conectado ao ADXL!");
    info("Leu calibracao.");
    EEPROM.get(0, calibration);
}

#define ACCELAROMETER_ORIENTATION_HORIZONTAL 0
#define ACCELAROMETER_ORIENTATION_VERTICAL 1
#define ACCELAROMETER_ORIENTATION ACCELAROMETER_ORIENTATION_HORIZONTAL

void update_accel()
{
    int ax, ay, az;
    adxl.readAccel(&ax, &ay, &az);

    accel.x = ax;
    accel.y = ay;
    accel.z = az;

    double x_Buff = float(accel.x);
    double y_Buff = float(accel.y);
    double z_Buff = float(accel.z);
    accel.raw_pitch = atan2((-x_Buff), sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
    accel.raw_roll = atan2(y_Buff, z_Buff) * 57.3;

#if (ACCELAROMETER_ORIENTATION == ACCELAROMETER_ORIENTATION_VERTICAL)
    accel.raw_roll -= 90;
#endif

    accel.pitch = accel.raw_pitch - calibration.pitch;
    accel.roll = accel.raw_roll - calibration.roll;
}

void calibrate()
{
    clear_and_reset();
    info("A calibrar...");
    delay(2000);
    update_accel();
    calibration = {accel.raw_pitch, accel.raw_roll};
    EEPROM.put(0, calibration);
    info("Calibracao escrita para a EEPROM.");
    delay(1500);
}

void reset_calibration()
{
    calibration = {0.00, 0.00};
    EEPROM.put(0, calibration);

    clear_and_reset();
    info("Calibracao a zero...");
    delay(1500);
}

#endif
