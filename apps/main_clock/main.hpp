#pragma once

#include "pico/util/datetime.h"

// Includes also buttons, API and ss_oled
#include "../../base_app.hpp"

class app_main_clock : public BaseApp {
    private:
        bool *ask_user_choice;
        int *user_choice;
        const char *choices[26] = {"One", "Two", "Three!", "This is looong!", "make sure to choose me!:p"};

        void time_as_str(char *buf, uint buf_size, const datetime_t *t);
        void date_as_str(char *buf, uint buf_size, const datetime_t *t);
        void show_datetime(Api *app_api);

        AppAttributes app_attributes = {1, true};
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }

        app_main_clock(Api *app_api);
        int render(Api *app_api);
        int btnpressed(Api *app_api, uint gpio, unsigned long delta);
        int bgrefresh(Api *app_api, bool in_foreground);
        ~app_main_clock();
};