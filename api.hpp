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
        void init_display();
    public:
        bool m_send_button_press_to_app = true;
        void init();
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
        bool datetime_get(datetime_t *t);
        // Get last button pressed, see buttons.hpp for values
        uint button_last_get();
        // Set last button pressed, should only be called by button gpio interrupt.
        void button_last_set(uint gpio);
};

#endif