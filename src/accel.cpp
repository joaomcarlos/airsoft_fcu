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

#define sampleFreq 1000.0f // sample frequency in Hz
#define betaDef 0.1f	   // 2 * proportional gain

MPU6050 mpu(Wire);

GyroCalibrationData calibration;
float pitch, roll, yaw;

float beta = betaDef;							  // 2 * proportional gain (Kp)
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f; // quaternion of sensor frame relative to auxiliary frame

void init_accelarometer()
{
	info("Accell Power On.");
	//delay(5000);
	Wire.begin();
	mpu.begin();
	//calibrate();
	//mpu.setGyroSensitivity(3);

	info("Conectado ao Accel!");
	info("Leu calibracao.");
	EEPROM.get(0, calibration);
	mpu.setGyroOffsets(calibration.gx, calibration.gy, calibration.gz);
}

void update_accel()
{
	mpu.update();

	MadgwickAHRSupdateIMU(mpu.getAngleX(), mpu.getAngleY(), mpu.getAngleZ(), mpu.getAccX(), mpu.getAccY(), mpu.getAccZ());

	yaw = atan2(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
	pitch = -asin(2.0f * (q1 * q3 - q0 * q2));
	roll = atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);

	/*pitch = mpu.getAngleZ();
    roll = mpu.getAngleX();
    yaw = mpu.getAngleY();

    pitch = mpu.getGyroAngleZ();
    roll = mpu.getGyroAngleX();
    yaw = mpu.getGyroAngleY();*/
}

float getPitch() { return pitch; }
float getRoll() { return roll; }
float getYaw() { return yaw; }

void calibrate()
{
	clear_and_reset();
	info("A calibrar...");
	//delay(2000);
	mpu.calcGyroOffsets();
	calibration = {mpu.getGyroXoffset(), mpu.getGyroYoffset(), mpu.getGyroZoffset()};
	EEPROM.put(0, calibration);
	info("Calibracao escrita para a EEPROM.");
	//delay(1500);
}

void reset_calibration()
{
	/** manual rought reading
    GX : -2.04
    GY : -0.03
    GZ : -0.56
    */
	calibration = {-2.04, -0.03, -0.56};
	EEPROM.put(0, calibration);

	info("Calibracao a zero...");
	delay(1500);
}

void display_accel()
{
	//clear_and_reset();
	display_text("pitch: " + String(getPitch()), 1, false, 0, 0);
	display_text("roll: " + String(getRoll()), 1, false, 0, 8);
	display_text("yaw: " + String(getYaw()), 1, false, 0, 16);
	display_text("dt: " + String(mpu.dt()), 1, false, 0, 24);

#ifdef LCD
	//int r = display.height() / 3;
	//int halfR = r / 2;
	//int posX = map(getRoll(), -45, 45, halfR, display.width() - halfR);
	//int posY = (display.height() / 2) + halfR;

	// draw ball and line
	//display.DrawClosedCurve()
	//display.drawCircle(posX, posY, r, WHITE);
	//display.drawVLine(posX, posY - halfR, r, WHITE);
	//display_draw_line(posX, posY - halfR, r);

	// draw absolute center
	//display.drawVLine(display.width() / 2, 10, display.height(), WHITE);
	//draw();
#endif
}

void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az)
{
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
	{

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	/*q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);*/
	float dt = max(mpu.dt(), 0.001);
	q0 += qDot1 * dt;
	q1 += qDot2 * dt;
	q2 += qDot3 * dt;
	q3 += qDot4 * dt;

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

#endif
