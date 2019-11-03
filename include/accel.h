#ifndef _ACCEL_H_
#define _ACCEL_H_

struct GyroCalibrationData
{
    float gx;
    float gy;
    float gz;
};

extern GyroCalibrationData calibration;

extern float beta;           // algorithm gain
extern float q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame
extern float pitch, roll, yaw;

void init_accelarometer();
void update_accel();
void calibrate();
void reset_calibration();
bool display_accel();
float invSqrt(float x);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

#endif
