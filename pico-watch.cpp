#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "init.hpp"
#include "api.hpp"
#include "buttons.hpp"
#include "globals.hpp"
#include "app_manager.hpp"

global_status g_s;
user_settings g_user;
Api app_api;

bool repeating_callback(struct repeating_timer *t) {
    // Enter shallow sleep mode when needed
    auto time_since_last_press = time_since_button_press();
    if (!g_s.is_sleeping && time_since_last_press > g_user.sleep_delay) {
        g_s.is_sleeping = true;
        app_api.performance_set(Api::perf_modes::ENTER_SHALLOW_SLEEP);
        app_api.display_power(false);
    } else if (g_s.is_sleeping && time_since_last_press < g_user.sleep_delay) {
        g_s.is_sleeping = false;
        app_api.performance_set(Api::perf_modes::EXIT_SHALLOW_SLEEP);
        app_api.display_power(true);
    }

    // Refresh each app
    // should it be done when sleeping?
    for (auto app : open_apps) {
        app->bgrefresh(&app_api, true); // FIXME: second arg
    }
    return true;
}

int main() {
    init_all();
    printf("~~~==~~~");
    init_buttons();
    app_api.init();
    struct repeating_timer timer;
    add_repeating_timer_ms(250, repeating_callback, NULL, &timer); // TODO: Execute on core1

    g_s.current_app = app_init(0);

    while (1) {
        if (g_s.app_switch_requested) {
            app_switch(g_s.current_app, g_s.app_switch_to_app);
            g_s.app_switch_requested = false;
        }
        
        if (g_s.app_ready && !g_s.is_sleeping) {
            app_render(g_s.current_app);
            app_api.display_write_backbuffer();
        }
        
        if (g_s.is_sleeping) __wfi();
        else sleep_ms(app_api.performance_render_interval_get());
    }
    return 0;
}
