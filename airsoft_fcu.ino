#include <math.h>
#include <Pushbutton.h>
#include <Narcoleptic.h>

// read https://digistump.com/wiki/digispark/tutorials/basics
// all pins can output, but pin5 is only 3v
// all pins can input, but if you need pullup registor, nullify the internal led
// pins 3 and 4 are usb pins and have internal 1.5k ohm resistor, need to overpower it to pulldown

#define fire_pin 1	  // output
#define trigger_pin 0 // input
#define safe_pin 2	  // input
#define full_pin 3	  // input

#define down_btn_pin 10 // input
#define up_btn_pin 16	// input
#define sel_btn_pin 14	// input

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FONT_SIZE 8

int mode = 2;
int magazine_size = 130;
int shots_left = 0;
bool frenzy = false;

void setup()
{
	init_display();
	init_fire_display_logic();
}
void loop()
{
	perform_draw_call();
	shots_left--;
	if (shots_left == -1)
		shots_left = magazine_size;
}

#ifndef DISPLAY_LOGIC

#include "lcdgfx.h"

DisplaySSD1306_128x64_I2C display(-1);
bool display_ready = false;
typedef bool (*DrawCallback)(void);
DrawCallback currentDrawCall;

void set_draw_callback(DrawCallback cb);

void init_display()
{
	display.begin();
	display.clear();
	display.fill(0x00);
	display.setFixedFont(ssd1306xled_font6x8);

	display_ready = 1;
	set_draw_callback(draw_fire_system_status);
}

void perform_draw_call()
{
	if (currentDrawCall)
	{
		currentDrawCall();
	}
}

void set_draw_callback(DrawCallback cb)
{
	currentDrawCall = cb;
}

#endif

#ifndef FIRING_DISPLAY_LOGIC
#define BAR_HEIGHT 12
#define BAR_MARGIN_TOP (SCREEN_HEIGHT - 12)
//#define BAR_MARGIN_TOP 31
#define BAR_WIDTH 2
#define BAR_MARGIN_EACH 2

const char *fire_mode_text[] = {"SAFE", "SINGLE", "FULL"};
bool fire_control_display_needs_redraw = true;
int old_mode = -1;
int old_shots_left = -1;

void init_fire_display_logic()
{
	shots_left = magazine_size;
}

bool draw_fire_system_status()
{
	if (fire_control_display_needs_redraw || mode != old_mode)
	{
		display.printFixedN(0, 0, fire_mode_text[mode], STYLE_NORMAL, 0);
		old_mode = mode;
	}

	if (fire_control_display_needs_redraw || shots_left != old_shots_left)
	{
		print_shot_counter();
		draw_progress_bar();
		old_shots_left = shots_left;
	}
	fire_control_display_needs_redraw = false;
	return true;
}

void print_shot_counter()
{
#define shots_left_margin_top 16
#define char3_height (8 * 3) + 1
#define char3_width (6 * 3) + 1
	static char ammoCount[5] = "00000";

	display.positiveMode();
	sprintf(ammoCount, "%03u", shots_left);

	display.printFixedN(0, shots_left_margin_top, ammoCount, STYLE_BOLD, 2);
}

void draw_progress_bar()
{
	int x = 0;
	int max_bars = ceil(SCREEN_WIDTH / (BAR_WIDTH + BAR_MARGIN_EACH));

	int bars = map(shots_left, 0, magazine_size, 0, max_bars);
	static int last_bars = 0;

	if (bars > last_bars)
	{
		display.positiveMode();
		for (x = 0; x < bars; x++)
		{
			int xs = BAR_WIDTH * x + BAR_MARGIN_EACH * x;
#define i_height 2
#define i_spacing 3
			if (BAR_WIDTH + BAR_MARGIN_EACH < 4)
			{
				//display.drawVLine(xs, calc_y_in_curve(xs) + BAR_MARGIN_TOP, 32 + BAR_MARGIN_TOP);
				display.drawVLine(xs, BAR_MARGIN_TOP, BAR_MARGIN_TOP + i_height);
				display.drawVLine(xs, BAR_MARGIN_TOP + i_height + i_spacing, BAR_MARGIN_TOP + BAR_HEIGHT);
			}
			else
			{

				display.fillRect(xs, BAR_MARGIN_TOP, xs + BAR_WIDTH, BAR_MARGIN_TOP + i_height);
				display.fillRect(xs, BAR_MARGIN_TOP + i_height + i_spacing, xs + BAR_WIDTH, BAR_MARGIN_TOP + BAR_HEIGHT);

				//display.fillRect(xs, BAR_MARGIN_TOP + BAR_HEIGHT - calc_y_in_curve(xs), xs + BAR_WIDTH, BAR_MARGIN_TOP + BAR_HEIGHT);
			}
		}
	}
	// remove the old bar
	display.negativeMode();
	for (x = max_bars; x > bars; x--)
	{
		int xs = BAR_WIDTH * x + BAR_MARGIN_EACH * x;
		if (BAR_WIDTH + BAR_MARGIN_EACH < 4)
		{
			display.drawVLine(xs, BAR_MARGIN_TOP + BAR_HEIGHT - calc_y_in_curve(xs), BAR_MARGIN_TOP + BAR_HEIGHT);
		}
		else
		{
			display.fillRect(xs, BAR_MARGIN_TOP + BAR_HEIGHT - calc_y_in_curve(xs), xs + BAR_WIDTH, BAR_MARGIN_TOP + BAR_HEIGHT);
		}
	}
	display.positiveMode();
	last_bars = bars;
}
// design it here https://www.desmos.com/calculator
int calc_y_in_curve(int x)
{
	return BAR_HEIGHT; // always full
					   //return round(3 + 0.0015f * pow(x, 2)); // follow a curve
}

