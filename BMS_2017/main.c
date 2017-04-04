/*
 * BMS_2017.c
 *
 * Created: 18/03/17 13:02:27
 * Author : Sondre
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "fsm.h"
#include "limits.h"
#include "error_flags.h"
#include "hardwareinit.h"
#include "BmsDrivers/hvm.h"
#include "BmsDrivers/battery.h"
#include "BmsDrivers/sdc_control.h"
#include "BmsDrivers/led_indicator.h"
#include "UniversalModuleDrivers/timer.h"

#define MAIN_PERIOD_TIMER TIMER0
#define MAIN_PERIOD_MS 100


int main(void)
{
	hardware_init();

	sei();

    while (1) {
		timer_start(MAIN_PERIOD_TIMER);

		bool slave_com_ok = battery_measure_cell_voltages();
		
		if (slave_com_ok && battery_last_data.max_cell_voltage > LIMITS_CELL_VOLTAGE_MAX) {
			error_flags_set(ERROR_FLAG_CELL_OVERVOLTAGE);
			sdc_open_relays();
		}
		if (slave_com_ok && battery_last_data.min_cell_voltage < LIMITS_CELL_VOLTAGE_MIN) {
			error_flags_set(ERROR_FLAG_CELL_UNDERVOLTAGE);
			sdc_open_relays();
		}

		  = battery_measure_temperature_and_current();

		if (battery_last_data.max_temperature > LIMITS_TEMPERATURE_MAX) {
			error_flags_set(ERROR_FLAG_OVERTEMPERATURE);
			sdc_open_relays();
		}
		if ((battery_last_data.current > LIMITS_CURRENT_MAX) || (battery_last_data.current < -LIMITS_CURRENT_MAX)) {
			error_flags_set(ERROR_FLAG_OVERCURRENT);
			sdc_open_relays();
		}

		hvm_measure();
		
		fsm_update();

		led_indicator_update();

		while(timer_elapsed_ms(MAIN_PERIOD_TIMER) < MAIN_PERIOD_MS) { };
    }
}

