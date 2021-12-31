#include "main.hpp"

// TODO in this app
// - 

void app_game_falldown::update_game() {
    m_player_ball.update();
    for (auto platform : m_platforms) {
        platform.update();
    }

    if (m_player_ball.m_has_died)
        m_gamestate = gameover;
}

void app_game_falldown::draw_game() {
    m_app_api->display_fill(0, 0);
    m_player_ball.draw(m_app_api);
    for (auto platform : m_platforms) {
        platform.draw(m_app_api);
    }
}

void app_game_falldown::draw_main_menu() {
    m_app_api->gui_header_text("A Falldown clone", 0,0,true);
    m_app_api->gui_header_text("Press select to start", 0,2);
    m_app_api->gui_header_text("In game:", 0,3);
    m_app_api->gui_header_text("up/down btns to move", 0,4);
}

void app_game_falldown::draw_gameover_menu() {
    m_app_api->gui_header_text("Game Over!", 30,2,true);
}

void app_game_falldown::init_game() {
    // place the player
    m_platforms.push_back(30);
    m_gamestate = game;
}

BaseApp::AppReturnValues app_game_falldown::render(Api *app_api) {
    switch (m_gamestate) {
        case menu:
            draw_main_menu(); break;
        case game:
            update_game(); draw_game(); break;
        case gameover:
            draw_gameover_menu(); break;
    }

    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_game_falldown::btn_pressed(Api *app_api, uint gpio, unsigned long delta) {
    switch (m_gamestate) {
        case menu:
            if (gpio == BUTTON_SELECT)
                init_game();
            break;
        case game:
            switch (gpio) {
                case BUTTON_DOWN:
                    m_player_ball.m_pressed_left = true; break;
                case BUTTON_UP:
                    m_player_ball.m_pressed_right = true; break;
            }
            break;
        case gameover:
            draw_gameover_menu(); break;
    }

    return AppReturnValues::OK;
}

BaseApp::AppReturnValues app_game_falldown::btn_released(Api *app_api, uint gpio, unsigned long delta) {
    if (m_gamestate == game) {
        switch (gpio) {
            case BUTTON_DOWN:
                m_player_ball.m_pressed_left = false; break;
            case BUTTON_UP:
                m_player_ball.m_pressed_right = false; break;
        }
    }
    return AppReturnValues::OK;
}

app_game_falldown::app_game_falldown(Api *app_api) {
    m_app_api = app_api;
}

app_game_falldown::~app_game_falldown() {
}
