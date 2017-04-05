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
#include "UniversalModuleDrivers/usbdb.h"
#include "UniversalModuleDrivers/can.h"

#define MAIN_PERIOD_TIMER TIMER0
#define MAIN_PERIOD_MS 100


int main(void)
{
	hardware_init();

	uint8_t lost_battery_data = 0;

    while (1) {
		timer_start(MAIN_PERIOD_TIMER);

		bool ltc6804_com_ok = battery_write_configuration();
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

		led_indicator_update();
		
		CanMessage_t msg;
		for (int cg = 0; cg < 3; cg++) {
			msg.id = 0x500 + cg;
			msg.length = 8;
			
			for(int c = 0; c < 4; c++) {
				msg.data[2 * c + 0] = battery_last_data.cell_voltage[cg * 4 + c] >> 8;
				msg.data[2 * c + 1] = battery_last_data.cell_voltage[cg * 4 + c] & 0x00FF;
			}
			can_send_message(&msg);
		}

		msg.id = 0x510;
		msg.length = 8;
		for(int t = 0; t < 4; t++) {
			msg.data[2 * t + 0] = battery_last_data.temperature[t] >> 8;
			msg.data[2 * t + 1] = battery_last_data.temperature[t] & 0x00FF;
		}
		can_send_message(&msg);

		msg.id = 0x443;
		msg.length = 2;
		msg.data[0] = error_flags_get_bitfield() << 8;
		msg.data[1] = error_flags_get_bitfield()  & 0x00FF;
		can_send_message(&msg);

		msg.id = 0x442;
		msg.length = 8;
		msg.data[0] = battery_last_data.min_cell_voltage >> 8;
		msg.data[1] = battery_last_data.min_cell_voltage & 0x00FF;
		msg.data[2] = battery_last_data.max_cell_voltage >> 8;
		msg.data[3] = battery_last_data.max_cell_voltage & 0x00FF;
		msg.data[4] = battery_last_data.avg_cell_voltage >> 8;
		msg.data[5] = battery_last_data.avg_cell_voltage & 0x00FF;
		msg.data[6] = battery_last_data.total_voltage >> 8;
		msg.data[7] = battery_last_data.total_voltage & 0x00FF;
		can_send_message(&msg);

		msg.id = 0x440;
		msg.length = 1;
		msg.data[0] = lost_battery_data;
		can_send_message(&msg);

		while(timer_elapsed_ms(MAIN_PERIOD_TIMER) < MAIN_PERIOD_MS) { };
    }
}

