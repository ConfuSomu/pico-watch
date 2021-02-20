#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
extern "C" {
#include "hardware/rtc.h"
}
#include "oled/ss_oled.h"

#include "init.hpp"
SSOLED oled;

void init_display() {
    oledInit(&oled, OLED_128x64, 0x3d, 0, 0, 1, SDA_PIN, SCL_PIN, RESET_PIN, 1000000L);
    oledFill(&oled, 0,1);
    oledSetContrast(&oled, OLED_DEFAULT_CONTRAST);
    //oledSetTextWrap(&oled, true);
}

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
    init_display();
    init_rtc();
}
