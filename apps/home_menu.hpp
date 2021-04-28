#ifndef __HOME_MENU_H__
#define __HOME_MENU_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

#include "../api.hpp"

namespace app_home_menu {
    int init(Api *app_api);
    int render(Api *app_api);
    int btnpressed(Api *app_api, uint gpio, unsigned long delta);
    int bgrefresh(Api *app_api, bool in_foreground);
    int destroy(Api *app_api);
}

#endif
