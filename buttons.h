#ifndef __BUTTONS_H__
#define __BUTTONS_H__
#include <stdio.h>

// Init buttons used in conjuction with interrupts
// All buttons are connected to ground.
#define NUMBER_OF_BUTTONS 5
#define BUTTON_HOME 22
#define BUTTON_SELECT 18
#define BUTTON_MODE 19
#define BUTTON_DOWN 20
#define BUTTON_UP 21
unsigned long button_time;
const int button_delayTime;

void init_buttons();
void gpio_interrupt_cb(uint gpio, uint32_t events);

#endif