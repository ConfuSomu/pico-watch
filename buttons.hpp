#pragma once
#include "init.hpp"

// Init buttons used in conjuction with interrupts
// All buttons are connected to ground.
#define NUMBER_OF_BUTTONS 5
#define BUTTON_NONE 0
#define BUTTON_HOME 22
#define BUTTON_SELECT 18
#define BUTTON_MODE 19
#define BUTTON_DOWN 20
#define BUTTON_UP 21

void init_buttons();
void gpio_interrupt_cb(uint gpio, uint32_t events);
unsigned long time_since_button_press();
