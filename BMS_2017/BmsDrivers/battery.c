/*
 * battery.c
 *
 * Created: 30/03/17 17:41:06
 *  Author: Sondre
 */ 

#include "battery.h"
#include "LTC6804/ltc6804.h"

#include <util/delay.h>


battery_t battery_last_data;


bool battery_measure_cell_voltages() {
	ltc_ErrorCode_t error;
	
	error = ltc_sendCommand(Command_ADCV);
	if (error != NO_ERROR) {
		return false;
	}

	_delay_us(LTC_T_CYCLE_US);

	ltc_RegisterGroup_t register_groups[] = {
		RegisterGroup_CELLA,
		RegisterGroup_CELLB,
		RegisterGroup_CELLC,
		RegisterGroup_CELLD
	};

	ltc_RegisterData_t received_data;
	int lost_registers = 0;
	
	for (int reg = 0; reg < 4; reg++) {
		error = ltc_readRegisterGroup(register_groups[reg], &received_data);
		if (error != NO_ERROR) {
			lost_registers++;
		} else {
			battery_last_data.cell_voltage[0 + 3 * reg] = received_data.cv.cellVoltage0_3_6_9;
			battery_last_data.cell_voltage[1 + 3 * reg] = received_data.cv.cellVoltage1_4_7_10;
			battery_last_data.cell_voltage[2 + 3 * reg] = received_data.cv.cellVoltage2_5_8_11;
		}
	}

	if (lost_registers > 0)
		return false;
	
	return true;
}

void battery_measure_temperature_and_current() {
	ltc_ErrorCode_t error;
	
	error = ltc_sendCommand(Command_ADAX);
	if (error != NO_ERROR) {
		return false;
	}

	_delay_us(LTC_T_CYCLE_US);

	int lost_registers = 0;
	ltc_RegisterData_t received_data;
	
	error = ltc_readRegisterGroup(RegisterGroup_AUXA, &received_data);
	if (error != NO_ERROR) {
		lost_registers++;
		} else {
		battery_last_data.temperatures[0] = received_data.auxA.gpioVoltage1;
		battery_last_data.temperatures[1] = received_data.auxA.gpioVoltage2;
		battery_last_data.temperatures[2] = received_data.auxA.gpioVoltage3;
	}

	error = ltc_readRegisterGroup(RegisterGroup_AUXB, &received_data);
	if (error != NO_ERROR) {
		lost_registers++;
	} else {
		battery_last_data.temperatures[3] = received_data.auxB.gpioVoltage4;
		battery_last_data.current = received_data.auxB.gpioVoltage5;
	}

	if (lost_registers > 0)
	return false;

	return true;
}
