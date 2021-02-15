#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "main_clock.h"
#include "../buttons.h"

// Time as string
// Adapted from pico-sdk/scr/common/pico_util/datetime.c
void time_as_str(char *buf, uint buf_size, const datetime_t *t) {
    snprintf(buf,
             buf_size,
             "%d:%02d:%02d",
             t->hour,
             t->min,
             t->sec);
};
void date_as_str(char *buf, uint buf_size, const datetime_t *t) {
    static const char *DATETIME_DOWS[7] = {
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat",
    };
    snprintf(buf,
             buf_size,
             "%02d-%02d-%02d %s",
             t->year,
             t->month,
             t->day,
             DATETIME_DOWS[t->dotw - 1]);
};

void show_datetime(SSOLED *oled) {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;
    rtc_get_datetime(&t);

    // time
    time_as_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(oled, 0,10,3, datetime_str, FONT_12x16, 0, 1);

    // date
    date_as_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(oled, 0,0,7, datetime_str, FONT_8x8, 0, 1);
}

// Rendering of the app
int main_clock_render(SSOLED *oled, char *data, uint data_size) {
    oledWriteString(oled, 0,15,0, (char *)"Test clock", FONT_8x8, 0, 1);
    show_datetime(oled);
    oledWriteString(oled, 0,0,0, &data[0], FONT_6x8, 0, 1);
    return 0;
}

// Example of how button inputs could be interpreted.
// Drawing on screen should be done in the render function.
int main_clock_btnpressed(SSOLED *oled, char *data, uint data_size, uint gpio) {
    switch (gpio) {
        case BUTTON_HOME: // Apps will generally not receive the Home button press
            data[0] = 'H'; break;
        case BUTTON_SELECT:
            data[0] = 'S'; break;
        case BUTTON_MODE:
            data[0] = 'M'; break;
        case BUTTON_DOWN:
            data[0] = 'D'; break;
        case BUTTON_UP:
            data[0] = 'U'; break;
        default:
            data[0] = '?';
    }
    return 0;
}

// Initlisation of the app.
int main_clock_init(SSOLED *oled, char *data, uint data_size) {
    return 1; // return 1 when function not implemented
}

// Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
int main_clock_bgrefresh(SSOLED *oled, char *data, uint data_size, char in_foreground) {
    return 1;
}

// Destruction of app, deinitlisation should be done here. This is only called if the app's APPS_DESTROY_ON_EXIT is set to 1. When it is not a "service" app.
int main_clock_destroy(SSOLED *oled, char *data, uint data_size) {
    return 1;
}
