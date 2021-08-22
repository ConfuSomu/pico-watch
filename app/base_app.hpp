#pragma once
#include "../hal/api.hpp"

// Base app
class BaseApp {
    public:
        struct AppAttributes {
            uint id = 0;
            bool destroy_on_exit = true;
            uint render_interval = 500; // Interval in ms at which the render method is called.
        };
        enum AppReturnValues {
            OK = 0,
            CLOSE, // Close app from foreground, not respecting AppAttribues::destroy_on_exit.
            QUIT // Completely quit app; stop running in background
        };

        // CHECK: Following have to be overwritten by derived classes
        virtual const AppAttributes& app_get_attributes() = 0;
        // Could be implemented as:
        // {return app_attributes;}
        // where app_attribues is an instance of AppAttributes

        virtual AppReturnValues render(Api *app_api) = 0; // Has to be implemented
        // Called when a button is released.
        // \param delta The time since the button has been last released. Delta is in ms, from time_since_button_press().
        virtual AppReturnValues btnpressed(Api *app_api, uint gpio, unsigned long delta) {return AppReturnValues::OK;};
        // Called when a button is released.
        // \param delta The time since the button has been pressed. Delta is in ms, from time_since_button_press().
        virtual AppReturnValues btnreleased(Api *app_api, uint gpio, unsigned long delta) {return AppReturnValues::OK;};
        virtual AppReturnValues bgrefresh(Api *app_api, bool in_foreground) {return AppReturnValues::OK;};
};
