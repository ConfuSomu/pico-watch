#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"

BaseApp::AppReturnValues app_tests::render(Api *app_api) {
    app_api->gui_header_text("App for testing APIs.");
    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_tests::btn_pressed(Api *app_api, uint gpio, unsigned long delta) {
    return AppReturnValues::OK;
}

app_tests::app_tests(Api *app_api) {
    app_api->performance_set(Api::perf_modes::LOW_POWER);
}

BaseApp::AppReturnValues app_tests::bgrefresh(Api *app_api, bool in_foreground) {
    return AppReturnValues::OK;
}

app_tests::~app_tests() {
}
