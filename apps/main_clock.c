#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "main_clock.h"

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

int main_clock_render(SSOLED *oled, char *data, uint data_size) {
    show_datetime(oled);
//    data[1] += 1; // testing
//    oledWriteString(oled, 0,0,2, &data[0], FONT_6x8, 0, 1);
    return 0;
}

int main_clock_init(SSOLED *oled, char *data, uint data_size) {
//    data[0] = 'a'; // testing, to showcase the use of data
//    data[1] = 'b';
    return 1; // return 1 when function not implemented
}

int main_clock_bgrefresh(SSOLED *oled, char *data, uint data_size) {
    return 1;
}

int main_clock_destroy(SSOLED *oled, char *data, uint data_size) {
    return 1;
}
