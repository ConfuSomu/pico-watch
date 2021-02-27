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
    //oledSetTextWrap(&oled, true);
}

int Api::dispWriteString(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender) {
    return oledWriteString(&m_oled, iScrollX, x, y, szMsg, iSize, bInvert, bRender);
}

void Api::dispFill(unsigned char ucData, int bRender) {
    oledFill(&m_oled, ucData, bRender);
}

bool Api::getDatetime(datetime_t *t) {
    return rtc_get_datetime(t);
}
