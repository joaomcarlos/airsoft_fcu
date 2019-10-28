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

#define trigger_pin 7 // input
#define fire_pin 21   // output (A3)

#define down_btn_pin 16
#define up_btn_pin 10
#define sel_btn_pin 14

#define safe_btn_pin 8
#define full_btn_pin 6

#define FONT_SIZE 8

#endif