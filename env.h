#ifndef ENV_H
#define ENV_H

/*  
    Comentar mesmo, para retirar o codigo
*/

//#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define LOGGER
#define SERIAL_LOGGER
#define LCD
#define ACCEL
#define TASKER_MAX_TASKS 10
#define TASKER_MAX_PRIO_TASKS 5

#define trigger_btn_pin 7 // input
#define fire_pin A3       // output (A3 = 21)

#define down_btn_pin 10
#define up_btn_pin 16
#define sel_btn_pin 14

#define safe_btn_pin 8
#define full_btn_pin 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define FONT_SIZE 8

#endif