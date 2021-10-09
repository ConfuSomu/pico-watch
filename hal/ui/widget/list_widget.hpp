#pragma once

#include "base_widget.hpp"

class ListWidget : private BaseWidget {
    public:
        ListWidget(Api *app_api, Geometry geometry);
        void paint(bool invert_colors = false);
};
