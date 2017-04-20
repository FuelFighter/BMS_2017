/*
 * limits.h
 *
 * Created: 01/04/17 16:22:31
 *  Author: Sondre
 */ 


#ifndef LIMITS_H_
#define LIMITS_H_

#define LIMITS_CELL_VOLTAGE_MAX 42000 // Unit: 100 µV
#define LIMITS_CELL_VOLTAGE_MIN 31500 // Unit: 100 µV

#define LIMITS_TEMPERATURE_MAX 600 // Unit: 0.1 deg celcius

#define LIMITS_CURRENT_MAX 500 // Unit: 0.1 A

#define LIMITS_PRECHARGE_TIME_MAX 10000 // ms
#define LIMITS_PRECHARGE_TIME_MIN 3000 // ms
#define LIMITS_PRECHARGE_RATIO_MIN 0.9

#define LIMITS_MAX_LOST_BATTERY_DATA 0

#endif /* LIMITS_H_ */