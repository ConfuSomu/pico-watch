#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

#include "init.h"


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

void show_datetime() {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;
    rtc_get_datetime(&t);

    // time
    time_as_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(&oled, 0,10,3, datetime_str, FONT_12x16, 0, 1);

    // date
    date_as_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(&oled, 0,0,7, datetime_str, FONT_8x8, 0, 1);
}

int main() {
    init_all();
    oledWriteString(&oled, 0,15,0, (char *)"Test clock", FONT_8x8, 0, 1);
    while (1) {
        show_datetime();
        sleep_ms(500);
    }
    return 0;
}
