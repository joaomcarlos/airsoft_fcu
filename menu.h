#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Pushbutton.h>
#include "display.h"

extern Pushbutton down_btn;
extern Pushbutton up_btn;
extern Pushbutton sel_btn;
extern int menu_opt;

bool update_menu_opts();
int menu(String title, String options);
#endif
