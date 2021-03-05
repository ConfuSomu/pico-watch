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
        uint8_t m_ucBuffer[1024] = {0};
        uint m_button_last_pressed = 0;
        int m_app_render_interval = 500;
        void init_display();
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
        int display_write_pixel(int x, int y, unsigned char ucColor, int bRender);
        // Display a popup over the current view and wait for select button to be pressed.
        // This is a blocking function and should be used only in the app's render method.
        // \param title Popup's title, length should not exceed 13 characters.
        // \param body String containing the popup's body. The zone has a size of 13×6 characters, so body should not be longer than 78 characters. Newline allows going to the next line and the text is automatically wrapped.
        // \note Strings longer than 13 and 78 respectively will be truncated.
        bool gui_popup_text(std::string title, std::string body);
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