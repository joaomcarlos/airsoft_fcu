#ifndef ACCEL_SOURCE
#define ACCEL_SOURCE

#include "accel.h"

ADXL345 adxl;
AccelCalibrationData calibration;

void init_accelarometer()
{
    info("ADXL Power On.");
    //delay(5000);
    adxl = ADXL345();
    adxl.powerOn();
    adxl.setRangeSetting(3);
    info("Conectado ao ADXL!");

    if (CALIBRATE)
    {
        info("A calibrar...");
        delay(2000);
        int x, y, z;
        adxl.readAccel(&x, &y, &z);
        calibration = {x, y, z};
        EEPROM.put(0, calibration);
        info("Calibracao escrita para a EEPROM.");
    }
    else
    {
        info("Leu calibracao.");
        EEPROM.get(0, calibration);
    }
}

void getAccel(int &x, int &y, int &z)
{
    adxl.readAccel(&x, &y, &z);
    x -= calibration.x;
    y -= calibration.y;
    z -= calibration.z;
}

#endif
