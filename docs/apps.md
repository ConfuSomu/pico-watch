Apps
====

Apps are classes that are derived from a virtual `base_app` class. Each of these derived classes have to define a set of methods to have a functional app. The mandatory methods are the app's render method (`render(Api*)`) and a method to retrieve the app's attributes (`app_get_attributes()`).


App attributes
----

Each app has attributes that define how the rest of the system must interface with it and how it is identified. The currently defined attributes are:

- `uint id`: a unique unsigned int that is used to uniquely identify the current app. See *Adding apps*.
- `bool destroy_on_exit`: set to `true` if the app should be closed when run; set to `false` if the app should continue to run when closed by pressing the home button. See *Background refresh*.
- `uint render_interval`: frequency in milliseconds at which the render method should be called. See *Render method*.

These attributes are stored in an `AppAttributes` struct. The app has to implement the virtual `app_get_attributes()` method to make sure that its (and not the virtual class') `AppAttribute` struct pointer is returned. More testing (and C++ standard reading) should be done to see if this is a real problem.

Thus, app attributes related elements can be implemented as:
```cpp
// in the app class definition:
private:
// ...
AppAttributes app_attributes = {1, true};
public:
    const AppAttributes& app_get_attributes() {
        return app_attributes;
    }
```

Render method
----

The render method has the duty of rendering the app on the display. This is where the UI and any other graphics will be shown to the user. A pointer to `app_api` is passed as argument to allow drawing graphics and text on-screen and access other system facilities.

This method should not take too much time as it is what dictates the app's frame rate. Offload setting application member variables to `btn_pressed`, for example, if a variable has to immediately change because of a user interaction, could be a wise choice.

The frequency at which this method is called is determined by `AppAttributes::render_interval` struct member. Please set the render frequency to the highest value that still provides a pretty smooth UI. The bigger the value, the more time the CPU can spend sleeping, thus saving more power and having a longer battery life.

Button pressed & released methods
----

The methods `btn_pressed()` and `btn_released()` allow an app to know which buttons have been pressed and then released by the user, respectively. These methods are called by the pin interrupt callback when a button is pressed and released. A gpio number is passed as argument with the time since *a* button has been last released or pressed, for each respective method. A pointer to `app_api` is also given as it might be helpful in offloading code from the render method and could help in having cleaner code.

The button that has been pressed can be found by comparing `uint gpio` with the valus of the `BUTTON_` defines included within `app/base_app.hpp`. Only the app which has a value of `app_id` equal to 0 can receive the home button press; for other apps, pressing on the home button closes it. See *Background refresh* for modifying the home button behaviour.

Please note that these two app methods may be called at any moment when the app is in foreground. It might be called when the render method is running or between render calls. Apps in background are guaranteed to never receive button presses.

Background refresh
----

Applications have a facility that allow them to update while in the background. When running in the background, they shouldn't draw to the screen as this will interfere with the foreground app.

To refresh in the background, an app must have set `AppAttributes::destroy_on_exit` to `false` to not be freed when closed (by the user or programmatically). Additionally, the `background_refresh()` method must be non-empty, else there's no use for running in the background. When running, an app can always close itself, to conserve system resources, by returning the right value. See *Return values*.

Adding apps
----

Currently to add an app, `app_manager.cpp` has to be edited, with the app constructor added to `app_mgr::app_create`, under an unused `app_id`. The app's header, containing its class derived from `base_app`, has to be included and the `NUMBER_OF_APPS` define incremented. Furthermore, `apps/home_menu/main.hpp` has to be edited to be able to access the app: the app's name has to be added to the `APPS_NAME` array, at the same index as its `app_id` and `#define NUMBER_OF_APPS` has to be updated to be able to fully navigate the list of apps.

In the future, this should be simplified by using a pre compile script to generate the relevant files.
