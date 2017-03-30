/*
 * battery.h
 *
 * Created: 26/03/17 13:43:42 
 *  Author: Sondre
 */ 


#ifndef BATTERY_H_
#define BATTERY_H_

#define NUMBER_OF_CELLS 12
#define NUMBER_OF_THERMISTORS 4

typedef struct {
	// Unit: mV (millivolt)
	uint16_t cell_voltage[NUMBER_OF_CELLS];

	// Unit: mV (millivolt)
	uint16_t total_voltage;

	// Unit: mA (milliampere)
	uint16_t current;

	// d°C (desidegrees celsius)
	uint16_t temperatures[NUMBER_OF_THERMISTORS];
} battery_t;

extern battery_t battery_last_data;

bool battery_measure_cell_voltages();

bool battery_measure_temperature_and_current();

#endif /* BATTERY_H_ */