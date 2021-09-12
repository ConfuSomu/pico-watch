#pragma once

#include "pico/util/datetime.h"

// Includes also buttons, API and ss_oled
#include "../../app/base_app.hpp"

#define NUMBER_OF_APPS 4
#define SIZE_APP_NAME 12

class app_home_menu : public BaseApp {
    private:
        const char *APPS_NAME[NUMBER_OF_APPS] = {"Home", "Clock", "Settings", "Tests"};
        int selected_app = 0;
        char display_app_name[SIZE_APP_NAME];

        void title_str(char *buf, uint buf_size, const datetime_t *t);
        void show_title(Api *app_api);

        AppAttributes app_attributes = {0, false};
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }
        
        app_home_menu(Api *app_api);
        AppReturnValues render(Api *app_api);
        AppReturnValues btn_pressed(Api *app_api, uint gpio, unsigned long delta);
        AppReturnValues bgrefresh(Api *app_api, bool in_foreground);
        ~app_home_menu();
};
