#include <algorithm>

#include "app_manager.hpp"
#include "../hal/api.hpp"
#include "../globals.hpp"

// App classes following:
#include "../apps/main_clock/main.hpp"
#include "../apps/home_menu/main.hpp"
#include "../apps/settings/main.hpp"
#define NUMBER_OF_APPS 3

// From pico-watch.c:
extern Api app_api;

std::vector<BaseApp*> app_mgr::open_apps;

BaseApp* app_mgr::app_check_if_init(int app_id) {
    for (auto app : open_apps) {
        if (app_id == app->app_get_attributes().id)
            return app;
    }
    return nullptr;
}

void app_mgr::new_foreground_app(BaseApp* app) {
    app_api.display_fill(0,1); // Clear OLED
    g_s.foreground_app = app;
}

void app_mgr::app_act_on_return_value(BaseApp* app, BaseApp::AppReturnValues return_value) {
    switch (return_value) {
        case BaseApp::AppReturnValues::OK:
            break;

        case BaseApp::AppReturnValues::QUIT:
            app_destroy(app);
            // No break
        
        case BaseApp::AppReturnValues::CLOSE:
            new_foreground_app(open_apps.front()); // The app has to be in foreground as the current function is called by app_render and app_btnpress
            break;

        default:
            printf("Unidentified return value %d for app %d at %x", return_value, app->app_get_attributes().id, app);
    }
}

BaseApp* app_mgr::app_create(int app_id) {
    BaseApp* new_app;

    switch (app_id) {
        case 0: new_app = new app_home_menu(&app_api); break;
        case 1: new_app = new app_main_clock(&app_api); break;
        case 2: new_app = new app_settings(&app_api); break;
        default: __breakpoint(); return open_apps.front(); // Should be home_menu
    }

    if (new_app != nullptr)
        open_apps.push_back(new_app);
    else {
        printf("new failed for app %d. Not enough memory?", app_id);
        return open_apps.front(); // Should be home_menu
    }

    return open_apps.back();
}

BaseApp* app_mgr::app_init(int app_id) {
    BaseApp* new_app;

    if (app_id > NUMBER_OF_APPS-1 or app_id < 0) {
        printf("Tried to init app %d", app_id);
        return app_init(0);
    }

    // Check if the app is already running.
    // Should this be done instead in app_mgr::app_switch() ?
    auto app_ptr = app_check_if_init(app_id);
    if (app_ptr)
        new_app = app_ptr;
     else
        new_app = app_create(app_id);
        
    return new_app;
}

void app_mgr::app_render(BaseApp* app) {
    app_act_on_return_value(app, app->render(&app_api));
}

void app_mgr::app_btnpressed(BaseApp* app, uint gpio, unsigned long delta) {
    app_act_on_return_value(app, app->btnpressed(&app_api, gpio, delta));
}

void app_mgr::app_destroy(BaseApp* to_erase) {
    auto erase_it = std::find(open_apps.begin(), open_apps.end(), to_erase); // "it" meaning iterator
    if (erase_it != open_apps.end()) {
        //assert(to_erase == erase_it);
        delete to_erase;
        open_apps.erase(erase_it);
    }
}

void app_mgr::app_all_bgrefresh() {
    std::vector<BaseApp*> to_erase;
    bool do_erase = false;

    for (auto app : open_apps) {
        bool is_foreground = app->app_get_attributes().id == g_s.foreground_app->app_get_attributes().id;

        switch (app->bgrefresh(&app_api, is_foreground)) {
            case BaseApp::AppReturnValues::OK:
                break;
            
            case BaseApp::AppReturnValues::QUIT:
                do_erase = true;
                to_erase.push_back(app);
                // No break here!
            
            case BaseApp::AppReturnValues::CLOSE:
                if (is_foreground)
                    new_foreground_app(open_apps.front());
                break;
            
            default:
                printf("Unidentified return value in bgrefresh for app %d at %x", app->app_get_attributes().id, app);
        }
    }

    if (do_erase) {
        for (auto app : to_erase) {
            app_destroy(app);
        }
    }
}

void app_mgr::app_switch_request(int to_appid) {
    if (!g_s.app_switch_requested)
        g_s.app_switch_to_app = to_appid;
    g_s.app_switch_requested = true;
}

void app_mgr::app_switch(BaseApp* app, int new_appid) {
    g_s.app_ready = false;

    if (app->app_get_attributes().destroy_on_exit)
        app_destroy(app);

    new_foreground_app(app_init(new_appid));    
    g_s.app_ready = true;
}
