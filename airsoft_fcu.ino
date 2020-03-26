#include <Arduino.h>

#include "env.h"
#include "global.h"
#include "tasker.h"
#include "display.h"
#include "fire_control.h"

void setup()
{
	//if (!Serial)
	//	delay(1500); // else it wont start with battery power
	if (!Serial)
		Serial.begin(9600);
	Serial.println("Hi there");
	init_display();

	init_fire_control();
}

void loop()
{
	// this should also take priority
	shooting_tasker.loop();
	tasker.loop();
}