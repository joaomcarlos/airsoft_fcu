#ifndef ACCEL_SOURCE
#define ACCEL_SOURCE

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MPU6050_tockn.h>
#include <MahonyAHRS.h>
#include <MadgwickAHRS.h>
#include <Wire.h>

#include "../include/log.h"
#include "../include/display.h"
#include "../include/accel.h"

MPU6050 mpu(Wire);

//Mahony filter;
Madgwick filter;
GyroCalibrationData calibration;

void init_accelarometer()
{
	info("Accell Power On.");
	//delay(5000);
	Wire.begin();
	mpu.begin();
	//mpu.calcGyroOffsets();
	//calibrate();
	//mpu.setGyroSensitivity(3);

	info("Conectado ao Accel!");
	info("Leu calibracao.");
	EEPROM.get(0, calibration);
	mpu.setGyroOffsets(calibration.gx, calibration.gy, calibration.gz);

	filter.begin(100); // filter to expect 100 measurements per second

	int i;
	for (i=0; i<3000; i++){
		update_accel();
	}
}

void update_accel()
{
	mpu.update();

	float gx = mpu.getGyroX();
	float gy = mpu.getGyroY();
	float gz = mpu.getGyroZ();

	float ax = mpu.getAccX();
	float ay = mpu.getAccY();
	float az = mpu.getAccZ();

	// Mahony expects gyroscope in radians/second
	float gyroScale = 3.14159f / 180.0f;
	//gx = gx * gyroScale;
	//gy = gy * gyroScale;
	//gz = gz * gyroScale;

	filter.updateIMU(gx, gy, gz, ax, ay, az);
}

float getRoll() { return filter.getRoll(); }
float getPitch() { return filter.getPitch(); }
float getYaw() { return filter.getYaw(); }

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

#endif
