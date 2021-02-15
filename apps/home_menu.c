#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "home_menu.h"
#include "../buttons.h"

extern void app_switch(int old_appid, int new_appid);

#define NUMBER_OF_APPS 2

char* APPS_NAME[NUMBER_OF_APPS][12] = {"Home", "Clock"};

void title_str(char *buf, uint buf_size, const datetime_t *t) {
    snprintf(buf,
             buf_size,
             "%d:%02d Home Menu",
             t->hour,
             t->min,
             t->sec);
};

void show_title(SSOLED *oled) {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;
    rtc_get_datetime(&t);

    // title with time
    title_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(oled, 0,0,0, datetime_str, FONT_8x8, 0, 1);
}

// Rendering of app
int home_menu_render(SSOLED *oled, char *data, uint data_size) {
    show_title(oled);
    oledWriteString(oled, 0,0,2, &data[0], FONT_6x8, 0, 1);
    oledWriteString(oled, 0,5,3, APPS_NAME[data[1]][0], FONT_12x16, 0, 1); // FIXME: The name does not update, it seems that the second string is empty… but data[1]'s content is correct, as BUTTON_SELECT switches to the correct app.
    return 0;
}

// Interpret button inputs
int home_menu_btnpressed(SSOLED *oled, char *data, uint data_size, uint gpio) {
    switch (gpio) {
        case BUTTON_HOME:
            data[0] = 'H'; break;
        case BUTTON_SELECT:
            data[0] = 'S';
            app_switch(0, data[1]);
            break;
        case BUTTON_MODE:
            data[0] = 'M'; break;
        case BUTTON_DOWN:
            data[0] = 'D';
            data[1]--;
            break;
        case BUTTON_UP:
            data[0] = 'U';
            data[1]++;
            break;
        default:
            data[0] = '?';
    }
    if (data[1] > NUMBER_OF_APPS-1) {
        data[1] = NUMBER_OF_APPS-1; data[0] = '>';
    } else if (data[1] < NUMBER_OF_APPS-1) {
        data[1] = 0; data[0] = '<';
    }
    return 0;
}

// Initlisation of the app.
int home_menu_init(SSOLED *oled, char *data, uint data_size) {
    data[1] = 0;
    return 0; // return 1 when function not implemented
}

// Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
int home_menu_bgrefresh(SSOLED *oled, char *data, uint data_size, char in_foreground) {
    return 1;
}

// Destruction of app, deinitlisation should be done here. This is only called if the app's APPS_DESTROY_ON_EXIT is set to 1. When it is not a "service" app.
int home_menu_destroy(SSOLED *oled, char *data, uint data_size) {
    return 1;
}
