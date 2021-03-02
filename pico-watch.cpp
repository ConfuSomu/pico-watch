#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "init.hpp"
#include "api.hpp"
#include "buttons.hpp"
#include "apps/main_clock.hpp"
#include "apps/home_menu.hpp"

int current_app = 0;
Api app_api;

#define NUMBER_OF_APPS 2
#define APP_DATA_BUFFER_LEN 256
int (*APPS_FUNC_INIT[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::init, app_main_clock::init};
int (*APPS_FUNC_RENDER[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::render, app_main_clock::render};
int (*APPS_FUNC_BTNPRESS[NUMBER_OF_APPS])(Api *app_api, uint gpio) = {app_home_menu::btnpressed, app_main_clock::btnpressed};
int (*APPS_FUNC_BGREFRESH[NUMBER_OF_APPS])(Api *app_api, char in_foreground) = {app_home_menu::bgrefresh, app_main_clock::bgrefresh};
int (*APPS_FUNC_DESTROY[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::destroy, app_main_clock::destroy};
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {0, 1};
int APPS_IS_INIT[NUMBER_OF_APPS] = {0, 0}; // Only run in background if init

int app_init(int app_id) {
    app_api.display_fill(0,1); // Clear OLED
    if (!APPS_IS_INIT[app_id]) {
        APPS_IS_INIT[app_id] = 1;
        return (*APPS_FUNC_INIT[app_id])(&app_api);
    }
}

int app_render(int app_id) {
    return (*APPS_FUNC_RENDER[app_id])(&app_api);
}

int app_btnpressed(int app_id, uint gpio) {
    if (app_api.m_send_button_press_to_app)
        return (*APPS_FUNC_BTNPRESS[app_id])(&app_api, gpio);
    return 2;
}

int app_destroy(int app_id) {
    // TODO: reset APPS_DATA for the app
    if (APPS_IS_INIT[app_id]) {
        APPS_IS_INIT[app_id] = 0;
        return (*APPS_FUNC_DESTROY[app_id])(&app_api);
    }
}

int app_bgrefresh(int app_id) {
    if (APPS_IS_INIT[app_id])
        return (*APPS_FUNC_BGREFRESH[app_id])(&app_api, app_id==current_app);
}

bool apps_bgrefresh(struct repeating_timer *t) {  // TODO: Refresh done on core1
    for (int i=0; i < NUMBER_OF_APPS; i++) {
        app_bgrefresh(i);
    }
    return true;
}

void app_switch(int old_appid, int new_appid) {
    if (APPS_DESTROY_ON_EXIT[old_appid]) {
        app_destroy(old_appid);
    }
    app_init(new_appid);
    current_app = new_appid;
}

int main() {
    init_all();
    init_buttons();
    app_api.init();
    struct repeating_timer timer;
    add_repeating_timer_ms(250, apps_bgrefresh, NULL, &timer);

    app_init(current_app);

    while (1) {
        app_render(current_app);
        sleep_ms(app_api.performance_render_interval_get());
    }
    return 0;
}
