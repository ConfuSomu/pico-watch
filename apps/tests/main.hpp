#pragma once

// Includes also buttons, API and ss_oled
#include "../../app/base_app.hpp"

class app_tests : public BaseApp {
    private:
        AppAttributes app_attributes = {3, true};
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }

        app_tests(Api *app_api);
        AppReturnValues render(Api *app_api);
        AppReturnValues btn_pressed(Api *app_api, uint gpio, unsigned long delta);
        AppReturnValues bgrefresh(Api *app_api, bool in_foreground);
        ~app_tests();
};
