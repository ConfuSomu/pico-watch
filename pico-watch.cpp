#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

#include "init.hpp"
#include "hal/api.hpp"
#include "buttons.hpp"
#include "globals.hpp"
#include "app/app_manager.hpp"

global_status g_s;
user_settings g_user;
Api app_api;

bool cb_status_check(struct repeating_timer *t) {
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

    // should it be done when sleeping?
    app_mgr::app_all_bgrefresh();
    
    return true;
}

int main() {
    init_all();
    printf("~~~==~~~");
    init_buttons();
    app_api.init();
    
    struct repeating_timer status_check_timer;
    add_repeating_timer_ms(250, cb_status_check, NULL, &status_check_timer); // TODO: Execute on core1

    g_s.foreground_app = app_mgr::app_init(0);

    while (1) {
        if (g_s.app_ready && !g_s.is_sleeping) {
            app_mgr::app_render(g_s.foreground_app);
            app_api.display_write_backbuffer();
        }
        
        if (g_s.is_sleeping)
            __wfi();
        else if (g_s.app_switch_requested) {
            app_mgr::app_switch(g_s.foreground_app, g_s.app_switch_to_app);
            g_s.app_switch_requested = false;
        } else
            sleep_ms(g_s.foreground_app->app_get_attributes().render_interval);
    }   
    return 0;
}
