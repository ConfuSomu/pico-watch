# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-watch C CXX)

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
#add_executable(pico-watch pico-watch.c )
add_executable(pico-watch
	pico-watch.c
	)

pico_set_program_name(pico-watch "pico-watch")
pico_set_program_version(pico-watch "0.1")
pico_enable_stdio_uart(pico-watch 1)

# Add the standard library to the build
target_link_libraries(pico-watch pico_stdlib)

# Add any user requested libraries
target_link_libraries(pico-watch
        Oled
        hardware_rtc
#        hardware_clocks
        )

pico_add_extra_outputs(pico-watch)
