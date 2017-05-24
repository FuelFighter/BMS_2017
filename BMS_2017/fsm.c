/*
 * fsm.c
 *
 * Created: 28/03/17 20:10:35 
 *  Author: Sondre
 */ 

#include "fsm.h"
#include "limits.h"
#include "error_flags.h"
#include "BmsDrivers/hvm.h"
#include "BmsDrivers/battery.h"
#include "BmsDrivers/sdc_control.h"
#include "BmsDrivers/dcdc_output_control.h"
#include "UniversalModuleDrivers/timer.h"


#define PRECHARGE_TIMER TIMER1

static fsm_state_t fsm_state;

static void set_state(fsm_state_t new_state) {
	if (new_state == STATE_IDLE) {
		dcdc_output_enable();
	}

	if (new_state == STATE_ERROR) {
		sdc_open_relays();
		dcdc_output_disable();
	}

	if (new_state == STATE_IDLE) {
		sdc_close_precharge_relay();
		sdc_open_positive_relay();
	}

	if (new_state == STATE_PRECHARGING) {
		timer_start(PRECHARGE_TIMER);
	}

	if (new_state == STATE_BATTERY_ACTIVE) {
		sdc_close_positive_relay();
		sdc_open_precharge_relay();
	}

	fsm_state = new_state;
}

void fsm_init() {
	set_state(STATE_STARTUP);
}

void fsm_update() {
	if (error_flags_is_critical()) {
		set_state(STATE_ERROR);
	}

	switch (fsm_state) {
		case STATE_STARTUP:
			if (battery_has_data()) {
				set_state(STATE_IDLE);
			}
		
		case STATE_IDLE: {
			if (!sdc_is_active()) {
				set_state(STATE_PRECHARGING);
			}
			break;
		}
		case STATE_PRECHARGING: {
			if (timer_elapsed_ms(PRECHARGE_TIMER) > LIMITS_PRECHARGE_TIME_MIN && 
				hvm_get_voltage() * 100 > battery_last_data.total_voltage * LIMITS_PRECHARGE_RATIO_MIN) {
				set_state(STATE_BATTERY_ACTIVE);
			} else if (timer_elapsed_ms(PRECHARGE_TIMER) > LIMITS_PRECHARGE_TIME_MAX) {
				error_flags_set(ERROR_FLAG_PRECHARGE_TIMEOUT);
				set_state(STATE_ERROR);
			} else if (sdc_is_active()) {
				set_state(STATE_IDLE);
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
			break;
		}
	}
}

fsm_state_t fsm_get_current_state() {
	return fsm_state;
}