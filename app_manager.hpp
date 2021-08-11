#pragma once
#include <vector>
#include "base_app.hpp"

// Interface to the app manager. These functions are accessible to other parts of the code, except open_apps, which is managed by the following functions.
namespace app_mgr {
    // List of pointers to currently running apps.
    extern std::vector<BaseApp*> open_apps;

    // Init a new app, that is not running.
    BaseApp* app_init(int app_id);

    // Allow the running app, referenced by app_id, to invoke its render routine.
    void app_render(BaseApp* app);

    // Delta is in ms, from time_since_button_press()
    void app_btnpressed(BaseApp* app, uint gpio, unsigned long delta);

    // This should only be called by pico-watch.cpp before app rendering, to chage the current app.
    void app_switch(BaseApp* app, int new_appid);

    // Requests the current app to be replaced by an other one. The replacement will be done at the right moment.
    void app_switch_request(int to_appid);

    // Refresh each app
    void app_all_bgrefresh();

    // Private functions following. I tried using anonymous namespaces but it was too complicated. I might come back to this later. Just don't use the following internal functions.

    // Check if the specified app (via app_id) is already running.
    // \return If app is init, pointer to app, else nullptr (more or less 0).
    BaseApp* app_check_if_init(int app_id);

    // Check the return value of the called application method and act on it.
    void app_act_on_return_value(BaseApp* app, BaseApp::AppReturnValues return_value);

    // Called by app_init to create the app object.
    BaseApp* app_create(int app_id);

    // Quit the app referenced by the app_id.
    void app_destroy(BaseApp* to_erase);
}
