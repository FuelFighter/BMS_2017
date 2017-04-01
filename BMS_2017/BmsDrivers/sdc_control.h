/*
 * sdc_control.h
 *
 * Created: 26/03/17 13:44:39 
 *  Author: Sondre
 */ 


#ifndef SDC_CONTROL_H_
#define SDC_CONTROL_H_


#include <stdbool.h>


void sdc_init();

bool sdc_is_active();

void sdc_open_relays();

void sdc_open_precharge_relay();

void sdc_close_precharge_relay();

void sdc_open_positive_relay();

void sdc_close_positive_relay();


#endif /* SDC_CONTROL_H_ */