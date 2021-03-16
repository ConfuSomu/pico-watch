#ifndef __API_H__
#define __API_H__

#include <iostream>
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

#include "buttons.hpp"

class Api {
    private:
        SSOLED m_oled;
        u_char m_init_done = 0;
        bool m_writebb_needed = false; // Do we need to write the internal backbuffer?
        uint8_t m_ucBuffer[1024] = {0};
        uint m_button_last_pressed = 0;
        int m_app_render_interval = 500;
        void init_display();
        // Careful of large heap usage with long strings! Heap seems to be fragmented, so shorter strings work best. More details on findings below:
        // When the string cannot be created, this comes from the fact that we, on some runs, allocation can be very close to the SRAM's boundry: 0x20041f88 with boundry at 0x20042000. The heap is nearly full. Even just appending to a string moves the allocation lower. I think that the best course of action would be to have more static variables and pull in less things to save SRAM.
        // When char* directly passed as parameter: The memory is possibly fragmented as long strings (>215) push the allocation downwards, into lower (higher address) in the heap. title is at 0x20041f90 and body at 0x200045e8, for length of 215 chars.
        void gui_popup_generic(std::string &title, std::string &body, int max_title_length = 13, int max_body_length = 78);
        void gui_popup_intchoice_footer(int current_num, int min_num, int max_num);
    public:
        bool m_send_button_press_to_app = true;
        enum perf_modes {
            LOW_POWER,
            NORMAL_PERF,
            HIGH_PERF,
            ENTER_SHALLOW_SLEEP,
            EXIT_SHALLOW_SLEEP // Restore perf setting
        };
        void init();
        // Control the display's power (on or off)
        void display_power(bool mode);
        int display_write_string(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender);
        void display_fill(unsigned char ucData, int bRender);
        void display_draw_line(int x1, int y1, int x2, int y2, int bRender);
        void display_draw_rectange(int x1, int y1, int x2, int y2, uint8_t ucColor, uint8_t bFilled);
        void display_draw_ellipse(int iCenterX, int iCenterY, int32_t iRadiusX, int32_t iRadiusY, uint8_t ucColor, uint8_t bFilled);
        void display_write_buffer(uint8_t *pBuffer);
        // Write the internal backbuffer to the display. Should be called when after all drawing calls. One call is done to avoid flickering of the display.
        void display_write_backbuffer();
        int display_write_pixel(int x, int y, unsigned char ucColor, int bRender);
        // Display a popup over the current view and wait for select button to be pressed.
        // This is a blocking function and should be used only in the app's render method.
        // \param title Popup's title, length should not exceed 13 characters.
        // \param body String containing the popup's body. The zone has a size of 13×6 characters, so body should not be longer than 78 characters. Newline allows going to the next line and the text is automatically wrapped.
        // \note Strings longer than 13 and 78 respectively will be truncated.
        bool gui_popup_text(std::string title, std::string body);
        // Display a popup over the current view and wait for select or mode (cancel) button to be pressed. The choice done (yes/no) by the user is returned as a bool.
        // This is a blocking function and should be used only in the app's render method.
        // \param title Popup's title. The title is prefixed with "Choice|", so the `title` argument cannot exceed 6 characters.
        // \param body String containing the popup's body. The zone has a size of 13×6 characters, so body should not be longer than 78 characters. Newline allows going to the next line and the text is automatically wrapped.
        // \note Strings longer than 13 and 78 respectively will be truncated.
        bool gui_popup_booleanchoice(std::string title, std::string body);
        // Display a popup over the current view and wait for user to choose (with left and right) a number between min_num and max_num. The default choice is default_num and the user can reset back to it with mode/cancel button. After confirming with select, the choice is returned.
        // This is a blocking function and should be used only in the app's render method.
        // \param title Popup's title. The title is prefixed with "Number|", so the `title` argument cannot exceed 6 characters.
        // \param body String containing the popup's body. The zone has a size of 13×3 characters, so body should not be longer than 39 characters. Newline allows going to the next line and the text is automatically wrapped. Under the body is displayed the current choosen number with the min and max in parenthesis.
        // \param min_num The smallest number that can be choosen.
        // \param max_num Biggest number that can be choosen.
        // \param default_num This should be between min_num and max_num, else user may be able to return a number out of range
        // \param step Value to increment/decrement from when user changes number. This cannot result in an out-of-bounds as the number is clipped to the min/max when this happens. This maybe undesirable behaviour.
        // \note Strings longer than 13 and 39 respectively will be truncated.
        int gui_popup_intchoice(std::string title, std::string body, int min_num = 0, int max_num = 10, int default_num = 5, int step = 1);
        // Display text at the bottom of the screen.
        // The font size is automatically choosen based on the text lenght.
        // \param text Text to display. Text longer than 21 will be truncated.
        // \param offset_x Set a horizental offset, to allow, for example, centering the text
        // \param offset_row Allow rendering the text higher. For example, one line higher when `offset_row = 1`.
        // \param invert Invert text and background color.
        // \param no_bg Do not draw background when true.
        bool gui_footer_text(std::string text, int offset_x = 0, int offset_row = 0, bool invert = false, bool no_bg = false);
        // Display text at the top of the screen.
        // The font size is automatically choosen based on the text lenght.
        // \param text Text to display. Text longer than 21 will be truncated.
        // \param offset_x Set a horizental offset, to allow, for example, centering the text
        // \param offset_row Render text lines lower. For example, one text line lower with `offset_row = 1`.
        // \param invert Invert text and background color.
        // \param no_bg Do not draw background when true.
        bool gui_header_text(std::string text, int offset_x = 0, int offset_row = 0, bool invert = false, bool no_bg = false);
        // Set performance mode.
        // FIXME: function currently does nothing!
        // An app should choose the lowest performance that can make it function. Set in init(). Only when required, higher performance should be used.
        // \param perf See Api::perf_modes enum for possible values
        bool performance_set(int perf);
        bool datetime_get(datetime_t *t);
        // Get app's current render interval
        // \return Value in millisec
        int performance_render_interval_get();
        // Same spirit as performance_set, the app should use the smallest value possible that does not cause a lack of responsiveness in the app's UI.
        // \param interval Time to wait in millisec between calls of the app's render function.
        void performance_render_interval_set(int interval);
        // Get last button pressed, see buttons.hpp for values
        // \return Last button pressed
        uint button_last_get();
        // Set last button pressed, should only be called by button gpio interrupt.
        void button_last_set(uint gpio);
};

#endif