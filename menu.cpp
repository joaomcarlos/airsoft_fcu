#ifndef _MENU_CPP_
#define _MENU_CPP_

#include "menu.h"
#include "accel.h"

#define back_opt "Back"
#define prefix_opt "> "

Pushbutton down_btn(15);
Pushbutton up_btn(14);
Pushbutton sel_btn(16);
int menu_opt = 0;

void update_menu(){
    if (sel_btn.getSingleDebouncedPress())
    {
        int res = menu("Menu Principal", "Acelarometro;Option2;Option3;Option4;Option5;Option6;Option7;Option8");
        if (res == 0){
            int r = menu("Acelarometro", "Actual;Alinhar Agora;Zerar calibracao");
            if(r == 0){
                String cal = "pitch:" + String(calibration.pitch) + " roll:" + String(calibration.roll);
                menu("Calibracao Actual", cal);
            }
            if(r == 1){
                calibrate();
            }
            if(r == 2){
                reset_calibration();
            }
        }
        if (res == 1)
            menu("Sub menu opt 2", "Sub opt 2-1;Sub opt 2-2");
    }
}

bool update_menu_opts()
{
    int old_menu_opt = menu_opt;
    if (up_btn.getSingleDebouncedPress())
        menu_opt += 1;
    if (down_btn.getSingleDebouncedPress())
        menu_opt -= 1;
    return old_menu_opt == menu_opt;
}

int menu(String title, String options)
{
    menu_opt = -1;
    int opt_count = 1;

    // count the items in options, split by ;
    int last_search_i = 0;
    while ((last_search_i = options.indexOf(";", last_search_i + 1)) != -1)
        opt_count++;

    // split options by ; into a list (buffer of 10)
    String opt_items[10];
    opt_items[0] = back_opt;

    last_search_i = 0;
    for (byte idx = 1; idx < opt_count + 1; idx++)
    {
        int colon_index = options.indexOf(";", last_search_i + 1);
        String opt_text = (colon_index == -1) ? options.substring(last_search_i) : options.substring(last_search_i, colon_index);
        opt_items[idx] = opt_text;
        last_search_i = colon_index + 1;
    }

    while (true)
    {
        if (sel_btn.getSingleDebouncedPress())
        {
            return menu_opt;
        }

        // save energy by short circuiting
        if (!update_menu_opts())
        {
            delay(100);
            continue;
        }

        // menu wraps around
        if (menu_opt + 1 > opt_count)
            menu_opt = -1;
        if (menu_opt < -1)
            menu_opt = opt_count - 1;

        clear_and_reset();
        display.drawFastHLine(0, 6, 128, WHITE);
        display_text(title, 1, 0, 0);

        int start = (menu_opt > 1) ? menu_opt - 1 : 0;
        for (byte idx = start; idx < opt_count + 1; idx++)
        {
            display_text(prefix_opt + opt_items[idx], 1, menu_opt == idx - 1);
        }

        draw();
        delay(100);
    }
}

#endif
