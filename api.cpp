#include <stdio.h>
#include "pico/stdlib.h"
extern "C" {
#include "hardware/rtc.h"
}

#include "api.hpp"
#include "init.hpp"

void Api::init() {
    if (!m_init_done) {
        init_display();
        m_init_done = 1;
    }
}

void Api::init_display() {
    oledInit(&m_oled, OLED_128x64, 0x3d, 0, 0, 1, SDA_PIN, SCL_PIN, RESET_PIN, 1000000L);
    oledFill(&m_oled, 0,1);
    oledSetContrast(&m_oled, OLED_DEFAULT_CONTRAST);
    oledSetBackBuffer(&m_oled, m_ucBuffer); // Seems to be required to draw lines, rectangles…
    //oledSetTextWrap(&oled, true);
}

void Api::display_power(bool mode) {
    oledPower(&m_oled, mode);
}

int Api::display_write_string(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender) {
    return oledWriteString(&m_oled, iScrollX, x, y, szMsg, iSize, bInvert, bRender);
}

void Api::display_fill(unsigned char ucData, int bRender) {
    oledFill(&m_oled, ucData, bRender);
}

void Api::display_draw_line(int x1, int y1, int x2, int y2, int bRender) {
    oledDrawLine(&m_oled, x1, y1, x2, y2, bRender);
}

void Api::display_draw_rectange(int x1, int y1, int x2, int y2, uint8_t ucColor, uint8_t bFilled) {
    oledRectangle(&m_oled, x1, y1, x2, y2, ucColor, bFilled);
    oledDumpBuffer(&m_oled, m_ucBuffer); // Write the back buffer, after experimentation, seems to be required when drawing this shape
}

void Api::display_draw_ellipse(int iCenterX, int iCenterY, int32_t iRadiusX, int32_t iRadiusY, uint8_t ucColor, uint8_t bFilled) {
    oledEllipse(&m_oled, iCenterX, iCenterY, iRadiusX, iRadiusY, ucColor, bFilled);
    oledDumpBuffer(&m_oled, m_ucBuffer);
}

void Api::display_write_buffer(uint8_t *pBuffer) {
    oledDumpBuffer(&m_oled, pBuffer);
}

int Api::display_write_pixel(int x, int y, unsigned char ucColor, int bRender) {
    return oledSetPixel(&m_oled, x, y, ucColor, bRender);
}

bool Api::gui_popup_text(std::string title, std::string body){
#define CHARS_PER_LINE 13
    m_button_last_pressed = 0;
    m_send_button_press_to_app = false;

    oledRectangle(&m_oled, 9,7, 119,63, 0, 1); // Background
    oledRectangle(&m_oled, 9,7, 119,63, 1, 0); // Popup border
    oledRectangle(&m_oled, 9,7, 119,16, 1, 1); // Title background, FIXME pixel bleeding
    oledDumpBuffer(&m_oled, m_ucBuffer); // Display rectangle

    // Truncate longer strings to avoid wasting time in for loop and drawing on OLED
    if (title.size() > 13)
        title.resize(13);
    if (body.size() > 78)
        body.resize(78);

    // Make body fit by adding '\n' at a regular interval
    int since_nl = 0; // Characters since newline
    for (std::string::size_type i = 0; i < body.size(); ++i) {
        if (body[i] == '\n')
            since_nl = 0;
        else if (since_nl++ == CHARS_PER_LINE) {
            body.insert(i, 1, '\n');
            since_nl = 0; // Probably unneeded
        }
    }
    // See https://stackoverflow.com/questions/1986966/does-s0-point-to-contiguous-characters-in-a-stdstring
    oledWriteString(&m_oled, 0, 15,1, &title[0], FONT_8x8, 1, 1); // Draw title
    oledWriteString(&m_oled, 0, 13,2, &body[0], FONT_8x8, 0, 1); // Draw body
    while (m_button_last_pressed != BUTTON_SELECT)
        sleep_ms(50);
    // Give back control to running app
    oledFill(&m_oled, 0, 1);
    m_send_button_press_to_app = true;
}

bool Api::gui_footer_text(std::string text, int offset_x, int offset_row, int invert) {
    // Max chars per line for FONT_8x8 is 16 chars
    // Max chars per line for FONT_6x8 is 21 chars
    // Truncate longer text
    if (text.size() > 21)
        text.resize(21);
    
    // Choose most adapted font size
    int font;
    if (text.size() > 16)
        font = FONT_6x8;
    else
        font = FONT_8x8;
    
    oledRectangle(&m_oled, 0,56, 128,64, invert, 1);
    oledDumpBuffer(&m_oled, m_ucBuffer);
    oledWriteString(&m_oled, 0,offset_x,7-offset_row, &text[0], font, invert, 1);
}

bool Api::gui_header_text(std::string text, int offset_x, int offset_row, int invert) {
    // Max chars per line for FONT_8x8 is 16 chars
    // Max chars per line for FONT_6x8 is 21 chars
    // Truncate longer text
    if (text.size() > 21)
        text.resize(21);
    
    // Choose most adapted font size
    int font;
    if (text.size() > 16)
        font = FONT_6x8;
    else
        font = FONT_8x8;
    
    oledRectangle(&m_oled, 0,0, 128,8, invert, 1);
    oledDumpBuffer(&m_oled, m_ucBuffer);
    oledWriteString(&m_oled, 0,offset_x,0+offset_row, &text[0], font, invert, 1);
}

bool Api::performance_set(int perf) {
    return false;
}

bool Api::datetime_get(datetime_t *t) {
    return rtc_get_datetime(t);
}

int Api::performance_render_interval_get() {
    return m_app_render_interval;
}

void Api::performance_render_interval_set(int interval) {
    if (interval < 10)
        interval = 10;
    m_app_render_interval = interval;
}

uint Api::button_last_get() {
    return m_button_last_pressed;
}

void Api::button_last_set(uint gpio) {
    m_button_last_pressed = gpio;
}
