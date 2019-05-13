#ifndef MENU_SOURCE
#define MENU_SOURCE

#include "menu.h"

Pushbutton down_btn(15);
Pushbutton up_btn(14);
Pushbutton sel_btn(16);
int menu_opt = 0;

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
    opt_items[0] = "Back";

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

        clear();
        display.drawFastHLine(0, 6, 128, WHITE);
        queueText(title, 1, 0, 0);

        int start = (menu_opt > 1) ? menu_opt - 1 : 0;
        for (byte idx = start; idx < opt_count + 1; idx++)
        {
            queueText("> " + opt_items[idx], 1, menu_opt == idx - 1);
        }

        draw();
        delay(100);
    }
}

#endif
