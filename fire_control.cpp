#ifndef _FIRE_CONTROL_CPP_
#define _FIRE_CONTROL_CPP_

#include <math.h>
#include "env.h"
#include "global.h"
#include "display.h"
#include "fire_control.h"

Pushbutton trigger_btn(trigger_btn_pin);
Pushbutton safe_btn(safe_btn_pin);
Pushbutton full_btn(full_btn_pin);

#define DWELL 30
#define POST_DWELL 20

#define SAFE 0
#define SINGLE 1
#define FULL 2

const char *fire_mode_text[] = {"SAFE", "SINGLE", "FULL"};
char fire_mode = SAFE;
int magazine_size = 130;
int shots_left = 0;
bool shooting = false;
bool cycling_shot = false;
bool fire_control_display_changed = true;

void init_fire_control()
{
	pinMode(fire_pin, OUTPUT);
	//trigger_off();
	tasker.setInterval(update_fire_control, 73);
	tasker.setInterval(update_fire_control_selector, 213);
	shots_left = magazine_size;
}

bool waiting_trigger_release = false;
void update_fire_control()
{
	bool trigger_pressed = trigger_btn.isPressed();

	switch (fire_mode)
	{
	case SAFE:
		break;
	case SINGLE:
		if (trigger_pressed && !waiting_trigger_release)
		{
			waiting_trigger_release = true;
			if (!cycling_shot)
				shoot();
		}
		if (!trigger_pressed)
			waiting_trigger_release = false;
		break;
	case FULL:
		if (trigger_pressed && !cycling_shot)
			shoot();
		break;

	default:
		break;
	}

	if (shots_left <= 0)
	{
		shots_left = magazine_size;
		fire_control_display_changed = true;
	}
}
void update_fire_control_selector()
{
	char f_mode = fire_mode;
	fire_mode = safe_btn.isPressed() ? SAFE : full_btn.isPressed() ? FULL : SINGLE;
	fire_control_display_changed = fire_mode != f_mode;
}

bool draw_fire_system_status()
{
	if (!fire_control_display_changed)
		return false;
	clear_and_reset();
	canvas.printFixed(0, 0, fire_mode_text[fire_mode], STYLE_NORMAL);
	display_text(String(shots_left), 4, false, 0, 8);
	//display_text("Tasks: " + String(tasker.pendingTasks()), 1, false, 30, 0);
	//display_text("PTasks: " + String(shooting_tasker.pendingTasks()), 1, false, 30, 8);
	//display_text(String(trigger_btn.isPressed() ? "TRIGGER" : ""), 1, false, 30, 8);
	//display_text(String(cycling_shot ? "CYCLING" : ""), 1, false, 30, 16);

	int bar_width = 1;
	int bar_margin = 0;

	int x = 0;
	int max_bars = ceil(SCREEN_WIDTH / (bar_width + bar_margin));

	int bars = map(shots_left, 0, magazine_size, 0, max_bars);
	for (x = 0; x < bars; x++)
	{
		int xs = bar_width * x + bar_margin * x;
		if (bar_width + bar_margin < 4)
			canvas.drawVLine(xs, calc_y_in_curve(xs), SCREEN_HEIGHT);
		else
			canvas.fillRect(xs, calc_y_in_curve(xs), xs + bar_width, SCREEN_HEIGHT);
	}

	fire_control_display_changed = false;
	return true;
}

// design it here https://www.desmos.com/calculator
int calc_y_in_curve(int x)
{
	return round(SCREEN_HEIGHT - (3 + 0.0015f * pow(x, 2)));
}

void shoot()
{
	cycling_shot = true;
	shooting_tasker.setTimeout(trigger_on, 1);
	shooting_tasker.setTimeout(trigger_off, DWELL);
	shooting_tasker.setTimeout(reset_shot_cycle, POST_DWELL);
	fire_control_display_changed = true;
}
void trigger_on()
{
	shots_left--;
	digitalWrite(fire_pin, 0);
}
void trigger_off()
{
	digitalWrite(fire_pin, 1);
}
void reset_shot_cycle()
{
	cycling_shot = false;
}

#endif