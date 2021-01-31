cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ss_oled_demo)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(ss_oled_demo
	ss_oled_demo.c
	BitBang_I2C.c
        BitBang_I2C.h
	ss_oled.c
        ss_oled.h
	)

#target_sources(ss_oled_demo PRIVATE
#	ss_oled_demo.c
#	BitBang_I2C.c
#        BitBang_I2C.h
#	ss_oled.c
#        ss_oled.h
#	)

target_link_libraries(ss_oled_demo pico_stdlib hardware_i2c)
#pico_enable_stdio_usb(ss_oled_demo 1)

pico_add_extra_outputs(ss_oled_demo)

