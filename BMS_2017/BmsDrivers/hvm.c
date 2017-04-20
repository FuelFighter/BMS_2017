/*
 * hvm.c
 *
 * Created: 01/04/17 15:52:27
 *  Author: Sondre
 */ 

#include "hvm.h"
#include "../UniversalModuleDrivers/adc.h"


#define HVM_ADC_CHANNEL CH_ADC2

static uint16_t latest_value = 0;

static uint16_t hvm_convert_voltage(uint16_t val) {
	return 52726UL * val / 1000;
}

void hvm_init() {
	adc_init();
}

void hvm_measure() {
	uint16_t adc_value = adc_read(HVM_ADC_CHANNEL);
	latest_value = hvm_convert_voltage(adc_value);
}

uint16_t hvm_get_voltage() {
	return latest_value;
}