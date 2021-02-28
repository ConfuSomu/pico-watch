#ifndef __API_H__
#define __API_H__

#include "pico/util/datetime.h"
#include "oled/ss_oled.h"

class Api {
    private:
        SSOLED m_oled;
        u_char m_init_done = 0;
        uint8_t m_ucBuffer[1024] = {0};
        void init_display();
    public:
        void init();
        int display_write_string(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender);
        void display_fill(unsigned char ucData, int bRender);
        void display_draw_line(int x1, int y1, int x2, int y2, int bRender);
        void display_draw_rectange(int x1, int y1, int x2, int y2, uint8_t ucColor, uint8_t bFilled);
        void display_draw_ellipse(int iCenterX, int iCenterY, int32_t iRadiusX, int32_t iRadiusY, uint8_t ucColor, uint8_t bFilled);
        void display_write_buffer(uint8_t *pBuffer);
        int display_write_pixel(int x, int y, unsigned char ucColor, int bRender);
        bool datetime_get(datetime_t *t);
};

#endif