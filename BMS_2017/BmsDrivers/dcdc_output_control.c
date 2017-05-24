/*
 * dcdc_output_control.c
 *
 * Created: 25/05/17 01:23:30
 *  Author: Sondre
 */ 

#include "dcdc_output_control.h"

#include <avr/io.h>

#define RELAY_CTRL_DDR DDRB
#define RELAY_CTRL_PORT PORTB
#define RELAY_CTRL_PIN PB4

void dcdc_output_init() {
	// Init the relayy control pin, and disable the 12V output by default
	DDRB |= (1 << RELAY_CTRL_PIN);
	PORTB &= ~(1 << RELAY_CTRL_PIN);
}

void dcdc_output_enable() {
	PORTB |= (1 << RELAY_CTRL_PIN);
}

void dcdc_output_disable() {
	PORTB &= ~(1 << RELAY_CTRL_PIN);
}
