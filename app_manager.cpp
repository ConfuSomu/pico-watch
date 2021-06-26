#include "app_manager.hpp"
#include "api.hpp"
#include "apps/main_clock.hpp"
#include "apps/home_menu.hpp"
#include "apps/settings/main.hpp"

#define NUMBER_OF_APPS 3
// From pico-watch.c:
extern Api app_api;

std::vector<BaseApp*> open_apps;
int APPS_DESTROY_ON_EXIT[NUMBER_OF_APPS] = {0, 1, 1};
int APPS_IS_INIT[NUMBER_OF_APPS] = {0}; // Only run in background if init

// Called by app_init to create the app object.
int app_create(int app_id) {
    switch (app_id) {
        case 0: open_apps.push_back(new app_home_menu(&app_api)); break;
        default: __breakpoint();
    }
}

// Init a new app, that is not running.
int app_init(int app_id) {
    app_api.display_fill(0,1); // Clear OLED
    app_api.performance_render_interval_set(500); // Reset interval

    if (app_id > NUMBER_OF_APPS-1 or app_id < 0) {
        printf("Tried to init app %d", app_id);
        return app_init(0);
    }

    if (!APPS_IS_INIT[app_id]) {
        app_create(app_id);
        /* int status = (*APPS_FUNC_INIT[app_id])(&app_api);

        switch (status) {
            case Api::app_init_return_status::MALLOC_FAILED:
                printf("App init failed (alloc), ");
                for (int i=0; i<10; i++) {
                    if ((*APPS_FUNC_INIT[app_id])(&app_api) != Api::app_init_return_status::MALLOC_FAILED) {
                        printf("worked after %d tries\n", i);
                        APPS_IS_INIT[app_id] = 1;
                        return app_id;
                    }
                }
                // Instead, the current app could continue running: return current_app
                printf("gave up, launching app 0\n");
                return app_init(0); // Note: this has to work (and should)
            
            default: // OK and unhandled status codes
                printf("App init, status: %d\n", status);
                break;
        } */
        APPS_IS_INIT[app_id] = 1;
    }
    return app_id;
}

// Allow the running app, referenced by app_id, to invoke its render routine.
int app_render(int app_id) {
    for (auto app : open_apps) {
        if (app_id == app->app_id)
            return app->render(&app_api);
    }
}

// Delta is in ms, from time_since_button_press()
int app_btnpressed(int app_id, uint gpio, unsigned long delta) {
    for (auto app : open_apps) {
        if (app_id == app->app_id)
            return app->btnpressed(&app_api, gpio, delta);
    }
}

// Quit the app referenced by the app_id.
int app_destroy(int app_id) {
    BaseApp* to_destroy;

    // FIXME: Does not work if the app is not open. An invalid item tries to get freed.
    int to_delete_id = 0;
    for (auto app : open_apps) {
        if (app_id == app->app_id) {
            to_destroy = app;
            break;
        }
        to_delete_id++;
    }
    delete to_destroy;
    open_apps.erase(open_apps.begin() + to_delete_id);

    APPS_IS_INIT[app_id] = 0;
    return 0;
}

// Requests the current app to be replaced by an other one. The replacement will be done at the right moment.
void app_switch_request(int to_appid) {
    if (!g_s.app_switch_requested)
        g_s.app_switch_to_app = to_appid;
    g_s.app_switch_requested = true;
    app_api.performance_render_interval_set(0); // This will be reset on new app init
}

void app_switch(int old_appid, int new_appid) {
    g_s.app_ready = false;
    if (APPS_DESTROY_ON_EXIT[old_appid])
        app_destroy(old_appid);
    g_s.current_app = app_init(new_appid);
    g_s.app_ready = true;
}
