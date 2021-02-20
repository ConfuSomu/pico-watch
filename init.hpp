#ifndef __INIT_H__
#define __INIT_H__

// To modify the I2C port used, change the `#define I2C_PORT` in "oled/BitBang_I2C.c"
// Pin numbers are GPIO pins.
#define SDA_PIN 6
#define SCL_PIN 7
#define RESET_PIN -1
#define OLED_DEFAULT_CONTRAST 40
extern SSOLED oled;

// Initial date & time
// The idea is to have the compiler set the date at compile-time.
// DOTW: 0 is Sunday
#define INIT_DATETIME_YEAR 2020
#define INIT_DATETIME_MONTH 06
#define INIT_DATETIME_DAY 05
#define INIT_DATETIME_DOTW 5
#define INIT_DATETIME_HOUR 15
#define INIT_DATETIME_MIN 45
#define INIT_DATETIME_SEC 00


// Init every componement
void init_all();
// Init OLED display
void init_display();
// Init onboard RTC
void init_rtc();

#endif