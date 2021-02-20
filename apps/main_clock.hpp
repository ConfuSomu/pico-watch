#ifndef __MAIN_CLOCK_H__
#define __MAIN_CLOCK_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

namespace app_main_clock {
    int init(SSOLED *oled);
    int render(SSOLED *oled);
    int btnpressed(SSOLED *oled, uint gpio);
    int bgrefresh(SSOLED *oled, char in_foreground);
    int destroy(SSOLED *oled);
}

#endif