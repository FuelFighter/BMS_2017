/*
 * hardwareinit.c
 *
 * Created: 04/04/17 18:23:51
 *  Author: Sondre
 */ 

#include "hardwareinit.h"

#include "BmsDrivers/hvm.h"
#include "BmsDrivers/sdc_control.h"
#include "BmsDrivers/led_indicator.h"
#include "UniversalModuleDrivers/timer.h"
#include "UniversalModuleDrivers/spi.h"
#include "UniversalModuleDrivers/can.h"

void hardware_init() {
	
	timer_init();
	
	spi_init(DIV_8);

	can_init();

	hvm_init();

	sdc_init();

	led_indicator_init();
}