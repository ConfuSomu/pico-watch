#ifndef __MAIN_CLOCK_H__
#define __MAIN_CLOCK_H__

#include "pico/util/datetime.h"
#include "../oled/ss_oled.h"

void time_as_str(char *buf, uint buf_size, const datetime_t *t);
void date_as_str(char *buf, uint buf_size, const datetime_t *t);
void show_datetime(SSOLED *oled);

int main_clock_init(SSOLED *oled, char *data, uint data_size);
int main_clock_render(SSOLED *oled, char *data, uint data_size);
int main_clock_btnpressed(SSOLED *oled, char *data, uint data_size, uint gpio);
int main_clock_bgrefresh(SSOLED *oled, char *data, uint data_size);
int main_clock_destroy(SSOLED *oled, char *data, uint data_size);

#endif