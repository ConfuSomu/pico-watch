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
    sleep_ms(500); // Wait for the OLED to settle
    oledInit(&m_oled, OLED_128x64, 0x3d, 0, 0, 1, SDA_PIN, SCL_PIN, RESET_PIN, 1000000L);
    oledFill(&m_oled, 0,1);
    oledSetContrast(&m_oled, OLED_DEFAULT_CONTRAST);
    oledSetBackBuffer(&m_oled, m_ucBuffer); // Seems to be required to draw lines, rectangles…
    //oledSetTextWrap(&oled, true);
}

void Api::display_power(bool mode) {
    oledPower(&m_oled, mode);
}

int Api::display_write_string(int iScrollX, int x, int y, const char *szMsg, int iSize, int bInvert, int bRender) {
    oledWriteString(&m_oled, iScrollX, x, y, szMsg, iSize, bInvert, 0);
    m_writebb_needed = true;
}

void Api::display_fill(unsigned char ucData, int bRender) {
    oledFill(&m_oled, ucData, bRender);
}

void Api::display_draw_line(int x1, int y1, int x2, int y2, int bRender) {
    oledDrawLine(&m_oled, x1, y1, x2, y2, 0);
    m_writebb_needed = true;
}

void Api::display_draw_rectange(int x1, int y1, int x2, int y2, uint8_t ucColor, uint8_t bFilled) {
    oledRectangle(&m_oled, x1, y1, x2, y2, ucColor, bFilled);
    m_writebb_needed = true; // Write the back buffer, after experimentation, seems to be required when drawing this shape
}

void Api::display_draw_ellipse(int iCenterX, int iCenterY, int32_t iRadiusX, int32_t iRadiusY, uint8_t ucColor, uint8_t bFilled) {
    oledEllipse(&m_oled, iCenterX, iCenterY, iRadiusX, iRadiusY, ucColor, bFilled);
    m_writebb_needed = true;
}

void Api::display_write_buffer(uint8_t *pBuffer) {
    oledDumpBuffer(&m_oled, pBuffer);
}

void Api::display_write_backbuffer() {
    if (m_writebb_needed)
        oledDumpBuffer(&m_oled, m_ucBuffer);
    m_writebb_needed = false;
}

int Api::display_write_pixel(int x, int y, unsigned char ucColor, int bRender) {
    return oledSetPixel(&m_oled, x, y, ucColor, bRender);
}

void Api::gui_popup_generic(std::string &title, std::string &body, int max_title_length, int max_body_length) {
    oledRectangle(&m_oled, 9,7, 119,63, 0, 1); // Background
    oledRectangle(&m_oled, 9,7, 119,63, 1, 0); // Popup border
    oledRectangle(&m_oled, 9,7, 119,16, 1, 1); // Title background, FIXME pixel bleeding
    m_writebb_needed = true; this->display_write_backbuffer(); // Display rectangle and anything else behind it (drawn before), could be moved after writing strings (but not done for debugging)

    // Truncate longer strings to avoid wasting time in for loop and drawing on OLED
    if (max_title_length > 13) max_title_length = 13;
    if (max_body_length > 78) max_body_length = 78;
    if (title.size() > max_title_length)
        title.resize(13);
    if (body.size() > max_body_length)
        body.resize(78);

    // Make body fit by adding '\n' at a regular interval
    #define CHARS_PER_LINE 13
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
    oledWriteString(&m_oled, 0, 15,1, title.c_str(), FONT_8x8, 1, 1); // Draw title
    oledWriteString(&m_oled, 0, 13,2, body.c_str(), FONT_8x8, 0, 1); // Draw body
}

bool Api::gui_popup_text(std::string title, std::string body){
    m_button_last_pressed = BUTTON_NONE;
    m_send_button_press_to_app = false;

    gui_popup_generic(title, body);

    while (m_button_last_pressed != BUTTON_SELECT)
        sleep_ms(50); // TODO: use _wfi()
    // Give back control to running app
    oledFill(&m_oled, 0, 1);
    m_send_button_press_to_app = true;
    return true;
}

bool Api::gui_popup_booleanchoice(std::string title, std::string body){
    m_button_last_pressed = BUTTON_NONE;
    m_send_button_press_to_app = false;

    title.insert(0, "Choice|"); // TODO: Could be made nicer with a custom char that uses the whole height, this would give a visible separation, with two "text blocks" composing the title
    gui_popup_generic(title, body);

    while (m_button_last_pressed != BUTTON_SELECT and m_button_last_pressed != BUTTON_MODE)
        sleep_ms(50); // TODO: use _wfi()

    bool choice;
    switch (m_button_last_pressed) {
        case BUTTON_SELECT:
            choice = true;
            break;
        case BUTTON_MODE:
            choice = false;
            break;
        default:
            __breakpoint(); // Impossible to attain (but you never know…)
    }
    // Give back control to running app
    oledFill(&m_oled, 0, 1);
    m_send_button_press_to_app = true;
    return choice;
}

