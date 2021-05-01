#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"
#include "../../api.hpp"

extern void app_switch(int old_appid, int new_appid);
extern bool rtc_get_datetime(datetime_t *t);

#define MAIN_SET_NUM 2
#define MAIN_SET_NUM_STR "2"
#define SIZE_SETTING_NAME 12
#define SET0_NAME "Date & Time"
#define SET1_NAME "Display"

#define SET0_DESC "Set date/time. Choose unit to change:"
#define SET0_0_DESC "Ajust selected unit. Use good values!"
#define SET0_1_DESC "Set the current month or day of week."

namespace app_settings {
    const char *MAIN_SET_NAMES[MAIN_SET_NUM] = {SET0_NAME, SET1_NAME};
    int selected_setting = 0;
    char display_setting_name[SIZE_SETTING_NAME];
    bool selected = false;

    void show_title(Api *app_api) {
        std::string title_str {"Settings (/" MAIN_SET_NUM_STR ")"};
        title_str.insert(10, std::to_string(selected_setting+1));
        app_api->gui_header_text(title_str);
    }

    // Set time
    void set0_menu(Api *app_api) {
        static const char *choices[16] = {"Hour", "Minute", "Second", "Year", "Month", "Day", "Day of week", "Apply and close"};
        uint max_value;
        uint min_value;
        uint default_value;
        datetime_t datetime;
        app_api->datetime_get(&datetime);

        int choice = 0;
        while (true) {
            choice = app_api->gui_popup_strchoice(SET0_NAME, SET0_DESC, choices, 8, 0, -1, choice);

            min_value = 0;
            switch (choice) {
                case 0: // Hour
                    max_value = 23;
                    default_value = datetime.hour;
                    break;
                case 1: // Minute
                    max_value = 59;
                    default_value = datetime.min;
                    break;
                case 2: // Second
                    max_value = 59;
                    default_value = datetime.sec;
                    break;
                case 3: // Year
                    max_value = 4095;
                    default_value = datetime.year;
                    break;
                case 5: // Day
                    max_value = 31; // FIXME: Depends on month!
                    min_value = 1;
                    default_value = datetime.month;
                    break;
                case 7: // Apply and exit
                    app_api->datetime_set(&datetime);
                    return;
            }

            // Display popup for editing value
            int new_value;
            if (choice == 4) { // Month
                // From newlib (which already includes this array of char arrays in the complied program). This should not take more space.
                // See newlib/libc/locale/timelocal.c (mirror @ https://github.com/bminor/newlib/blob/80cda9bbda04a1e9e3bee5eadf99061ed69ca5fb/newlib/libc/locale/timelocal.c#L40)
                static const char *month_choice[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

                datetime.month = app_api->gui_popup_strchoice(choices[choice], SET0_1_DESC, month_choice, 12, 0, -1, datetime.month) + 1;

            } else if (choice == 6) { // Day of week
                static const char *dotw_choice[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

                datetime.dotw = app_api->gui_popup_strchoice(choices[choice], SET0_1_DESC, dotw_choice, 7, 0, -1, datetime.dotw);
            
            } else {
                new_value = app_api->gui_popup_intchoice(choices[choice], SET0_0_DESC, min_value, max_value, default_value);
            }

            // Store the modification in the struct
            switch (choice) {
                case 0: datetime.hour = new_value; break;
                case 1: datetime.min  = new_value; break;
                case 2: datetime.sec  = new_value; break;
                case 3: datetime.year = new_value; break;
                // Month & date of week is stored directly after their special popup.
                case 5: datetime.day  = new_value; break;
            }
        }
    }

    // Rendering of app
    int render(Api *app_api) {
        show_title(app_api);
        app_api->display_write_string(0,0,3, display_setting_name, FONT_12x16, 0, 1);

        if (selected) {
            selected = false;
            switch (selected_setting) {
                case 0:
                    set0_menu(app_api);
                    break;
                case 1:
                    app_api->gui_popup_text(SET1_NAME, "Todo!");
                    break;
            }
        }

        return 0;
    }

    // Example of how button inputs could be interpreted.
    // Drawing on screen should be done in the render function.
    int btnpressed(Api *app_api, uint gpio, unsigned long delta) {
        switch (gpio) {
            case BUTTON_SELECT:
                selected = true;
                break;
            case BUTTON_DOWN:
                selected_setting--;
                break;
            case BUTTON_UP:
                selected_setting++;
                break;
        }
        if (selected_setting > MAIN_SET_NUM-1) {
            selected_setting = MAIN_SET_NUM-1;
        } else if (selected_setting < 0) {
            selected_setting = 0;
        }
        // Add spaces to avoid "ghost" characters from app names displayed before
        snprintf(display_setting_name, SIZE_SETTING_NAME, "%s             ", MAIN_SET_NAMES[selected_setting]);
        return 0;
    }

    // Initlisation of the app.
    int init(Api *app_api) {
        app_api->performance_set(Api::perf_modes::LOW_POWER);
        app_api->performance_render_interval_set(100);
        selected_setting = 0;
        selected = false;
        snprintf(display_setting_name, SIZE_SETTING_NAME, "%s", MAIN_SET_NAMES[0]);
        return Api::app_init_return_status::OK;
    }

    // Processor intensive operations and functions related to drawing to the screen should only be done when the app is in_foreground(=1). This function is only called when the app is init.
    int bgrefresh(Api *app_api, bool in_foreground) {
        return 1;
    }

    // Destruction of app, deinitlisation should be done here. This is only called if the app's APPS_DESTROY_ON_EXIT is set to 1. When it is not a "service" app.
    int destroy(Api *app_api) {
        return 0;
    }
}
