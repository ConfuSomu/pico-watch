Pico Watch
=======

Pico Watch is a watch based on the Raspberry Pi Pico platform. The watch is composed of an 128x64 OLED display and five buttons. A buzzer and RGB LED are planned to be added later on to provide additional features, such as notifications from running apps and alarms.

The RP2040 used in the Pico has an official SDK that is extensively documented and has good community support for a new platform, since being released in January 2021.

Hardware
----

The final design is to be composed of Pimoroni's Tiny 2040, five buttons, an 128x64 SSD1306 OLED display and a piezo buzzer. The RGB LED is embedded in the Tiny 2040 board.

The five buttons function are up, down, select, mode and home. The home button is not accessible by the foreground application, except if it is the home menu app.

Power will be provided by either coin cell batteries (CR2032), AAA batteries or more obscure types. It depends on power consumption. If all fails, lithium polymer or lithium-ion batteries could be used.

Software
----

The software is made to be modular by adopting a concept where every user interface is composed of apps. These apps have access to an API that facilitates programming by providing varying layers of abstraction such as modal dialogue boxes. App management, including instantiation and freeing, is managed by custom code.[1] Thus, on start-up, the home menu app is loaded.

The current available apps are the home menu (`home_menu`), a clock (`main_clock`) and settings (`settings`).

See file `docs/apps.md` for more information, including on how to add apps.

User settings are stored in a global `g_s` struct. They currently include:

- The time format (24h or 12h)
- The OLED brightness
- The OLED screen timeout

The settings app allows the user to change these settings in addition to the current date and time.

Building
----

The project uses CMake, as recommended by the Raspberry Pi Pico C/C++ SDK documentation. Thus, the `CMakeLists.txt` file orchestrates the whole building process.

See section 2.1 "The Build System", page 8, of "[Pico C/C++ SDK](https://datasheets.raspberrypi.com/)" for instructions on how to build Pico projects.

Debugging
----

When working on the project, I use Microsoft VSCode for programming as it provides easily useable debugging features and code autocompletion. 

I use a GDB script that allows debugging STL containers. The script is available in `utility/stl_gdb_utils`. To use it, one has to copy the contents to their .gdbinit file, that's probably located at `~/.gdbinit`.

### Debugging tips

- Access tty: `minicom -b 115200 -o -D /dev/ttyS0`
- In GDB:
  - Use `pvector app_mgr::open_apps` to print current open apps. You need the GDB helper scripts.
  - Use `print malloc(500)` to debug memory allocation failures. You can malloc more than `PICO_HEAP_SIZE` as this only guaranties a minimum heap size, see [here](https://www.raspberrypi.org/forums/viewtopic.php?f=144&t=318030).
  - `dump binary value <filename> <symbol>` allows writing the contents of a variable to disk. The contents can later be written back into memory.

Libraries used
----

- [RP2040 ss_oled fork](https://github.com/bitbank2/Pi_Pico_C_Projects/tree/master/ss_oled) by bitbank ([Github](https://github.com/bitbank2), [Twitter](https://twitter.com/@fast_code_r_us))
  - Relevant [forum topic](https://forums.raspberrypi.com/viewtopic.php?f=144&t=299754).
  - I am using a modified version, based on commit `18ccaba` in the ss_oled git repository.
- [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk).
  - This project's code was last build against version (git tag) `1.0.1` of the SDK, released February 1 2021.

Inspiration
----

These projects were a good source of inspiration:
- [Open Smartwatch](https://github.com/Open-Smartwatch/open-smartwatch-os)
- [Sensor Watch](https://twitter.com/josecastillo/status/1476336711416397824) ([Crowd Supply campaign](https://www.crowdsupply.com/oddly-specific-objects/sensor-watch)). This is a replacement PCB for the Casio F-91W. It uses an Atmel ARM Cortex M0+ microcontroller.

----

[1]: I might have accidentally created a custom microkernel(?) because of the features that manage running apps. I won't use this term and instead prefer to use the word "system" to designate the code that's not in the `apps` subdirectory.
