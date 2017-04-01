/*
 * battery.h
 *
 * Created: 26/03/17 13:43:42 
 *  Author: Sondre
 */ 


#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_CELLS 12
#define NUMBER_OF_THERMISTORS 4

typedef struct {
	// Unit: 100 µV (100 microvolt)
	uint16_t cell_voltage[NUMBER_OF_CELLS];
	uint16_t max_cell_voltage;
	uint16_t min_cell_voltage;
	uint16_t avg_cell_voltage;

	// Unit: mV (1 millivolt)
	uint16_t total_voltage;

	// Unit: mA (1 milliampere)
	uint16_t current;

	// Unit: d°C (1 decidegree celsius)
	uint16_t temperature[NUMBER_OF_THERMISTORS];
	uint16_t max_temperature;
	uint16_t min_temperature;
	uint16_t avg_temperature;
} battery_t;

extern battery_t battery_last_data;

bool battery_measure_cell_voltages();

bool battery_measure_temperature_and_current();

#endif /* BATTERY_H_ */