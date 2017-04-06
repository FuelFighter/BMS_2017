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
	msg.data[0] = (uint8_t)fsm_get_current_state();
	can_send_message(&msg);

	// Error flags
	msg.id = CANID_BMS_ERROR_FLAGS;
	msg.length = 2;
	msg.data[0] = (uint8_t)(error_flags_get_bitfield() >> 8);
	msg.data[1] = (uint8_t)(error_flags_get_bitfield() & 0x00FF);
	can_send_message(&msg);
}


void can_sender_send_data_messages() {
	CanMessage_t msg;

	// Cell voltages
	for (int g = 0; g < 3; g++) {
		msg.id = CANID_BMS_CELLS(g);
		msg.length = 8;
		for (int c = 0; c < 4; c++) {
			msg.data[2 * c + 0] = (uint8_t)(battery_last_data.cell_voltage[4 * g + c] >> 8);
			msg.data[2 * c + 1] = (uint8_t)(battery_last_data.cell_voltage[4 * g + c] & 0x00FF);
		}
		can_send_message(&msg);
	}

	// Temperatures
	msg.id = CANID_BMS_TEMPS;
	msg.length = 8;
	for (int t = 0; t < 4; t++) {
		msg.data[2 * t + 0] = (uint8_t)(battery_last_data.temperature[t] >> 8);
		msg.data[2 * t + 1] = (uint8_t)(battery_last_data.temperature[t] & 0x00FF);
	}
	can_send_message(&msg);
	
	// Current and output voltage
	msg.id = CANID_BMS_CURRENT_VOLTAGE;
	msg.length = 4;
	msg.data[0] = (uint8_t)(battery_last_data.current >> 8);
	msg.data[1] = (uint8_t)(battery_last_data.current & 0x00FF);
	msg.data[2] = (uint8_t)(hvm_get_voltage() >> 8);
	msg.data[3] = (uint8_t)(hvm_get_voltage() & 0x00FF);
	can_send_message(&msg);

	msg.id = 0x500;
	msg.length = 1;
	msg.data[0] = PINB;
	can_send_message(&msg);
}
