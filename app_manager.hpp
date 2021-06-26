#pragma once
#include <vector>
#include "base_app.hpp"

extern std::vector<BaseApp*> open_apps;

int app_init(int app_id);
int app_render(int app_id);
int app_btnpressed(int app_id, uint gpio, unsigned long delta);
void app_switch(int old_appid, int new_appid);
void app_switch_request(int to_appid);
