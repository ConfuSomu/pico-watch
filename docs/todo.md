To-do
====

This document contains a list of tasks that have to be done in order to have a fun and fully featured watch. As this project is composed of two main sections, the hardware and software, the tasks have been divided in two categories. Some tasks are relevant to both spheres, so they have been included in both places.

Hardware
====

- [ ] Design a 3D printed enclosure.
- [ ] Design the circuit schematic.
  - A Shottkey diode will be necessary for better power efficiency. See [this](https://electronics.stackexchange.com/questions/548990/which-shottky-diode-do-i-need-for-redundant-power-to-a-raspberry-pi-pico) and section 4.5 "Powering Pico" of the "[Raspberry Pi Pico Datasheet](https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf)".
  - A lithium polymer battery coupled with a solar panel might be an interesting idea.These online resources have implementation ideas: [Raspberry Pi Forum](https://www.raspberrypi.org/forums/viewtopic.php?f=144&t=305398) and [Adafruit](https://www.adafruit.com/product/4755).
- [ ] Choose a suitable IR or NFC IC or daughter board.
  - Maybe the Pico can emulate NFC in software or via the PIO? To check. RF tuning won't be very easy, so an I2C/SPI module could help in adding the interface. 
  - See *Hardware ideas* section for more details; as they also concern software.
- [ ] Add buzzer support
  - This will require API methods and other software support to be usable by apps.
- [ ] Add RGB LED support
  - This is required to move the project to a Pimoroni Tiny 2040, as it has an RGB LED.
  - This will require API methods and other software support to be usable by apps.
- [ ] Choose the right battery type
  - [ ] Measure current consumption.

Software
====

Home menu app
----

- [ ] Open the clock app when pressing home button while being on home menu.

Clock app
----

- [ ] Clean up app.
- [ ] Add more features.
  - [ ] Mode to save current time and date with an option to add a note later. When saving data, a default name/note should be given (such as "A") instead of prompting for one to make saving a quick action. Have a list consisting of saved timestamps and the ability to modify each data note. This list should be exportable as a CSV encoded in a QR code.

Settings app
----

- [ ] Add way to reset display when there is a problem where display becomes "desynced" (a problem that's visible when the upper pixel line does not correspond with the top of the display, and seems to start at the middle) or flipped.
  - [ ] Provide screenshot of the problem.
  - I believe that it might be an I2C connection problem on my breadboard.

App manager (`AppMgr`)
----

- [ ] Have a per app CMakeLists.txt file.
- [ ] Create pre-build script to construct `app_mgr::app_create()` from text file or from `apps` directory.
  - [ ] Move `app_create` to another file to help.

Background refresh (`bg_refresh`)
----

- [ ] Run `cb_status_check` on second core. (This might not be easy because of possible race conditions.)
  - Send messages, such as button presses, via queues.

App API
-----

- [ ] Method that returns an std::list of apps that are currently installed. This will simplify implementation of home menu apps and reduce places where a list of apps has to be maintained.
- [ ] Make a list widget to aid in app UI development. See `todo/list_widget.md`.
  - [ ] Modify `ss_oled` to allow writing text at arbitrary coordinates.
  - [ ] Implement `BaseWidget` class, which has a paint virtual method.
  - [ ] Implement list widget.
- [ ] Ring widget
  - See Apple Watch "fitness rings" and Twitter character left indicator for inspiration.
- [ ] Method to disable automatic screen timeout.
  - Might be useful in multimedia applications, such as watching animations or reading.
- [ ] Pop-up for entering text.
  - Method will have an argument for limiting string length.
  - Returns pointer to `std::string`.
- [ ] QR generation. See *Hardware ideas*.
  - [This code](https://github.com/cyberman54/ESP32-Paxcounter/blob/e299444a894d38f70be872f489a07be0890345cc/src/display.cpp#L629) could give a good starting point.
- [ ] Notification support. Allow apps to display on-screen notifications. See shell applet for implementation.
- [ ] A shell applet--accessed by holding on the home button, à la Nintendo Switch--will allow users to view quick information about the system, such as battery level, time and notifications. The applet will be drawn over the current running, which will still stay in foreground, but won't receive any button presses.
  - If the applet can be compact enough, it will be possible to render the foreground app behind it and grey it out with the help of a checkered pattern. This could be a nice visual effect. Though, this will not help in terms of battery life.
  - The shell applet will be derived from `BaseApp` but its methods will be called a bit differently by the system.

BaseApp class specification
----

- [ ] Pass an enum representing a button instead of an arbitrary gpio number to `btn_pressed()` and `btn_released()`. This will require a rewrite of all `btn_...` app method implementations.
- [ ] `AppAttribute` that, when `true`, causes the render method to only be called on button press. This can save a lot of power as the app won't be rendered when there is no change; the CPU will have more time to sleep.
  - Instead of adding a new struct member, this could be enabled when `AppAttribute::render_interval = 0`.

Main function
----

- [ ] Change app render loop to not use `sleep_ms()`. It might help reduce power consumption and have a more consistent framerate in apps. On the other hoof, I don't see if it is possible to change the timer interval when creating a timer with `add_repeating_timer_ms()`; testing has to be done to see if modifying the `repeating_timer` struct causes any problems.
  - I don't know if I should implement this.

Debugging facilities
----

- [ ] Have a way to take screen captures.
  - I wrote the OLED buffer using the GDB command `dump binary value raw_screencap app_api.m_ucBuffer`. Then I converted the 1-bit depth image to a PNG with imagemagick: `convert -size 128x64 -depth 1 gray:raw_screencap test.png`. Unfortunately, this didn't give a useful result because of how the data is ordered when used by the display.
  - Either I should make a script that's run on the host computer to reorder the bits in the buffer dump to produce a desirable results, or I should write a function, run on the watch, that creates a valid array that can be dumped by GDB and immediately converted.
  - According to the current maintainer and developer of the *ss_oled* library (transcribed a little):
    - The internal buffer has its memory laid out in the same way as the physical display.
    - The display memory is 8 rows of 128 bytes.
    - Pixel 0,0 is bit 0 of byte 0, but bit 1 of byte 0 is pixel 0,1 (the next line)
    - Each row of 128 bytes represents pixels from 0,0 to 127,7
    - Each row has the byte oriented vertically with bit 0 as the top row and bit 7 as the bottom row.
    - Thus: you need to write some code to re-orient the bits if you're going to output the image as a "normal" bitmap which has the bytes oriented horizontally with bit 7 on the left and bit 0 on the right.

App ideas
----

- [ ] Basic watch functions
  - [ ] Clock app
  - [ ] Stopwatch
  - [ ] Timer
  - [ ] Alarm clock (needs additional app APIs)
- [ ] World clock to show time in multiple timezones.
- Fun!
  - [ ] Draw the Mandelbrot set.
    - See these links for inspiration, in order of usefulness:
      - https://medium.com/farouk-ounanes-home-on-the-internet/mandelbrot-set-in-c-from-scratch-c7ad6a1bf2d9
      - https://jonisalonen.com/2013/lets-draw-the-mandelbrot-set/
      - https://codereview.stackexchange.com/questions/124358/mandelbrot-image-generator-and-viewer
      - https://www.geeksforgeeks.org/fractals-in-cc/
      - https://mobile.codeguru.com/cpp/g-m/article.php/c19871/Mandelbrot-Using-C-AMP.htm
  - [ ] Text file reader. The files could either be linked in the binary or read from an SD card via SPI.
    - They could also be transferred into RAM via IR or NFC.
  - [ ] Random quote viewer
  - [ ] DVD screensaver effect, with the DVD logo.
    - [On updating the logo's position](https://github.com/foone/Win95Uptime/blob/5a58d134d5b4ec3a10736b6e608ebc9178521b03/source/Win95Uptime.cpp#L390)
- Games!
  - [ ] A TI-84 game called "Falldown". See branch `app-game_falldown` for a work in progress.
    - A [Basic version of the game](https://www.ticalc.org/archives/files/fileinfo/239/23954.html), with screenshots. A [screenshot](https://tiwizard-media.s3.amazonaws.com/2012/11/falldown.gif) of what I would like to implement, from another version of the game.
  - [ ] Snake
  - [ ] Doom-esque first-person shooter
    - Two buttons would be used to turn the camera to the left and right, a button would be used to go forward, and another button could serve to fire (use item) at ennemis. Holding the last button could open the game's menu.
    - Thus, the buttons would be: forward, left (camera), right (camera), fire (hold for menu).
    - Other control schemes could be envisaged.
    - This would require us to do 3D in software! I think that the hardware should be powerful enough. There are two cores available.
  - [ ] Flappy bird

Hardware ideas
----

- [ ] IR or NFC support for data exchange
  - This could be used to exchange GPG information, business cards (vCard), or maybe even 1-bit images and text to read on the watch.
  - NFC has the advantage of being now widely supported by smartphones. Mobile operating systems, such as Android and iOS, allow writing to NFC devices using specialized apps.
  - QR codes could also be used for one way data transfer. This method has the advantage of not requiring any additional hardware. Thought, the low screen resolution limit how much data can be displayed on a single QR code, so data might need to be divided between multiple QR codes. QR code generation and display should be implemented as an API method to facilitation adoption by apps.
  - Maybe the data matrix format has a bigger data density? It has different text modes that allow packing more text in less space, but it is less supported by smartphones and might require the installation of an app to be readable by a mobile phone.
