#pragma once
#include <vector>
#include "base_app.hpp"

// List of pointers to currently running apps.
extern std::vector<BaseApp*> open_apps;

// Init a new app, that is not running.
BaseApp* app_init(int app_id);

// Allow the running app, referenced by app_id, to invoke its render routine.
int app_render(BaseApp* app);

// Delta is in ms, from time_since_button_press()
int app_btnpressed(BaseApp* app, uint gpio, unsigned long delta);

// This should only be called by pico-watch.cpp before app rendering, to chage the current app.
void app_switch(BaseApp* app, int new_appid);

// Requests the current app to be replaced by an other one. The replacement will be done at the right moment.
void app_switch_request(int to_appid);

// Refresh each app
void app_all_bgrefresh();
