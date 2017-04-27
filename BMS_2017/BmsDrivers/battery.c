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
static bool has_cell_data = false;
static bool has_aux_data = false;

static void battery_update_cell_metadata() {
	battery_last_data.total_voltage = 0;
	battery_last_data.min_cell_voltage = UINT16_MAX;
	battery_last_data.max_cell_voltage = 0;

	uint32_t avg_accumulator = 0;

	for (int c = 0; c < NUMBER_OF_CELLS; c++) {
		// Divide by 10 to convert from 100 �V to 1 mV
		battery_last_data.total_voltage += battery_last_data.cell_voltage[c] / 10;
		avg_accumulator += battery_last_data.cell_voltage[c];

		if (battery_last_data.cell_voltage[c] > battery_last_data.max_cell_voltage) {
			battery_last_data.max_cell_voltage = battery_last_data.cell_voltage[c];
		}
		
		if (battery_last_data.cell_voltage[c] < battery_last_data.min_cell_voltage) {
			battery_last_data.min_cell_voltage = battery_last_data.cell_voltage[c];
		}
	}

	battery_last_data.avg_cell_voltage = avg_accumulator / NUMBER_OF_CELLS;
}

static void battery_update_temperature_metadata() {
	battery_last_data.min_temperature = UINT16_MAX;
	battery_last_data.max_temperature = 0;
	int16_t acc = 0;

	for (int t = 0; t < NUMBER_OF_THERMISTORS; t++) {
		acc += battery_last_data.temperature[t];
		
		if (battery_last_data.temperature[t] > battery_last_data.max_temperature) {
			battery_last_data.max_temperature = battery_last_data.temperature[t];
		}
		
		if (battery_last_data.temperature[t] < battery_last_data.min_temperature) {
			battery_last_data.min_temperature = battery_last_data.temperature[t];
		}
	}

	battery_last_data.avg_temperature = acc / NUMBER_OF_THERMISTORS;
}

static int16_t battery_convert_temperature(uint16_t val) {
	// Uses a third degree polynomial fitted to the theoretical curve.
	// Calculation is carefully set up to avoid loss of precision and integer overflow.
	// This is accurate to 0.1 degrees between 0 and 80 degrees Celsius.
	int32_t x = (int32_t)val;
	return (int16_t)((x * ((x * (((x * -1092) / 1000) + 53300)) / 100000 - 12000)) / 100000 + 1219);
}

static bool current_measurement_bias_is_set = false;
static int16_t current_measurement_bias;

static int16_t battery_convert_current(uint16_t val) {
	int16_t converted_value = (-25000L + val) / 40;
	
	if (!current_measurement_bias_is_set) {
		current_measurement_bias = converted_value;
		current_measurement_bias_is_set = true;
	}

	return converted_value - current_measurement_bias;
}

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
	
	battery_update_cell_metadata();

	has_cell_data = true;

	return true;
}

bool battery_measure_temperature_and_current() {
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
		battery_last_data.temperature[0] = battery_convert_temperature(received_data.auxA.gpioVoltage1);
		battery_last_data.temperature[1] = battery_convert_temperature(received_data.auxA.gpioVoltage2);
		battery_last_data.temperature[2] = battery_convert_temperature(received_data.auxA.gpioVoltage3);
	}

	error = ltc_readRegisterGroup(RegisterGroup_AUXB, &received_data);
	if (error != NO_ERROR) {
		lost_registers++;
	} else {
		battery_last_data.temperature[3] = battery_convert_temperature(received_data.auxB.gpioVoltage4);
		battery_last_data.current = battery_convert_current(received_data.auxB.gpioVoltage5);
	}

	if (lost_registers > 0)
	return false;

	battery_update_temperature_metadata();

	has_aux_data = true;

	return true;
}

bool battery_write_configuration() {
	ltc_ErrorCode_t error;

	ltc_RegisterData_t config_data;
	config_data.config.dischargeTimeout = DischargeTimeout_DISABLED;
	config_data.config.dischargeEnable_bf = 0;
	config_data.config.gpioValu = 0x1F;
	config_data.config.refon = 1;
	config_data.config.swtrd = 1;
	error = ltc_writeConfigurationRegister(&config_data);
	if (error != NO_ERROR) {
		return false;
	}

	return true;
}

bool battery_has_data() {
	return has_aux_data && has_cell_data;
}