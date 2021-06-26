#pragma once
#include "api.hpp"

// Base app
class BaseApp {
    public:
        uint app_id = 0; // CHECK: This has to be overwritten by derived classes
        virtual int render(Api *app_api) = 0; // Has to be implemented
        virtual int btnpressed(Api *app_api, uint gpio, unsigned long delta) {};
        virtual int bgrefresh(Api *app_api, bool in_foreground) {};
};
