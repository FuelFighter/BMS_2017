/*
 * led_indicator.c
 *
 * Created: 04/04/17 19:01:16
 *  Author: Sondre
 */ 

#include "led_indicator.h"
#include "../UniversalModuleDrivers/rgbled.h"
#include "../fsm.h"
#include <stdbool.h>

static bool led_on = false;

void led_indicator_init() {
	rgbled_init();
}

void led_indicator_update() {
	if (led_on) {
		rgbled_turn_off(LED_ALL);
		led_on = false;
	} else {
		switch(fsm_get_current_state()) {
			case STATE_STARTUP: rgbled_turn_on(LED_BLUE | LED_GREEN); break;
			case STATE_IDLE: rgbled_turn_on(LED_BLUE); break;
			case STATE_PRECHARGING: rgbled_turn_on(LED_GREEN | LED_RED); break;
			case STATE_BATTERY_ACTIVE: rgbled_turn_on(LED_GREEN); break;
			case STATE_ERROR: rgbled_turn_on(LED_RED); break;
		}
		
		led_on = true;
	}
}