#include <stdio.h>
#include "pico/stdlib.h"

#include "home_menu.hpp"
#include "../api.hpp"
#include "../buttons.hpp"

extern void app_switch(int old_appid, int new_appid);
extern bool rtc_get_datetime(datetime_t *t);

#define NUMBER_OF_APPS 2
#define SIZE_APP_NAME 12

namespace app_home_menu {
    const char *APPS_NAME[NUMBER_OF_APPS] = {"Home", "Clock"};
    int selected_app = 0;
    char display_app_name[SIZE_APP_NAME];

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
        app_api->gui_header_text((std::string)datetime_str);
    }

    // Rendering of app
    int render(Api *app_api) {
        show_title(app_api);
        app_api->display_write_string(0,5,3, display_app_name, FONT_12x16, 0, 1);
        return 0;
    }

    // Example of how button inputs could be interpreted.
    // Drawing on screen should be done in the render function.
    int btnpressed(Api *app_api, uint gpio) {
        switch (gpio) {
            case BUTTON_SELECT:
                app_switch(0, selected_app);
                return 0;
            case BUTTON_DOWN:
                selected_app--;
                break;
            case BUTTON_UP:
                selected_app++;
                break;
        }
        if (selected_app > NUMBER_OF_APPS-1) {
            selected_app = NUMBER_OF_APPS-1;
        } else if (selected_app < 0) {
            selected_app = 0;
        }
        // Add spaces to avoid "ghost" characters from app names displayed before
        snprintf(display_app_name, SIZE_APP_NAME, "%s             ", APPS_NAME[selected_app]);
        return 0;
    }

    // Initlisation of the app.
    int init(Api *app_api) {
        app_api->performance_set(Api::perf_modes::LOW_POWER);
        selected_app = 0;
        snprintf(display_app_name, SIZE_APP_NAME, "%s", APPS_NAME[0]);
        return Api::app_init_return_status::OK;
    }

    // Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
    int bgrefresh(Api *app_api, bool in_foreground) {
        return 1;
    }

    // Destruction of app, deinitlisation should be done here. This is only called if the app's APPS_DESTROY_ON_EXIT is set to 1. When it is not a "service" app.
    int destroy(Api *app_api) {
        return 0;
    }
}
