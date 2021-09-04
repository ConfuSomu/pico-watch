#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"

// TODO in this app
// - 

// Time as string
// Adapted from pico-sdk/scr/common/pico_util/datetime.c
void app_main_clock::time_as_str(char *buf, uint buf_size, const datetime_t *t) {
    snprintf(buf,
            buf_size,
            "%d:%02d:%02d ",
            t->hour,
            t->min,
            t->sec);
};

void app_main_clock::date_as_str(char *buf, uint buf_size, const datetime_t *t) {
    static const char *DATETIME_DOWS[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    snprintf(buf,
            buf_size,
            "%02d-%02d-%02d %s",
            t->year,
            t->month,
            t->day,
            DATETIME_DOWS[t->dotw]);
};

void app_main_clock::show_datetime(Api *app_api) {
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t t;
    app_api->datetime_get(&t);

    // time
    time_as_str(datetime_str, sizeof(datetime_buf), &t);
    app_api->display_write_string(0,10,3, datetime_str, FONT_12x16, 0, 1);

    // date
    date_as_str(datetime_str, sizeof(datetime_buf), &t);
    app_api->gui_footer_text((std::string)datetime_str);
}

BaseApp::AppReturnValues app_main_clock::render(Api *app_api) {
    app_api->gui_header_text("Test clock", 17);
    show_datetime(app_api);

    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_main_clock::btn_pressed(Api *app_api, uint gpio, unsigned long delta) {
    return AppReturnValues::OK;
}

app_main_clock::app_main_clock(Api *app_api) {
    app_api->performance_set(Api::perf_modes::LOW_POWER);
}

BaseApp::AppReturnValues app_main_clock::bgrefresh(Api *app_api, bool in_foreground) {
    return AppReturnValues::OK;
}

app_main_clock::~app_main_clock() {
}
