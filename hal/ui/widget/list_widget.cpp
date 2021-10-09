#include "list_widget.hpp"

ListWidget::ListWidget(Api *app_api, Geometry geometry) {
    printf("On init: m_x=%d and x=%d", m_geometry.x, geometry.x);
}

void ListWidget::paint(bool invert_colors) {
    m_app_api->display_draw_rectange(m_geometry.x, m_geometry.y, m_geometry.x+m_geometry.width, m_geometry.y+m_geometry.height, 1, 1);
}
