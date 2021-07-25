#pragma once
#include "api.hpp"

// Base app
class BaseApp {
    public:
        // CHECK: Following have to be overwritten by derived classes
        // TODO: Replace this with a call to a function returning a struct with all app attributes.
        uint app_id = 0;
        bool app_destroy_on_exit = true;

        virtual int render(Api *app_api) = 0; // Has to be implemented
        virtual int btnpressed(Api *app_api, uint gpio, unsigned long delta) {};
        virtual int bgrefresh(Api *app_api, bool in_foreground) {};
};
