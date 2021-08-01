#pragma once
#include "../../base_app.hpp"
#include "strings.hpp"

class app_settings : public BaseApp {
    private:
        const char *MAIN_SET_NAMES[MAIN_SET_NUM] = {SET0_NAME, SET1_NAME};
        int selected_setting = 0;
        char display_setting_name[SIZE_SETTING_NAME];
        bool selected = false;

        void show_title(Api *app_api);
        void set0_menu(Api *app_api);
        void set1_menu(Api *app_api);

        AppAttributes app_attributes = {2, true};
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }

        app_settings(Api *app_api);
        int render(Api *app_api);
        int btnpressed(Api *app_api, uint gpio, unsigned long delta);
        int bgrefresh(Api *app_api, bool in_foreground);
        ~app_settings();
};
#include "strings-undef.hpp"
