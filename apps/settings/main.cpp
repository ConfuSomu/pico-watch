#include <stdio.h>
#include "pico/stdlib.h"

#include "main.hpp"
#include "../../globals.hpp"
#include "strings.hpp"

extern bool rtc_get_datetime(datetime_t *t);

void app_settings::show_title(Api *app_api) {
    std::string title_str {"Settings (/" MAIN_SET_NUM_STR ")"};
    title_str.insert(10, std::to_string(selected_setting+1));
    app_api->gui_header_text(title_str);
}

// Time and date settings
void app_settings::set0_menu(Api *app_api) {
    #define NUM_CHOICES 9
    static const char *choices[NUM_CHOICES] = {"Hour", "Minute", "Second", "Year", "Month", "Day", "Day of week", STR_SET_APPLY, STR_SET_CANCEL};
    uint max_value;
    uint min_value;
    uint default_value;
    datetime_t datetime;
    app_api->datetime_get(&datetime);

    int choice = 0;
    while (true) {
        choice = app_api->gui_popup_strchoice(SET0_NAME, SET0_DESC, choices, NUM_CHOICES, 0, -1, choice);

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
                default_value = datetime.day;
                break;
            case 7: // Apply and exit
                app_api->datetime_set(&datetime);
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
    #undef NUM_CHOICES
}

// Display settings
void app_settings::set1_menu(Api *app_api) {
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
BaseApp::AppReturnValues app_settings::render(Api *app_api) {
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

    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_settings::btnpressed(Api *app_api, uint gpio, unsigned long delta) {
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
    return AppReturnValues::OK;
}

app_settings::app_settings(Api *app_api) {
    app_api->performance_set(Api::perf_modes::LOW_POWER);
    app_api->performance_render_interval_set(100);
    selected_setting = 0;
    selected = false;
    snprintf(display_setting_name, SIZE_SETTING_NAME, "%s", MAIN_SET_NAMES[0]);
}

BaseApp::AppReturnValues app_settings::bgrefresh(Api *app_api, bool in_foreground) {
    return AppReturnValues::OK;
}

app_settings::~app_settings() {
}
