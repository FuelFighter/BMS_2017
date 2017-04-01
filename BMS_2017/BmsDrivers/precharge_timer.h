/*
 * precharge_timer.h
 *
 * Created: 01/04/17 15:46:57
 *  Author: Sondre
 */ 


#ifndef PRECHARGE_TIMER_H_
#define PRECHARGE_TIMER_H_

#include <stdint.h>

void precharge_timer_init();

void precharge_timer_start();

uint16_t precharge_timer_elapsed_ms();


#endif /* PRECHARGE_TIMER_H_ */