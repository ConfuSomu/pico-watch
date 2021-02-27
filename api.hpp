#ifndef __API_H__
#define __API_H__

#include "oled/ss_oled.h"

class Api {
    private:
        SSOLED m_oled;
        u_char m_init_done = 0;
        void init_display();
    public:
        void init();
        int dispWriteString(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender);
        void dispFill(unsigned char ucData, int bRender);
};

#endif