#endif

#ifndef FIRING_LOGIC

// as discussed here: https://www.facebook.com/groups/WolverineAirsoftSMP/2434754936777579

// engine cycle control
#define sane_dwell 130	   // default 30 or Polarstar:19 // how nozzle takes to fire and move back, to allow BB load
#define sane_cooldown 40   // default 50 or Polarstar:12 // how nozzle takes to move forward and seat the BB before next shot
#define insane_dwell 28	   // default 30 or Polarstar:19 // how nozzle takes to fire and move back, to allow BB load
#define insane_cooldown 19 // default 50 or Polarstar:12 // how nozzle takes to move forward and seat the BB before next shot

// trigger automation
#define binary_trigger_time 130			   // less than this long pressed and double tap, more and its single shot only
#define single_supress_time 700			   // more than this long pressed and fire once every
#define single_supress_time_frenzy 300	   // as above but in frenzy
#define single_supress_cycle 500		   // when in "single suppress" how much time between shots
#define single_supress_cycle_in_frenzy 200 // as above but in frenzy
#define full_auto_trigger_time 500		   // more than this long pressed and full auto
#define full_auto_trigger_time_frenzy 130  // more than this long pressed and full auto
#define frenzy_timeout 5000				   // double shots activate frenzy for this time amount

// battery saver
#define long_sleep_time 15 * 60 * 1000 // after 15 minutes, enter long sleep, may take up to 30 seconds to wake up (usually less)
#define long_sleep_cycle 30 * 1000	   // when in safe for a long period, sleep longer to try and keep the battery from dying
#define short_sleep_cycle 1000		   // when in safe, sleep a bit to save power

// fire modes
#define SAFE 0
#define SEMI 1
#define FULL 2

Pushbutton trigger_btn(trigger_pin);
Pushbutton safe_btn(safe_pin);
Pushbutton full_btn(full_pin);
void firing_setup()
{
	pinMode(fire_pin, OUTPUT);
	digitalWrite(fire_pin, LOW); // keep it low
	setup_power_saving();
}

long lastNonSleepCycle = 0;
long lastTriggerPress = 0;
void firing_loop()
{
	mode = get_firing_mode();
	if (mode == SAFE)
	{
		if (millis() - lastNonSleepCycle > long_sleep_time)
			Narcoleptic.delay(long_sleep_time); // During this time power consumption is minimised
		else
			Narcoleptic.delay(short_sleep_cycle); // During this time power consumption is minimised
		return;
	}

	lastNonSleepCycle = millis(); // to aid in detecting long sleeps

	if (!trigger_btn.isPressed())
		return;

	if (millis() - lastTriggerPress > frenzy_timeout)
		frenzy = false;

	lastTriggerPress = millis();
	switch (mode)
	{
	case SAFE:
		break;
	case SEMI:
		perform_semi_logic();
		break;
	case FULL:
		perform_full_auto_logic();
		break;
	}
}

void fire_once()
{
	digitalWrite(fire_pin, HIGH);
	delay((mode == FULL) ? insane_dwell : sane_dwell);
	digitalWrite(fire_pin, LOW);
	delay((mode == FULL) ? insane_cooldown : sane_cooldown);
}

// 0-SAFE, 1-SEMI, 2-FULL
int get_firing_mode()
{
	if (safe_btn.isPressed())
		return SAFE;
	if (full_btn.isPressed())
		return FULL;
	return SEMI;
}

