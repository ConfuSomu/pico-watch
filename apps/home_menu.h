#ifndef __HOME_MENU_H__
#define __HOME_MENU_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

void show_title(SSOLED *oled);

int home_menu_init(SSOLED *oled, char *data, uint data_size);
int home_menu_render(SSOLED *oled, char *data, uint data_size);
int home_menu_btnpressed(SSOLED *oled, char *data, uint data_size, uint gpio);
int home_menu_bgrefresh(SSOLED *oled, char *data, uint data_size, char in_foreground);
int home_menu_destroy(SSOLED *oled, char *data, uint data_size);

#endif