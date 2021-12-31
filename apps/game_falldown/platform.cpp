#include "platform.hpp"

Platform::Platform() {
    m_position_y = DISPLAY_HEIGHT-1;
    m_speed = 1;

    create_tiles();
}

Platform::Platform(int y, int speed) {
    m_position_y = y;
    m_speed = speed;

    create_tiles();
}

void Platform::create_tiles() {
    // example of platform creation
    for (size_t i = 1; i < m_tiles_solid.size(); i += 2) {
        m_tiles_solid.set(i);
    }
}

void Platform::update() {
    m_position_y -= m_speed * 1;
}

void Platform::draw(Api *app_api) {
    // display_draw_line(int x1, int y1, int x2, int y2, int bRender);
    // for each tile
    // FIXME: Is broken
    int position_x = 0;
    for (size_t i = 0; i < m_tiles_solid.size(); ++i) {
        if (m_tiles_solid[i]) {
            int tile_start_x = PLATFORM_TILE_SIZE * i;
            int tile_end_x = tile_start_x + PLATFORM_TILE_SIZE;

            app_api->display_draw_line(tile_start_x, m_position_y, tile_end_x, m_position_y, 1);
        }
    }
}