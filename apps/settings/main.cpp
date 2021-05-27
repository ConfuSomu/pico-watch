#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"
#include "../../api.hpp"

extern void app_switch(int old_appid, int new_appid);
extern bool rtc_get_datetime(datetime_t *t);

#define MAIN_SET_NUM 2
#define MAIN_SET_NUM_STR "2"
#define SIZE_SETTING_NAME 12
#define STR_SET_APPLY "Apply and close"
#define STR_SET_CANCEL "Quit without saving"
#define SET0_NAME "Date & Time"
#define SET1_NAME "Display"

#define SET0_DESC "Set date/time. Choose unit to change:"
#define SET0_0_DESC "Adjust selected unit. Use good values!"
#define SET0_1_DESC "Set the current month or day of week."

#define SET1_DESC "Adjust settings related to OLED display."
#define SET1_0_DESC "Adjust display brightness."
#define SET1_1_DESC "Time before turning off OLED and entering low power."
#define SET1_2_DESC "Set display time format.\nCurrent:\nY: 24h\nN: AM/PM"
#define SET1_2_DESC_INDEX_CURRENT 33 // Don't forget me!
#define SET1_1_MIN 5000
// According to https://stackoverflow.com/questions/589575/what-does-the-c-standard-state-the-size-of-int-long-type-to-be :
#define SET1_1_MAX 65500
#define SET1_1_STEP 500

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

    // Time and date settings
    void set0_menu(Api *app_api) {
        static const char *choices[9] = {"Hour", "Minute", "Second", "Year", "Month", "Day", "Day of week", STR_SET_APPLY, STR_SET_CANCEL};
        uint max_value;
        uint min_value;
        uint default_value;
        datetime_t datetime;
        app_api->datetime_get(&datetime);

        int choice = 0;
        while (true) {
            choice = app_api->gui_popup_strchoice(SET0_NAME, SET0_DESC, choices, 9, 0, -1, choice);

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
                case 8: // Quit without saving
                    return;
            }

            // Display popup for editing value
            int new_value;
            if (choice == 4) { // Month
                // From newlib (which already includes this array of char arrays in the complied program). This should not take more space.
                // See newlib/libc/locale/timelocal.c (mirror @ https://github.com/bminor/newlib/blob/80cda9bbda04a1e9e3bee5eadf99061ed69ca5fb/newlib/libc/locale/timelocal.c#L40)
                static const char *month_choice[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

                datetime.month = app_api->gui_popup_strchoice(choices[choice], SET0_1_DESC, month_choice, 12, 0, -1, datetime.month - 1) + 1;

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

    // Display settings
    void set1_menu(Api *app_api) {
        #define NUM_CHOICES 4
        static const char *choices[NUM_CHOICES] = {"Display brightness", "Sleep timeout", "Time format", STR_SET_APPLY};

        // There is no space for the "AM/PM" as this would push the last "M" on a new line, to make it nicer, a space could be afforded before the "24h" text.
        static const char *time_format[2] = {"AM/PM", " 24h"};

        int choice = 0;
        while (true) {
            choice = app_api->gui_popup_strchoice(SET1_NAME, SET1_DESC, choices, NUM_CHOICES, 0,-1,choice);

            switch(choice) {
                case 0: // Display brightness
                    g_user.oled_contrast = app_api->gui_popup_intchoice(SET1_NAME, SET1_0_DESC, 1, 255, g_user.oled_contrast, 5);
                    app_api->display_set_contrast(g_user.oled_contrast);
                    break;
                case 1: // Sleep timeout
                    g_user.sleep_delay = app_api->gui_popup_intchoice(SET1_NAME, SET1_1_DESC, SET1_1_MIN, SET1_1_MAX, g_user.sleep_delay, SET1_1_STEP);
                    break;
                case 2: // Time format
                    // TODO: Rewrite this, one day
                    g_user.time_format = app_api->gui_popup_booleanchoice(SET1_NAME, ((std::string)SET1_2_DESC).insert(SET1_2_DESC_INDEX_CURRENT, time_format[(int)g_user.time_format]));
                    break;
                case 3:
                    return;
            }
        }
        #undef NUM_CHOICES
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
                    set1_menu(app_api);
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
