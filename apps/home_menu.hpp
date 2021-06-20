#pragma once

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "../api.hpp"
#include "../base_app.hpp"

#define NUMBER_OF_APPS 3
#define SIZE_APP_NAME 12

class app_home_menu : public BaseApp {
    private:
        const char *APPS_NAME[NUMBER_OF_APPS] = {"Home", "Clock", "Settings"};
        int selected_app = 0;
        char display_app_name[SIZE_APP_NAME];
        void title_str(char *buf, uint buf_size, const datetime_t *t);
        void show_title(Api *app_api);
    public:
        uint app_id = 0;
        app_home_menu(Api *app_api);
        int render(Api *app_api);
        int btnpressed(Api *app_api, uint gpio, unsigned long delta);
        int bgrefresh(Api *app_api, bool in_foreground);
        ~app_home_menu();
};
