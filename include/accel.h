#ifndef _ACCEL_H_
#define _ACCEL_H_

struct GyroCalibrationData
{
    float x;
    float y;
    float z;
};

struct AccelData
{
    int x;
    int y;
    int z;
    float raw_pitch;
    float raw_roll;
    float raw_yaw;
    float pitch;
    float roll;
    float yaw;
};

extern GyroCalibrationData calibration;
extern AccelData accel;

void init_accelarometer();
void update_accel();
void calibrate();
void reset_calibration();
void display_accel();

#endif
