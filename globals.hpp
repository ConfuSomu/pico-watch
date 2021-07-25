#pragma once
#include "base_app.hpp"

struct global_status {
    BaseApp* current_app = 0;
    bool is_sleeping = false;
    bool app_ready = true;
    bool app_switch_requested = false;
    int app_switch_to_app = 0;

    // Debounce control
    // See https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=301522#p1812063
    // Time is currently shared between all buttons.
    unsigned long button_last_pressed_time;
    const int button_delay_time = 125;
};

struct user_settings {
    unsigned char oled_contrast = OLED_DEFAULT_CONTRAST;
    // In milliseconds
    unsigned int sleep_delay = ENTER_SLEEP_DELAY;
    // true: 24h, false: AM/PM
    // TODO: Use an enum, but this would make programming the UI more complex.
    bool time_format = true;
};

extern global_status g_s;
extern user_settings g_user;