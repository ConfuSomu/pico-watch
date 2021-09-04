#pragma once

#include "pico/util/datetime.h"

// Includes also buttons, API and ss_oled
#include "../../app/base_app.hpp"

class app_main_clock : public BaseApp {
    private:
        bool *ask_user_choice;
        int *user_choice;
        const char *choices[10] = {"One", "Close (fg)", "Quit (fg)", "Close (bg)", "Quit (bg)"};

        void time_as_str(char *buf, uint buf_size, const datetime_t *t);
        void date_as_str(char *buf, uint buf_size, const datetime_t *t);
        void show_datetime(Api *app_api);

        AppAttributes app_attributes = {1, true};
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }

        app_main_clock(Api *app_api);
        AppReturnValues render(Api *app_api);
        AppReturnValues btn_pressed(Api *app_api, uint gpio, unsigned long delta);
        AppReturnValues bgrefresh(Api *app_api, bool in_foreground);
        ~app_main_clock();
};
