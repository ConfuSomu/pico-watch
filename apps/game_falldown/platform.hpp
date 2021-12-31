#pragma once
#include "../../hal/api.hpp"
#include "../../globals.hpp"

#include "bitset"
#define PLATFORM_TILE_SIZE 16
// The original game on TI-84 Plus had 12 tiles per platform
#define PLATFORM_NUM_TILES (DISPLAY_WIDTH/PLATFORM_TILE_SIZE)

class Platform {
    private:
        int m_position_y;
        int m_speed;
        
        // Bitset where solid tiles have a size of 
        std::bitset<PLATFORM_NUM_TILES> m_tiles_solid;

        void create_tiles();
    public:
        Platform();
        Platform(int y = DISPLAY_HEIGHT-1, int speed = 1);
        void update();
        void draw(Api *app_api);
};
