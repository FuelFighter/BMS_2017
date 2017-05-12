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
#include "can_sender.h"
#include "error_flags.h"
#include "hardwareinit.h"
#include "BmsDrivers/hvm.h"
#include "BmsDrivers/battery.h"
#include "BmsDrivers/sdc_control.h"
#include "BmsDrivers/led_indicator.h"
#include "UniversalModuleDrivers/timer.h"
#include "UniversalModuleDrivers/usbdb.h"

#define MAIN_PERIOD_TIMER TIMER0
#define MAIN_PERIOD_MS 100


int main(void)
{
	hardware_init();

	uint8_t lost_battery_data = 0;
	uint8_t no_data_counter = 0;

    while (1) {
		timer_start(MAIN_PERIOD_TIMER);

		bool ltc6804_com_ok = battery_write_configuration(0x000);
		if (!ltc6804_com_ok) lost_battery_data++;

		ltc6804_com_ok = battery_measure_cell_voltages();
		if (!ltc6804_com_ok) lost_battery_data++;

		if (ltc6804_com_ok && battery_last_data.max_cell_voltage > LIMITS_CELL_VOLTAGE_MAX) {
			error_flags_set(ERROR_FLAG_CELL_OVERVOLTAGE);
			sdc_open_relays();
		}
		if (ltc6804_com_ok && battery_last_data.min_cell_voltage < LIMITS_CELL_VOLTAGE_MIN) {
			error_flags_set(ERROR_FLAG_CELL_UNDERVOLTAGE);
			sdc_open_relays();
		}

		ltc6804_com_ok = battery_measure_temperature_and_current();
		if (!ltc6804_com_ok) lost_battery_data++;

		if (ltc6804_com_ok && battery_last_data.max_temperature > LIMITS_TEMPERATURE_MAX) {
			error_flags_set(ERROR_FLAG_OVERTEMPERATURE);
			sdc_open_relays();
		}
		if (ltc6804_com_ok && ((battery_last_data.current > LIMITS_CURRENT_MAX) || (battery_last_data.current < -LIMITS_CURRENT_MAX))) {
			error_flags_set(ERROR_FLAG_OVERCURRENT);
			sdc_open_relays();
		}

		if (lost_battery_data > LIMITS_MAX_LOST_BATTERY_DATA) {
			error_flags_set(ERROR_FLAG_LTC6804_LOS);
			sdc_open_relays();
		} else {
			lost_battery_data = 0;
		}

		hvm_measure();
		
		fsm_update();
		can_sender_send_status_messages();
		can_sender_send_data_messages();
		
		if (!battery_has_data()) {
			no_data_counter++;
			if (no_data_counter > LIMITS_MAX_STARTUP_NO_DATA) {				
				error_flags_set(ERROR_FLAG_NO_DATA_ON_STARTUP);
				sdc_open_relays();
			}
		}

		led_indicator_update();
		
		while(timer_elapsed_ms(MAIN_PERIOD_TIMER) < MAIN_PERIOD_MS) { };
    }
}

