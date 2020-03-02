#include <Arduino.h>

#include "env.h"
#include "global.h"
#include "tasker.h"
#include "log.h"
#include "display.h"
#include "accel.h"
#include "menu.h"
#include "fire_control.h"

void setup()
{
	//if (!Serial)
	//	delay(1500); // else it wont start with battery power
	init_display();
	init_accelarometer();

	init_menu();
	init_fire_control();

	info("A iniciar ...");
	//if (!Serial)
	//		delay(1500);
}

void loop()
{
	//update_accel(); // highest priority to avoid drift

	// this should also take priority
	shooting_tasker.loop();
	tasker.loop();
}