#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

#include "init.h"
#include "apps/main_clock.h"

int current_app = 0;

#define NUMBER_OF_APPS 1
#define APP_DATA_BUFFER_LEN 256
int (*APPS_FUNC_INIT[NUMBER_OF_APPS])(SSOLED *oled, char *data, uint data_size) = {main_clock_init};
int (*APPS_FUNC_RENDER[NUMBER_OF_APPS])(SSOLED *oled, char *data, uint data_size) = {main_clock_render};
int (*APPS_FUNC_BGREFRESH[NUMBER_OF_APPS])(SSOLED *oled, char *data, uint data_size) = {main_clock_bgrefresh};
int (*APPS_FUNC_DESTROY[NUMBER_OF_APPS])(SSOLED *oled, char *data, uint data_size) = {main_clock_destroy};
char APPS_DATA[NUMBER_OF_APPS][APP_DATA_BUFFER_LEN];
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {1};
int APPS_IS_INIT[NUMBER_OF_APPS] = {0};

int app_init(int app_id) {
    APPS_IS_INIT[app_id] = 1;
    return (*APPS_FUNC_INIT[app_id])(&oled, &APPS_DATA[app_id][0], sizeof(APPS_DATA[app_id]));
}

int app_render(int app_id) {
    return (*APPS_FUNC_RENDER[app_id])(&oled, &APPS_DATA[app_id][0], sizeof(APPS_DATA[app_id]));
}

int app_destroy(int app_id) {
    if (APPS_IS_INIT[app_id])
        return (*APPS_FUNC_INIT[app_id])(&oled, &APPS_DATA[app_id][0], sizeof(APPS_DATA[app_id]));
}

int main() {
    init_all();

    app_init(current_app);

    oledWriteString(&oled, 0,15,0, (char *)"Test clock", FONT_8x8, 0, 1);
    while (1) {
        app_render(current_app);
        sleep_ms(500);
    }
    return 0;
}
