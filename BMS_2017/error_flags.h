/*
 * error_flags.h
 *
 * Created: 01/04/17 15:09:12
 *  Author: Sondre
 */ 


#ifndef ERROR_FLAGS_H_
#define ERROR_FLAGS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	ERROR_FLAG_PRECHARGE_TIMEOUT = 0,
	ERROR_FLAG_LTC6804_LOS = 1,
	ERROR_FLAG_CELL_OVERVOLTAGE = 2,
	ERROR_FLAG_CELL_UNDERVOLTAGE = 3,
	ERROR_FLAG_OVERCURRENT = 4,
	ERROR_FLAG_OVERTEMPERATURE = 5
} error_flag_t;

void error_flags_set (error_flag_t flag);

bool error_flags_is_critical();

uint16_t error_flags_get_bitfield();

#endif /* ERROR_FLAGS_H_ */