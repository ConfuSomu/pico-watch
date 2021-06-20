#ifndef __BUTTONS_H__
#define __BUTTONS_H__
#include "init.hpp"

// Init buttons used in conjuction with interrupts
// All buttons are connected to ground.
#define NUMBER_OF_BUTTONS 5
#define BUTTON_NONE 0
#define BUTTON_HOME 22
#define BUTTON_SELECT 18
#define BUTTON_MODE 19
#define BUTTON_DOWN 20
#define BUTTON_UP 21

struct global_status {
    int current_app = 0;
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

void init_buttons();
void gpio_interrupt_cb(uint gpio, uint32_t events);
unsigned long time_since_button_press();

#endif