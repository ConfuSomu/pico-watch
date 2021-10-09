#pragma once

// To modify the I2C port used, change the `#define I2C_PORT` in "oled/BitBang_I2C.c"
// Pin numbers are GPIO pins.
#define SDA_PIN 6
#define SCL_PIN 7
#define RESET_PIN -1
#define OLED_DEFAULT_CONTRAST 40

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Initial date & time
// The idea is to have the compiler set the date at compile-time.
// DOTW: 0 is Sunday
#define INIT_DATETIME_YEAR 2021
#define INIT_DATETIME_MONTH 4
#define INIT_DATETIME_DAY 8
#define INIT_DATETIME_DOTW 4
#define INIT_DATETIME_HOUR 13
#define INIT_DATETIME_MIN 53
#define INIT_DATETIME_SEC 00

// Time (ms) before entering sleep
#define ENTER_SLEEP_DELAY 10000


// Init every componement
void init_all();
// Init onboard RTC
void init_rtc();
