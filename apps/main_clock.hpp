#ifndef __MAIN_CLOCK_H__
#define __MAIN_CLOCK_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "../api.hpp"

namespace app_main_clock {
    int init(Api *app_api);
    int render(Api *app_api);
    int btnpressed(Api *app_api, uint gpio);
    int bgrefresh(Api *app_api, bool in_foreground);
    int destroy(Api *app_api);
}

#endif