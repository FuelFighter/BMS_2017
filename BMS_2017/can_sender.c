/*
 * can_sender.c
 *
 * Created: 06/04/17 17:48:32
 *  Author: Sondre
 */ 
 
#include "can_sender.h"

#include "fsm.h"
#include "error_flags.h"
#include "BmsDrivers/hvm.h"
#include "BmsDrivers/battery.h"
#include "UniversalModuleDrivers/can.h"

#include <avr/io.h>

void can_sender_send_status_messages() {
	CanMessage_t msg;
	
	// State
	msg.id = CANID_BMS_STATE;
	msg.length = 1;
	msg.data.u8[0] = (uint8_t)fsm_get_current_state();
	can_send_message(&msg);

	// Error flags
	msg.id = CANID_BMS_ERROR_FLAGS;
	msg.length = 2;
	msg.data.u16[0] = error_flags_get_bitfield();
	can_send_message(&msg);
}


void can_sender_send_data_messages() {
	CanMessage_t msg;

	// Cell voltages
	for (int g = 0; g < 3; g++) {
		msg.id = CANID_BMS_CELLS(g);
		msg.length = 8;
		for (int c = 0; c < 4; c++) {
			msg.data.u16[c] = battery_last_data.cell_voltage[4 * g + c];
		}
		can_send_message(&msg);
	}

	// Temperatures
	msg.id = CANID_BMS_TEMPS;
	msg.length = 8;
	for (int t = 0; t < 4; t++) {
		msg.data.u16[t] = battery_last_data.temperature[t];
	}
	can_send_message(&msg);
	
	// Current and output voltage
	msg.id = CANID_BMS_CURRENT_VOLTAGE;
	msg.length = 4;
	msg.data.i16[0] = battery_last_data.current;
	msg.data.u16[1] = hvm_get_voltage();
	can_send_message(&msg);
}

void can_sender_send_debug(uint8_t value) {
	CanMessage_t msg;

	msg.id = CANID_BMS_DEBUG;
	msg.length = 1;
	msg.data.u8[0] = value;
	can_send_message(&msg);
}

void can_sender_send_debug_16(uint16_t value) {
	CanMessage_t msg;

	msg.id = CANID_BMS_DEBUG;
	msg.length = 2;
	msg.data.u16[0] = value;

	can_send_message(&msg);
}