void perform_semi_logic()
{
	// in frenzy, skip the "normal" logic
	if (frenzy)
	{
		perform_semi_logic_frenzy();
		return;
	}

	// by doing it this way, we can quickly short circuit and get a faster shot
	digitalWrite(fire_pin, HIGH);

	// check if its a long press
	while (trigger_btn.isPressed())
	{
		if (millis() - lastTriggerPress >= single_supress_time)
		{
			// cycle it
			digitalWrite(fire_pin, LOW);
			delay(sane_cooldown);

			// if its a long press, enter "single suppress" mode
			while (trigger_btn.isPressed())
			{
				// fire once every single_supress_cycle
				fire_once();
				delay(single_supress_cycle);
			}
			return; // return ealier to avoid extra binary trigger shot
		}
	}

	long pull_time = millis() - lastTriggerPress;
	// binary trigger
	if (pull_time <= binary_trigger_time)
	{
		// wait at least the minimum dwell to cycle properly
		if (pull_time < insane_dwell)
			delay(insane_dwell - pull_time);

		// reset the shot and wait for cooldown
		digitalWrite(fire_pin, LOW);
		delay(insane_cooldown);

		frenzy = true;
		fire_once();
		return;
	}

	// normal shot release...

	// wait at least the minimum dwell to cycle properly
	if (pull_time < sane_dwell)
		delay(sane_dwell - pull_time);

	// complete the cycle
	digitalWrite(fire_pin, LOW);
	delay(sane_cooldown);
}

void perform_semi_logic_frenzy()
{
	digitalWrite(fire_pin, HIGH);
	delay(sane_dwell);
	digitalWrite(fire_pin, LOW);
	delay(insane_cooldown); // take advantage to start cooling down asap

	// check if its a long press
	while (trigger_btn.isPressed())
	{
		if (millis() - lastTriggerPress >= single_supress_time_frenzy)
		{
			// if its a long press, enter "single suppress" mode
			while (trigger_btn.isPressed())
			{
				// fire once every single_supress_cycle
				digitalWrite(fire_pin, HIGH);
				delay(sane_dwell);
				digitalWrite(fire_pin, LOW);
				delay(single_supress_cycle_in_frenzy);
			}
			return; // return ealier to avoid extra binary trigger shot
		}
	}

	// binary trigger assist
	if (millis() - lastTriggerPress <= binary_trigger_time)
	{
		digitalWrite(fire_pin, HIGH);
		delay(sane_dwell);
		digitalWrite(fire_pin, LOW);
		delay(sane_cooldown); // its fine since our human trigger cant match
	}
}

void perform_full_auto_logic()
{
	// in frenzy, skip the "normal" logic
	if (frenzy)
	{
		perform_full_auto_logic_frenzy();
		return;
	}

	// 3 round burst on short press
	fire_once();
	fire_once();
	fire_once();

	// check if its a long press
	while (trigger_btn.isPressed())
	{
		delay(10);
		if (millis() - lastTriggerPress >= full_auto_trigger_time_frenzy)
		{
			// if its a long press, enter "full auto" mode
			while (trigger_btn.isPressed())
			{
				// fire continously
				fire_once();
			}
		}
	}
}

void perform_full_auto_logic_frenzy()
{
	// 3 round burst on short press
	fire_once();
	fire_once();
	fire_once();

	// check if its a long press
	while (trigger_btn.isPressed())
	{
		delay(10);
		if (millis() - lastTriggerPress >= full_auto_trigger_time)
		{
			// if its a long press, enter "full auto" mode
			while (trigger_btn.isPressed())
			{
				// fire continously
				fire_once();
			}
		}
	}
}
#endif

void setup_power_saving()
{
	// Narcoleptic.disableMillis(); Do not disable millis - we need it for our delay() function.
	//Narcoleptic.disableTimer1();
	Narcoleptic.disableTimer2();
	Narcoleptic.disableSerial();
	Narcoleptic.disableADC(); // !!! enabling this causes >100uA consumption !!!
	Narcoleptic.disableWire();
	Narcoleptic.disableSPI();

	// Another tweaks to lower the power consumption
	ADCSRA &= ~(1 << ADEN); //Disable ADC
	ACSR = (1 << ACD);		//Disable the analog comparator
							//
							//
							// ATtiny25/45/85
							//Disable digital input buffers on all ADC0-ADC3 + AIN1/0 pins
							//DIDR0 = (1<<ADC0D)|(1<<ADC2D)|(1<<ADC3D)|(1<<ADC1D)|(1<<AIN1D)|(1<<AIN0D);
}