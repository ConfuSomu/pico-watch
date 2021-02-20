#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

#include "init.hpp"
#include "buttons.hpp"
#include "apps/main_clock.hpp"
#include "apps/home_menu.hpp"

int current_app = 0;

#define NUMBER_OF_APPS 2
#define APP_DATA_BUFFER_LEN 256
int (*APPS_FUNC_INIT[NUMBER_OF_APPS])(SSOLED *oled) = {app_home_menu::init, app_main_clock::init};
int (*APPS_FUNC_RENDER[NUMBER_OF_APPS])(SSOLED *oled) = {app_home_menu::render, app_main_clock::render};
int (*APPS_FUNC_BTNPRESS[NUMBER_OF_APPS])(SSOLED *oled, uint gpio) = {app_home_menu::btnpressed, app_main_clock::btnpressed};
int (*APPS_FUNC_BGREFRESH[NUMBER_OF_APPS])(SSOLED *oled, char in_foreground) = {app_home_menu::bgrefresh, app_main_clock::bgrefresh};
int (*APPS_FUNC_DESTROY[NUMBER_OF_APPS])(SSOLED *oled) = {app_home_menu::destroy, app_main_clock::destroy};
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {0, 1};
int APPS_IS_INIT[NUMBER_OF_APPS] = {0, 0}; // Only run in background if init

int app_init(int app_id) {
    oledFill(&oled, 0,1); // Clear OLED
    if (!APPS_IS_INIT[app_id]) {
        APPS_IS_INIT[app_id] = 1;
        return (*APPS_FUNC_INIT[app_id])(&oled);
    }
}

int app_render(int app_id) {
    return (*APPS_FUNC_RENDER[app_id])(&oled);
}

int app_btnpressed(int app_id, uint gpio) {
    return (*APPS_FUNC_BTNPRESS[app_id])(&oled, gpio);
}

int app_destroy(int app_id) {
    // TODO: reset APPS_DATA for the app
    if (APPS_IS_INIT[app_id]) {
        APPS_IS_INIT[app_id] = 0;
        return (*APPS_FUNC_DESTROY[app_id])(&oled);
    }
}

int app_bgrefresh(int app_id) {
    if (APPS_IS_INIT[app_id])
        return (*APPS_FUNC_BGREFRESH[app_id])(&oled, app_id==current_app);
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
    struct repeating_timer timer;
    add_repeating_timer_ms(250, apps_bgrefresh, NULL, &timer);

    app_init(current_app);

    while (1) {
        app_render(current_app);
        sleep_ms(500);
    }
    return 0;
}
