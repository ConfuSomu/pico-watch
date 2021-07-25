#pragma once
#include <vector>
#include "base_app.hpp"

extern std::vector<BaseApp*> open_apps;

BaseApp* app_init(int app_id);
int app_render(BaseApp* app);
int app_btnpressed(BaseApp* app, uint gpio, unsigned long delta);
void app_switch(BaseApp* app, int new_appid);
void app_switch_request(int to_appid);
