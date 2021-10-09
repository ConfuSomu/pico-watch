#pragma once

#include "../../api.hpp"
#include "../../../init.hpp"

class BaseWidget {
    private:
        // A pointer to the API is used by the paint method to draw the widget on-screen.
        Api *m_app_api;

    public:
        // Widget geometry, in pixels
        struct Geometry {
            int x = 0; // top-left corner of widget
            int y = 0; // top-left corner of widget
            int width = 1;
            int height = 1;
        } m_geometry;

        BaseWidget(Api *app_api, Geometry geometry) :
            m_app_api{app_api},
            m_geometry{geometry}
            {}
        virtual void paint(bool invert_colors = false) = 0;
};
