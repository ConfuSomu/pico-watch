#ifndef __MAIN_CLOCK_H__
#define __MAIN_CLOCK_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

namespace app_main_clock {
    int init(SSOLED *oled, char *data, uint data_size);
    int render(SSOLED *oled, char *data, uint data_size);
    int btnpressed(SSOLED *oled, char *data, uint data_size, uint gpio);
    int bgrefresh(SSOLED *oled, char *data, uint data_size, char in_foreground);
    int destroy(SSOLED *oled, char *data, uint data_size);
}

#endif