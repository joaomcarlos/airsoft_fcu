#ifndef ENV_H
#define ENV_H

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define LOGGER 1
#define SERIAL_LOGGER 1
#define LCD 1
#define ACCEL 1

#define trigger_pin 9 // input
#define fire_pin 7    // output

#define down_btn_pin 15
#define up_btn_pin 14
#define sel_btn_pin 16

#endif