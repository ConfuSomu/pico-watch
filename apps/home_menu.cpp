#include <stdio.h>
#include "pico/stdlib.h"

#include "home_menu.hpp"
#include "../api.hpp"
#include "../buttons.hpp"

extern void app_switch(int old_appid, int new_appid);
extern bool rtc_get_datetime(datetime_t *t);

#define NUMBER_OF_APPS 2

namespace app_home_menu {
    char *APPS_NAME[NUMBER_OF_APPS] = {"Home", "Clock"};
    char *pressed_button;
    int *selected_app;

    void title_str(char *buf, uint buf_size, const datetime_t *t) {
        snprintf(buf,
                buf_size,
                "%d:%02d Home Menu",
                t->hour,
                t->min,
                t->sec);
    };

    void show_title(Api *app_api) {
        char datetime_buf[256];
        char *datetime_str = &datetime_buf[0];
        datetime_t t;
        app_api->datetime_get(&t);

        // title with time
        title_str(datetime_str, sizeof(datetime_buf), &t);
        app_api->display_write_string(0,0,0, datetime_str, FONT_8x8, 0, 1);
    }

    // Rendering of app
    int render(Api *app_api) {
        show_title(app_api);
        app_api->display_write_string(0,0,2, pressed_button, FONT_6x8, 0, 1);
        app_api->display_write_string(0,5,3, APPS_NAME[*selected_app], FONT_12x16, 0, 1);
        sleep_ms(500);
        app_api->gui_popup_text("Title is a string", "Body text, and new line. And this is a pretty long string.\nIterating is very fun!");
        return 0;
    }

    // Example of how button inputs could be interpreted.
    // Drawing on screen should be done in the render function.
    int btnpressed(Api *app_api, uint gpio) {
        switch (gpio) {
            case BUTTON_HOME:
                *pressed_button = 'H'; break;
            case BUTTON_SELECT:
                *pressed_button = 'S';
                app_switch(0, *selected_app);
                break;
            case BUTTON_MODE:
                *pressed_button = 'M'; break;
            case BUTTON_DOWN:
                *pressed_button = 'D';
                *selected_app--;
                break;
            case BUTTON_UP:
                *pressed_button = 'U';
                *selected_app++;
                break;
            default:
                *pressed_button = '?';
        }
        if (*selected_app > NUMBER_OF_APPS-1) {
            *selected_app = NUMBER_OF_APPS-1; *pressed_button = '>';
        } else if (*selected_app < NUMBER_OF_APPS-1) {
            *selected_app = 0; *pressed_button = '<';
        }
        return 0;
    }

    // Initlisation of the app.
    int init(Api *app_api) {
        pressed_button = new char; *pressed_button = '*';
        selected_app = new int; *selected_app = 0; // Make sure to init the values to something known!
        return 0; // return 1 when function not implemented
    }

    // Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
    int bgrefresh(Api *app_api, char in_foreground) {
        return 1;
    }

    // Destruction of app, deinitlisation should be done here. This is only called if the app's APPS_DESTROY_ON_EXIT is set to 1. When it is not a "service" app.
    int destroy(Api *app_api) {
        delete pressed_button; pressed_button = nullptr;
        delete selected_app; selected_app = nullptr;
        return 1;
    }
}
