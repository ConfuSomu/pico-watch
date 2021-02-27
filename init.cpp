#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
extern "C" {
#include "hardware/rtc.h"
}

#include "init.hpp"

void init_rtc() {
    datetime_t init_date = {
        .year = INIT_DATETIME_YEAR,
        .month = INIT_DATETIME_MONTH,
        .day = INIT_DATETIME_DAY,
        .dotw = INIT_DATETIME_DOTW, // 0 is Sunday, so 5 is Friday
        .hour = INIT_DATETIME_HOUR,
        .min = INIT_DATETIME_MIN,
        .sec = INIT_DATETIME_SEC };
    rtc_init();
    rtc_set_datetime(&init_date);
}

void init_all() {
    stdio_init_all();
    init_rtc();
}
