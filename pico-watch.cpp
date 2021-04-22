#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "init.hpp"
#include "api.hpp"
#include "buttons.hpp"
#include "apps/main_clock.hpp"
#include "apps/home_menu.hpp"

global_status g_s;
Api app_api;

#define NUMBER_OF_APPS 2
#define APP_DATA_BUFFER_LEN 256
int (*APPS_FUNC_INIT[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::init, app_main_clock::init};
int (*APPS_FUNC_RENDER[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::render, app_main_clock::render};
int (*APPS_FUNC_BTNPRESS[NUMBER_OF_APPS])(Api *app_api, uint gpio) = {app_home_menu::btnpressed, app_main_clock::btnpressed};
int (*APPS_FUNC_BGREFRESH[NUMBER_OF_APPS])(Api *app_api, bool in_foreground) = {app_home_menu::bgrefresh, app_main_clock::bgrefresh};
int (*APPS_FUNC_DESTROY[NUMBER_OF_APPS])(Api *app_api) = {app_home_menu::destroy, app_main_clock::destroy};
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {0, 1};
int APPS_IS_INIT[NUMBER_OF_APPS] = {0, 0}; // Only run in background if init

int app_init(int app_id) {
    app_api.display_fill(0,1); // Clear OLED
    app_api.performance_render_interval_set(500); // Reset interval

    if (app_id > NUMBER_OF_APPS-1 or app_id < 0) {
        printf("Tried to init app %d", app_id);
        return app_init(0);
    }

    if (!APPS_IS_INIT[app_id]) {
        int status = (*APPS_FUNC_INIT[app_id])(&app_api);

        switch (status) {
            case Api::app_init_return_status::MALLOC_FAILED:
                printf("App init failed (alloc), ");
                for (int i=0; i<10; i++) {
                    if ((*APPS_FUNC_INIT[app_id])(&app_api) != Api::app_init_return_status::MALLOC_FAILED) {
                        printf("worked after %d tries\n", i);
                        APPS_IS_INIT[app_id] = 1;
                        return app_id;
                    }
                }
                // Instead, the current app could continue running: return current_app
                printf("gave up, launching app 0\n");
                return app_init(0); // Note: this has to work (and should)
            
            default: // OK and unhandled status codes
                printf("App init, status: %d\n", status);
                break;
        }
        APPS_IS_INIT[app_id] = 1;
    }
    return app_id;
}

int app_render(int app_id) {
    return (*APPS_FUNC_RENDER[app_id])(&app_api);
}

int app_btnpressed(int app_id, uint gpio) {
    return (*APPS_FUNC_BTNPRESS[app_id])(&app_api, gpio);
}

int app_destroy(int app_id) {
    if (APPS_IS_INIT[app_id]) {
        APPS_IS_INIT[app_id] = 0;
        return (*APPS_FUNC_DESTROY[app_id])(&app_api);
    }
}

int app_bgrefresh(int app_id) {
    if (APPS_IS_INIT[app_id])
        return (*APPS_FUNC_BGREFRESH[app_id])(&app_api, app_id==g_s.current_app);
}

bool repeating_callback(struct repeating_timer *t) {
    // Enter shallow sleep mode when needed
    uint32_t time_since_last_press = to_ms_since_boot(get_absolute_time())-g_s.button_last_pressed_time;
    if (!g_s.is_sleeping && time_since_last_press > ENTER_SLEEP_DELAY) {
        g_s.is_sleeping = true;
        app_api.performance_set(Api::perf_modes::ENTER_SHALLOW_SLEEP);
        app_api.display_power(false);
    } else if (g_s.is_sleeping && time_since_last_press < ENTER_SLEEP_DELAY) {
        g_s.is_sleeping = false;
        app_api.performance_set(Api::perf_modes::EXIT_SHALLOW_SLEEP);
        app_api.display_power(true);
    }
    // Refresh each app, but should it be done when sleeping?
    for (int i=0; i < NUMBER_OF_APPS; i++) {
        app_bgrefresh(i);
    }
    return true;
}

void app_switch(int old_appid, int new_appid) {
    g_s.app_ready = false;
    // FIXME: race condition when pressing on HOME while app is rendering!
    // The system is blocked waiting for the app to finish rendering, which will never happen. To fix the problem, app switching has to be a flag (c.f struct) that is set, and checked before rendering app. "if (app_switching.requested) app_switch(...);" We will not need anymore the app_rendering flag, as the check is done while the app is not rendering.
    while (g_s.app_rendering); // Wait for the app to finish rendering cycle
    if (APPS_DESTROY_ON_EXIT[old_appid])
        app_destroy(old_appid);
    g_s.current_app = app_init(new_appid);
    g_s.app_ready = true;
}

int main() {
    init_all();
    printf("~~~==~~~");
    init_buttons();
    app_api.init();
    struct repeating_timer timer;
    add_repeating_timer_ms(250, repeating_callback, NULL, &timer); // TODO: Execute on core1

    app_init(g_s.current_app);

    while (1) {
        if (g_s.app_ready && !g_s.is_sleeping) {
            g_s.app_rendering = true;
            app_render(g_s.current_app);
            app_api.display_write_backbuffer();
            g_s.app_rendering = false;
        }
        if (g_s.is_sleeping) __wfi();
        else sleep_ms(app_api.performance_render_interval_get());
    }
    return 0;
}
