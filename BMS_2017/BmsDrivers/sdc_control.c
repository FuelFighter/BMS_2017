/*
 * sdc_control.c
 *
 * Created: 01/04/17 14:24:13
 *  Author: Sondre
 */ 

#include "sdc_control.h"
#include <avr/io.h>

#define RELAY_CTRL_PORT PORTE
#define RELAY_CTRL_PIN_REG PINE
#define RELAY_CTRL_DDR DDRE
#define RELAY_POSITIVE_PIN 3
#define RELAY_PRECHARGE_PIN 4
#define RELAY_SDC_PIN 5

#define SDC_ACTIVE_PIN_REG PINB
#define SDC_ACTIVE_DDR DDRB
#define SDC_ACTIVE_PIN 4

void sdc_init() {
	// Set relay control pins to output, and turn on only the sdc relay.
	RELAY_CTRL_DDR |= (1 << RELAY_POSITIVE_PIN) | (1 << RELAY_PRECHARGE_PIN) | (1 << RELAY_SDC_PIN);
	RELAY_CTRL_PORT = 0;
	RELAY_CTRL_PORT |= (1 << RELAY_SDC_PIN);

	// Set sdc_active pin to input
	SDC_ACTIVE_DDR &= ~(1 << SDC_ACTIVE_PIN);
}

bool sdc_is_active() {
	if (SDC_ACTIVE_PIN_REG & (1 << SDC_ACTIVE_PIN)) {
		return false;
	} else {
		return true;
	}
}

void sdc_open_relays() {
	//while (RELAY_CTRL_PIN_REG != 0) {
	RELAY_CTRL_PORT = 0;
	//}
}

void sdc_open_precharge_relay() {
	RELAY_CTRL_PORT &= ~(1 << RELAY_PRECHARGE_PIN);
}

void sdc_close_precharge_relay() {
	RELAY_CTRL_PORT |= (1 << RELAY_PRECHARGE_PIN);
}


void sdc_open_positive_relay() {
	RELAY_CTRL_PORT &= ~(1 << RELAY_POSITIVE_PIN);
}


void sdc_close_positive_relay() {
	RELAY_CTRL_PORT |= (1 << RELAY_POSITIVE_PIN);
}
