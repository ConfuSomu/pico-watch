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