void Api::gui_popup_intchoice_footer(int current_num, int min_num, int max_num) {
    char buf[30];
    snprintf(&buf[0], sizeof(buf),
            "Select: %d (%d/%d)",
            current_num,
            min_num,
            max_num);
    oledRectangle(&m_oled, 9,55, 119,63, 1, 1); // Footer background, FIXME pixel bleeding
    oledWriteString(&m_oled, 0,10,7, buf, FONT_6x8, 1, 0);
    m_writebb_needed = true; display_write_backbuffer();
}

int Api::gui_popup_intchoice(std::string title, std::string body, int min_num, int max_num, int default_num, int step){
    m_button_last_pressed = BUTTON_NONE;
    m_send_button_press_to_app = false;

    int current_num = default_num;

    title.insert(0, "Number|"); // TODO: Could be made nicer with a custom char instead of pipe char
    gui_popup_generic(title, body, 13, 39); // 39: 3 lines of body text, to leave space for number
    gui_popup_intchoice_footer(current_num, min_num, max_num);

    do {
        m_button_last_pressed = BUTTON_NONE;
        sleep_ms(50); // TODO: use _wfi()
        switch (m_button_last_pressed) {
            case BUTTON_UP:
                current_num += step;
                if (current_num > max_num)
                    current_num = max_num;
                break;
            case BUTTON_DOWN:
                current_num -= step;
                if (current_num < min_num)
                    current_num = min_num;
                break;
            case BUTTON_MODE:
                current_num = default_num;
                break;
        }
        if (m_button_last_pressed)
            gui_popup_intchoice_footer(current_num, min_num, max_num);
    } while (m_button_last_pressed != BUTTON_SELECT);    

    // Give back control to running app
    oledFill(&m_oled, 0, 1);
    m_send_button_press_to_app = true;
    return current_num;
}

void Api::gui_popup_strchoice_footer(const char selection[]) {
    std::string buf{selection};
    buf.insert(0, "Select: ");

    if (buf.size() > 36) // 2 lines of 18 chars
        buf.resize(36);

    // Choose most adapted font size
    int font;
    int chars_per_line;
    if (buf.size() > 26) {
        font = FONT_6x8;
        chars_per_line = 18;
    } else {
        font = FONT_8x8;
        chars_per_line = 13;
    }

    // Make selection text fit by adding '\n' at a regular interval
    // TODO: Make this a private function
    int since_nl = 0; // Characters since newline
    for (std::string::size_type i = 0; i < buf.size(); ++i) {
        if (buf[i] == '\n')
            since_nl = 0;
        else if (since_nl++ == chars_per_line) {
            buf.insert(i, 1, '\n');
            since_nl = 0; // Probably unneeded
        }
    }

    oledRectangle(&m_oled, 9,47, 119,63, 1, 1); // Footer background, FIXME pixel bleeding
    oledWriteString(&m_oled, 0,10,6, buf.c_str(), font, 1, 0);
    m_writebb_needed = true; display_write_backbuffer();
}

int Api::gui_popup_strchoice(std::string title, std::string body, const char *choices[27], int choices_size, int min_index, int max_index, int default_index){
    m_button_last_pressed = BUTTON_NONE;
    m_send_button_press_to_app = false;
    if (max_index == -1)
        max_index = choices_size-1;

    int current_index = default_index;

    title.insert(0, "Choose|"); // TODO: Could be made nicer with a custom char instead of pipe char
    gui_popup_generic(title, body, 13, 39); // 39: 3 lines of body text, to leave space for selection string
    gui_popup_strchoice_footer(choices[current_index]);

    do {
        m_button_last_pressed = BUTTON_NONE;
        sleep_ms(50); // TODO: use _wfi()
        switch (m_button_last_pressed) {
            case BUTTON_UP:
                current_index += 1;
                if (current_index > max_index)
                    current_index = max_index;
                break;
            case BUTTON_DOWN:
                current_index -= 1;
                if (current_index < min_index)
                    current_index = min_index;
                break;
            case BUTTON_MODE:
                current_index = default_index;
                break;
        }
        if (m_button_last_pressed)
            gui_popup_strchoice_footer(choices[current_index]);
    } while (m_button_last_pressed != BUTTON_SELECT);    

    // Give back control to running app
    oledFill(&m_oled, 0, 1);
    m_send_button_press_to_app = true;
    return current_index;
}

bool Api::gui_footer_text(std::string text, int offset_x, int offset_row, bool invert, bool no_bg) {
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
    
    if (!no_bg) {
        oledRectangle(&m_oled, 0,56-offset_row*8, 127,64-offset_row*8, invert, 1);
        m_writebb_needed = true;
    }
    oledWriteString(&m_oled, 0,offset_x,7-offset_row, text.c_str(), font, invert, 0);
}

bool Api::gui_header_text(std::string text, int offset_x, int offset_row, bool invert, bool no_bg) {
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
    
    if (!no_bg) {
        oledRectangle(&m_oled, 0,0+offset_row*8, 127,8+offset_row*8, invert, 1);
        m_writebb_needed = true;
    }
    oledWriteString(&m_oled, 0,offset_x,0+offset_row, text.c_str(), font, invert, 0);
}

bool Api::performance_set(int perf) {
    return false;
}

bool Api::datetime_get(datetime_t *t) {
    return rtc_get_datetime(t);
}

bool Api::datetime_set(datetime_t *t) {
    return rtc_set_datetime(t);
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
