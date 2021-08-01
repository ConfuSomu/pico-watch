#pragma once
#include "api.hpp"

// Base app
class BaseApp {
    public:
        struct AppAttributes {
            uint id = 0;
            bool destroy_on_exit = true;
        };
        // CHECK: Following have to be overwritten by derived classes
        virtual const AppAttributes& app_get_attributes() = 0;
        // Could be implemented as:
        // {return app_attributes;}
        // where app_attribues is an instance of AppAttributes

        virtual int render(Api *app_api) = 0; // Has to be implemented
        virtual int btnpressed(Api *app_api, uint gpio, unsigned long delta) {};
        virtual int bgrefresh(Api *app_api, bool in_foreground) {};
};
