#ifndef _MENU_H_
#define _MENU_H_

#include <Arduino.h>
#include <Pushbutton.h>
#include "display.h"

// https://github.com/pololu/pushbutton-arduino
extern Pushbutton down_btn;
extern Pushbutton up_btn;
extern Pushbutton sel_btn;
extern int menu_opt;

void update_menu();




#endif
