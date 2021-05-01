#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"
#include "../../api.hpp"

extern void app_switch(int old_appid, int new_appid);
extern bool rtc_get_datetime(datetime_t *t);

#define MAIN_SET_NUM 4
#define MAIN_SET_NUM_STR "4"
#define SIZE_SETTING_NAME 12
#define SET0_NAME "Time"
#define SET1_NAME "Date"
#define SET2_NAME "Brightness"
#define SET3_NAME "Sleep delay"

namespace app_settings {
    const char *MAIN_SET_NAMES[MAIN_SET_NUM] = {SET0_NAME, SET1_NAME, SET2_NAME, SET3_NAME};
    int selected_setting = 0;
    char display_setting_name[SIZE_SETTING_NAME];
    bool selected = false;

    void show_title(Api *app_api) {
        std::string title_str {"Settings (/" MAIN_SET_NUM_STR ")"};
        title_str.insert(10, std::to_string(selected_setting+1));
        app_api->gui_header_text(title_str);
    }

    // Rendering of app
    int render(Api *app_api) {
        show_title(app_api);
        app_api->display_write_string(0,0,3, display_setting_name, FONT_12x16, 0, 1);

        if (selected) {
            selected = false;
            switch (selected_setting) {
                case 0:
                    app_api->gui_popup_text(SET0_NAME, "Todo!");
                    break;
                case 1:
                    app_api->gui_popup_text(SET1_NAME, "Todo!");
                    break;
            }
        }

        return 0;
    }

    // Example of how button inputs could be interpreted.
    // Drawing on screen should be done in the render function.
    int btnpressed(Api *app_api, uint gpio, unsigned long delta) {
        switch (gpio) {
            case BUTTON_SELECT:
                selected = true;
                break;
            case BUTTON_DOWN:
                selected_setting--;
                break;
            case BUTTON_UP:
                selected_setting++;
                break;
        }
        if (selected_setting > MAIN_SET_NUM-1) {
            selected_setting = MAIN_SET_NUM-1;
        } else if (selected_setting < 0) {
            selected_setting = 0;
        }
        // Add spaces to avoid "ghost" characters from app names displayed before
        snprintf(display_setting_name, SIZE_SETTING_NAME, "%s             ", MAIN_SET_NAMES[selected_setting]);
        return 0;
    }

    // Initlisation of the app.
    int init(Api *app_api) {
        app_api->performance_set(Api::perf_modes::LOW_POWER);
        app_api->performance_render_interval_set(100);
        selected_setting = 0;
        selected = false;
        snprintf(display_setting_name, SIZE_SETTING_NAME, "%s", MAIN_SET_NAMES[0]);
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
