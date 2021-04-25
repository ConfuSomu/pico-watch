#include <stdio.h>
#include "pico/stdlib.h"

#include "buttons.hpp"
#include "api.hpp"
// From pico-watch.c:
extern int app_btnpressed(int app_id, uint gpio);
extern void app_switch_request(int);
extern Api app_api;

//const uint BUTTON_PINS[] = {BUTTON_HOME, BUTTON_SELECT, BUTTON_MODE, BUTTON_UP, BUTTON_DOWN};

void gpio_interrupt_cb(uint gpio, uint32_t events) {
    if (button_time_since_press() > g_s.button_delay_time) {

        if (app_api.m_interpret_button_press) {
            if (gpio == BUTTON_HOME && (g_s.current_app != 0)) // Home app
                app_switch_request(0);
            else
                app_btnpressed(g_s.current_app, gpio);
        }

        app_api.button_last_set(gpio);
        g_s.button_last_pressed_time = to_ms_since_boot(get_absolute_time());
    }
}

unsigned long time_since_button_press() {
    // FIXME: This does not correct overflows
    return to_ms_since_boot(get_absolute_time())-g_s.button_last_pressed_time;
}

void init_buttons() {
    gpio_init(BUTTON_HOME);
    gpio_pull_up(BUTTON_HOME);
    gpio_set_irq_enabled_with_callback(BUTTON_HOME, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);
    gpio_init(BUTTON_SELECT);
    gpio_pull_up(BUTTON_SELECT);
    gpio_set_irq_enabled_with_callback(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);
    gpio_init(BUTTON_MODE);
    gpio_pull_up(BUTTON_MODE);
    gpio_set_irq_enabled_with_callback(BUTTON_MODE, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);
    gpio_init(BUTTON_DOWN);
    gpio_pull_up(BUTTON_DOWN);
    gpio_set_irq_enabled_with_callback(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);
    gpio_init(BUTTON_UP);
    gpio_pull_up(BUTTON_UP);
    gpio_set_irq_enabled_with_callback(BUTTON_UP, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);

    /* For some reason for loop does not work… TODO
    for (int i=0; i < NUMBER_OF_BUTTONS; i++) {
        uint button = BUTTON_PINS[i];
        gpio_init(button);
        gpio_pull_up(button);
        gpio_set_irq_enabled_with_callback(button, GPIO_IRQ_EDGE_FALL , true, &gpio_interrupt_cb);
    }
    */
    g_s.button_last_pressed_time = to_ms_since_boot(get_absolute_time());
}
