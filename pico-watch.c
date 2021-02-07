#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

// To modify the I2C port used, change the `#define I2C_PORT` in "oled/BitBang_I2C.c"
// Pin numbers are GPIO pins.
#define SDA_PIN 6
#define SCL_PIN 7
#define RESET_PIN -1
SSOLED oled;
// Unused but useful for reference:
#define OLED_WIDTH 128
#define OLED_HEIGHT 64


void init_display() {
    oledInit(&oled, OLED_128x64, 0x3d, 0, 0, 1, SDA_PIN, SCL_PIN, RESET_PIN, 1000000L);
    oledFill(&oled, 0,1);
    oledSetContrast(&oled, 127);
    oledSetTextWrap(&oled, true);
}

void init_rtc() {
    datetime_t init_date = {
        .year = 2020,
        .month = 06,
        .day = 05,
        .dotw = 5, // 0 is Sunday, so 5 is Friday
        .hour = 15,
        .min = 45,
        .sec = 00 };
    rtc_init();
    rtc_set_datetime(&init_date);
}

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

void show_datetime() {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;

    rtc_get_datetime(&t);
    time_as_str(datetime_str, sizeof(datetime_buf), &t);
    oledWriteString(&oled, 0,10,3, datetime_str, FONT_12x16, 0, 1);
}

int main()
{
    stdio_init_all();
    init_display();
    init_rtc();
    oledWriteString(&oled, 0,15,0, (char *)"Test clock", FONT_8x8, 0, 1);
    while (1) {
        show_datetime();
        sleep_ms(500);
    }
    return 0;
}
