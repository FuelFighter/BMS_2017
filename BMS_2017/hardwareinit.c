/*
 * hardwareinit.c
 *
 * Created: 04/04/17 18:23:51
 *  Author: Sondre
 */ 

#include "hardwareinit.h"

#include <avr/interrupt.h>

#include "BmsDrivers/hvm.h"
#include "BmsDrivers/sdc_control.h"
#include "BmsDrivers/led_indicator.h"
#include "UniversalModuleDrivers/timer.h"
#include "UniversalModuleDrivers/spi.h"
#include "UniversalModuleDrivers/can.h"

void hardware_init() {
	
	timer_init();
	
	spi_init(DIV_16);

	can_init(0x000, 0x000); // Accept mask = 0x000 => accept all messages

	hvm_init();

	sdc_init();

	led_indicator_init();
	
	sei();
}