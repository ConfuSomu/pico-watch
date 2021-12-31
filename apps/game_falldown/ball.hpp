#pragma once
#include "../../hal/api.hpp"
#include "../../globals.hpp"

class Ball {
    private:
        float m_position[2];
        int m_speed;
        int m_radius = 5;

    public:
        bool m_pressed_left = false;
        bool m_pressed_right = false;

        bool m_has_died = false;

        Ball(int x, int y, int speed = 1);
        void update();
        void draw(Api *app_api);
};
