/*
 * error_flags.c
 *
 * Created: 01/04/17 15:09:29
 *  Author: Sondre
 */ 

#include "error_flags.h"

#include <stdint.h>

static uint16_t error_flags = 0;

void error_flags_set (error_flag_t flag) {
	error_flags |= (1 << flag);
}

bool error_flags_is_critical() {
	if (error_flags != 0) {
		return true;
	} else {
		return false;
	}
}

uint16_t error_flags_get_bitfield() {
	return error_flags;
}