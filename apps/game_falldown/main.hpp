#pragma once

#include <vector>

// Includes also buttons, API and ss_oled
#include "../../app/base_app.hpp"

#include "ball.hpp"
#include "platform.hpp"

class app_game_falldown : public BaseApp {
    private:
        enum GameStates {menu, game, gameover};

        GameStates m_gamestate = menu;
        Api* m_app_api;

        Ball m_player_ball = {0,0};
        std::vector<Platform> m_platforms;

        void draw_main_menu();
        void draw_game();
        void draw_gameover_menu();
        void init_game();
        void update_game();

        AppAttributes app_attributes = {1, true,
            30}; // Around 30 fps
    public:
        const AppAttributes& app_get_attributes() {
            return app_attributes;
        }

        app_game_falldown(Api *app_api);
        AppReturnValues render(Api *app_api);
        AppReturnValues btn_pressed(Api *app_api, uint gpio, unsigned long delta);
        AppReturnValues btn_released(Api *app_api, uint gpio, unsigned long delta);
        ~app_game_falldown();
};
