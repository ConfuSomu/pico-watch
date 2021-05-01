#ifndef __SETTINGS_H__
#define __SETTINGS_H__
#include "../../api.hpp"

namespace app_settings {
    int init(Api *app_api);
    int render(Api *app_api);
    int btnpressed(Api *app_api, uint gpio, unsigned long delta);
    int bgrefresh(Api *app_api, bool in_foreground);
    int destroy(Api *app_api);
}

#endif
