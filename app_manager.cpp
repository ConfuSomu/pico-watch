#include <algorithm>

#include "app_manager.hpp"
#include "api.hpp"
#include "globals.hpp"
#include "apps/main_clock/main.hpp"
#include "apps/home_menu/main.hpp"
#include "apps/settings/main.hpp"

#define NUMBER_OF_APPS 3
// From pico-watch.c:
extern Api app_api;

std::vector<BaseApp*> open_apps;
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {0, 1, 1};

// Check if the specified app (via app_id) is already running.
// \return If app is init, pointer to app, else nullptr (more or less 0).
BaseApp* app_check_if_init(int app_id) {
    for (auto app : open_apps) {
        if (app_id == app->app_get_attributes().id)
            return app;
    }
    return nullptr;
}

// Called by app_init to create the app object.
BaseApp* app_create(int app_id) {
    switch (app_id) {
        case 0: open_apps.push_back(new app_home_menu(&app_api)); break;
        case 1: open_apps.push_back(new app_main_clock(&app_api)); break;
        default: __breakpoint(); return open_apps.front(); // Should be home_menu
    }
    // TODO: Check when new fails

    return open_apps.back();
}

// Init a new app, that is not running.
BaseApp* app_init(int app_id) {
    BaseApp* new_app;
    app_api.display_fill(0,1); // Clear OLED
    app_api.performance_render_interval_set(500); // Reset interval

    if (app_id > NUMBER_OF_APPS-1 or app_id < 0) {
        printf("Tried to init app %d", app_id);
        return app_init(0);
    }

    auto app_ptr = app_check_if_init(app_id);
    if (app_ptr)
        new_app = app_ptr;
     else
        new_app = app_create(app_id);
        
    return new_app;
}

// Allow the running app, referenced by app_id, to invoke its render routine.
int app_render(BaseApp* app) {
    return app->render(&app_api);
}

// Delta is in ms, from time_since_button_press()
int app_btnpressed(BaseApp* app, uint gpio, unsigned long delta) {
    return app->btnpressed(&app_api, gpio, delta);
}

// Quit the app referenced by the app_id.
int app_destroy(BaseApp* to_erase) {
    auto erase_it = std::find(open_apps.begin(), open_apps.end(), to_erase); // "it" meaning iterator
    if (erase_it != open_apps.end()) {
        //assert(to_erase == erase_it);
        delete to_erase;
        open_apps.erase(erase_it);
    }

    return 0;
}

// Refresh each app
void app_all_bgrefresh() {
    for (auto app : open_apps) {
        app->bgrefresh(&app_api, app->app_get_attributes().id == g_s.current_app->app_get_attributes().id);
    }
}

// Requests the current app to be replaced by an other one. The replacement will be done at the right moment.
void app_switch_request(int to_appid) {
    if (!g_s.app_switch_requested)
        g_s.app_switch_to_app = to_appid;
    g_s.app_switch_requested = true;
    app_api.performance_render_interval_set(0); // This will be reset on new app init
}

void app_switch(BaseApp* app, int new_appid) {
    g_s.app_ready = false;
    if (app->app_get_attributes().destroy_on_exit)
        app_destroy(app);

    auto app_ptr = app_check_if_init(new_appid);
    if (app_ptr)
        g_s.current_app = app_ptr;
    else
        g_s.current_app = app_init(new_appid);
    
    g_s.app_ready = true;
}
