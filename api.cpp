#include <stdio.h>
#include "pico/stdlib.h"
extern "C" {
#include "hardware/rtc.h"
}
#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

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

bool Api::datetime_get(datetime_t *t) {
    return rtc_get_datetime(t);
}
