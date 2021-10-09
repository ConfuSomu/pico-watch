#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"
#include "../../hal/ui/widget/list_widget.hpp"
#define NUMBER_OF_SCREENS 2

BaseApp::AppReturnValues app_tests::render(Api *app_api) {
    app_api->gui_header_text("App for testing APIs.");

    switch (m_current_screen) {
    case 0:
        for (uint i = 0; i < 127*63; i++) {
        int x = rand() % 128;
        int y = rand() % 64;
        app_api->display_write_pixel(x, y, rand() % 2, 0);
        }
        break;
    
    case 1: {
        ListWidget widget(app_api, {10, 10, 100, 50});
        widget.paint();
        break;
        }
    
    default:
        break;
    }
    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_tests::btn_pressed(Api *app_api, uint gpio, unsigned long delta) {
    if (gpio == BUTTON_MODE and ++m_current_screen > (NUMBER_OF_SCREENS-1))
        m_current_screen = 0;
    
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
