#include "ball.hpp"

Ball::Ball(int x, int y, int speed) {
    m_position[0] = x;
    m_position[1] = y;
    m_speed = speed;
}

void Ball::update() {
    m_position[1] += 1 * m_speed;

    if (m_pressed_left)
        m_position[0] -= 5 * m_speed;
    if (m_pressed_right)
        m_position[0] += 5 * m_speed;

    if (m_position[0] > DISPLAY_WIDTH)
        m_position[0] = DISPLAY_WIDTH;
    else if (m_position[0] < 0)
        m_position[0] = 0;
    
    if (m_position[1] > DISPLAY_HEIGHT)
        m_position[1] = DISPLAY_HEIGHT;
    else if (m_position[1] < 0) {
        m_position[1] = 0;
        m_has_died = true;
    }
}

void Ball::draw(Api *app_api) {
    app_api->display_draw_ellipse((int)m_position[0], (int)m_position[1], m_radius, m_radius, 1, 1);
}