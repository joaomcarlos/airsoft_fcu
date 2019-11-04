#ifndef _MENU_CPP_
#define _MENU_CPP_

#include "env.h"
#include "menu.h"
#include "accel.h"
#include "fire_control.h"

#define back_opt "Back"
#define prefix_opt "> "

#define MENU_OPCOES_ROOT -1

#define MENU_OPCOES_PRINCIPAL 1
#define MENU_OPCOES_ACELAROMETRO 2

#define MENU_OPCOES_ACELAROMETRO_ACTUAL 21
#define MENU_OPCOES_ACELAROMETRO_ALINHAR 22
#define MENU_OPCOES_ACELAROMETRO_ZERAR_CALIBRACAO 23

Pushbutton down_btn(down_btn_pin);
Pushbutton up_btn(up_btn_pin);
Pushbutton sel_btn(sel_btn_pin);

int cur_menu = -1;
int menu_opt = -1;
bool menu_changed = true;

void change_menu(int menu)
{
    cur_menu = menu;
    menu_opt = -1;
    menu_changed = true;
}

bool drawMenu(String title, String options);
void handle_menu_action();
void handle_menu_screen_logic();

void update_menu()
{
    if (up_btn.getSingleDebouncedPress())
    {
        menu_opt += 1;
        menu_changed = true;
    }
    if (down_btn.getSingleDebouncedPress())
    {
        menu_opt -= 1;
        menu_changed = true;
    }

    if (sel_btn.getSingleDebouncedPress())
    {
        handle_menu_action();
    }

    handle_menu_screen_logic();
}

bool display_menu_principal()
{
    return drawMenu("Menu Principal", "Acelarometro;Option2;Option3;Option4;Option5;Option6;Option7;Option8");
}

bool display_menu_acelarometro()
{
    return drawMenu("Acelarometro", "Actual;Alinhar Agora;Zerar calibracao");
}

bool drawMenu(String title, String options)
{
    // save energy by short circuiting
    if (menu_changed)
    {
        menu_changed = false;

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

        // menu selection wraps around
        if (menu_opt + 1 > opt_count)
            menu_opt = -1;
        if (menu_opt < -1)
            menu_opt = opt_count - 1;

        clear_and_reset();
        display_text(title, 1, false, 0, 0); // title

        int start = (menu_opt > 1) ? menu_opt - 1 : 0;
        for (byte idx = start; idx < opt_count + 1; idx++)
        {
            int y = FONT_SIZE + (idx - start) * FONT_SIZE;
            if (y > display.height())
                continue;

            display_text("                                           ", 1, false, 0, y);
            display_text(prefix_opt + opt_items[idx] + " ", 1, menu_opt == idx - 1, 0, y);
        }
        return true;
    }
    return false;
}

void handle_menu_action()
{
    switch (cur_menu)
    {
    case MENU_OPCOES_ROOT:
        change_menu(MENU_OPCOES_PRINCIPAL);
        break;

    case MENU_OPCOES_PRINCIPAL:
        switch (menu_opt)
        {
        case -1:
            change_menu(MENU_OPCOES_ROOT);
            break;
        case 0:
            change_menu(MENU_OPCOES_ACELAROMETRO);
            break;
        default:
            change_menu(MENU_OPCOES_ROOT);
            break;
        }
        break;

    case MENU_OPCOES_ACELAROMETRO:
        switch (menu_opt)
        {
        case -1:
            change_menu(MENU_OPCOES_PRINCIPAL);
            break;
        case 0:
            change_menu(MENU_OPCOES_ACELAROMETRO_ACTUAL);
            break;
        case 1:
            calibrate();
            change_menu(MENU_OPCOES_ROOT);
            break;
        case 2:
            reset_calibration();
            change_menu(MENU_OPCOES_ROOT);
            break;

        default:
            change_menu(MENU_OPCOES_ROOT);
            break;
        }
        break;
    case MENU_OPCOES_ACELAROMETRO_ACTUAL:
        change_menu(MENU_OPCOES_ACELAROMETRO);
        break;

    default:
        change_menu(MENU_OPCOES_ROOT);
        break;
    }
}

void handle_menu_screen_logic()
{
    switch (cur_menu)
    {
    case MENU_OPCOES_ROOT:
        set_draw_callback(display_accel);
        break;
    case MENU_OPCOES_PRINCIPAL:
        set_draw_callback(display_menu_principal);
        break;
    case MENU_OPCOES_ACELAROMETRO:
        set_draw_callback(display_menu_acelarometro);
        break;
    case MENU_OPCOES_ACELAROMETRO_ACTUAL:
        set_draw_callback(display_calibration);
        break;

    default:
        break;
    }
}

#endif
