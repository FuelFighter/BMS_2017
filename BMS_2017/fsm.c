/*
 * fsm.c
 *
 * Created: 28/03/17 20:10:35 
 *  Author: Sondre
 */ 

#include "fsm.h"


typedef enum {
	STATE_IDLE,
	STATE_PRECHARGING,
	STATE_BATTERY_ACTIVE,
	STATE_ERROR
} fsm_state_t;

static fsm_state_t fsm_state;

static void set_state(fsm_state_t new_state) {
	if (new_state == STATE_ERROR) {
		rgbled_turn_off(LED_ALL);
		sdc_open_relays();
		rgbled_turn_on(LED_RED);
	}

	if (new_state == STATE_IDLE) {
		rgbled_turn_off(LED_ALL);
		sdc_close_precharge_relay();
		sdc_open_positive_relay();
		rgbled_turn_on(LED_BLUE);
	}

	if (new_state == STATE_PRECHARGING) {
		rgbled_turn_off(LED_ALL);
		precharge_timer_start();
		rgbled_turn_on(LED_RED | LED_GREEN);
	}

	if (new_state == STATE_BATTERY_ACTIVE) {
		rgbled_turn_off(LED_ALL);
		sdc_close_relays();
		sdc_open_precharge_relay();
		rgbled_turn_on(LED_GREEN);
	}

	fsm_state = new_state;
}

void fsm_update() {
	if (error_flags_is_critical()) {
		set_state(STATE_ERROR);
	}
	
	switch (fsm_state) {
		case STATE_IDLE: {
			if (!sdc_is_active()) {
				set_state(STATE_PRECHARGING);
			}
			break;
		}
		case STATE_PRECHARGING: {
			if (precharge_timer_elapsed_ms() > PRECHARGE_MINIMUM_TIME_MS && 
				data_get_output_voltage() > data_get_battery_voltage() * PRECHARGE_MINIMUM_RATIO) {
				set_state(STATE_BATTERY_ACTIVE);
			} else if (precharge_timer_elapsed_ms() > PRECHARGE_MAXIMUM_TIME_MS) {
				error_flags_set(ERROR_FLAG_PRECHARGE_TIMEOUT);
				set_state(STATE_ERROR);
			}
			break;
		}
		case STATE_BATTERY_ACTIVE: {
			if (sdc_is_active()) {
				set_state(STATE_IDLE);
			}
			break;
		}
		case STATE_ERROR: {
			sdc_open_relays();
			rgbled_turn_off(LED_GREEN | LED_BLUE);
			rgbled_turn_on(LED_RED);
			break;
		}
	}
}