#include <stdio.h>
#include "pico/stdlib.h"

#include "../../globals.hpp"
#include "../../app_manager.hpp"
#include "main.hpp"

extern bool rtc_get_datetime(datetime_t *t);

void app_home_menu::title_str(char *buf, uint buf_size, const datetime_t *t) {
    snprintf(buf,
            buf_size,
            "%d:%02d Home Menu",
            t->hour,
            t->min,
            t->sec);
};

void app_home_menu::show_title(Api *app_api) {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;
    app_api->datetime_get(&t);

    // title with time
    title_str(datetime_str, sizeof(datetime_buf), &t);
    app_api->gui_header_text((std::string)datetime_str);
}

// Rendering of app
BaseApp::AppReturnValues app_home_menu::render(Api *app_api) {
    show_title(app_api);
    app_api->display_write_string(0,5,3, display_app_name, FONT_12x16, 0, 1);
    return AppReturnValues::OK;
}

// Example of how button inputs could be interpreted.
// Drawing on screen should be done in the render function.
BaseApp::AppReturnValues app_home_menu::btnpressed(Api *app_api, uint gpio, unsigned long delta) {
    switch (gpio) {
        case BUTTON_SELECT:
            app_mgr::app_switch_request(selected_app);
            return AppReturnValues::OK;
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
    return AppReturnValues::OK;
}

// Initlisation of the app.
app_home_menu::app_home_menu(Api *app_api) {
    app_api->performance_set(Api::perf_modes::LOW_POWER);
    selected_app = 0;
    snprintf(display_app_name, SIZE_APP_NAME, "%s", APPS_NAME[0]);
}

// Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
BaseApp::AppReturnValues app_home_menu::bgrefresh(Api *app_api, bool in_foreground) {
    return AppReturnValues::OK;
}
