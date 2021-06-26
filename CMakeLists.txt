cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-watch C CXX)

# PICO_USE_STACK_GUARDS=1 for testing, might leave as is.
add_compile_definitions(PICO_DEBUG_MALLOC PICO_DEBUG_MALLOC_LOW_WATER=0 PICO_USE_STACK_GUARDS=1)# PICO_MALLOC_PANIC)

# Increase heap size, this should leave:
# sram_size = SRAM_END(=0x20042000) - SRAM_BASE(=0x20000000) = 0x42000
# stack_size = 0x800 (default)
# heap_size = 0x5000 (changed)
# mem_left = sram_size - (stack_size + heap_size) = 0x3c800
# We still have to factor in global variables, so this is not precise. The best place for information seems to be the pico-wath.elf.map file.

# Small size used for testing, seems to work fine.
add_compile_definitions(PICO_HEAP_SIZE=0x200)

# Initialise the Pico SDK
pico_sdk_init()

# Add OLED library
add_library(Oled
        oled/BitBang_I2C.c
                oled/BitBang_I2C.h
	oled/ss_oled.c
                oled/ss_oled.h
        )
target_link_libraries(Oled pico_stdlib hardware_i2c)

# Main code
add_executable(pico-watch
	pico-watch.cpp
        app_manager.cpp
        init.cpp
        init.hpp
        buttons.cpp
        buttons.hpp
        api.cpp
        api.hpp
        apps/home_menu.cpp
        apps/home_menu.hpp
        apps/main_clock.cpp
        apps/main_clock.hpp
        apps/settings/main.cpp
        apps/settings/main.hpp
	)

pico_set_program_name(pico-watch "pico-watch")
pico_set_program_version(pico-watch "0.1")
# To access uart on a Pico connected directly to a Raspberry Pi (4 B): `minicom -b 115200 -o -D /dev/ttyS0`
pico_enable_stdio_uart(pico-watch 1)

# Add the standard library to the build
target_link_libraries(pico-watch pico_stdlib)

# Add any user requested libraries
target_link_libraries(pico-watch
        Oled
        hardware_rtc
        hardware_sync # For use of __wfi()
        )

pico_add_extra_outputs(pico-watch)
