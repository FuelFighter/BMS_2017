/*
 * fsm.h
 *
 * Created: 28/03/17 20:10:43 
 *  Author: Sondre
 */ 


#ifndef FSM_H_
#define FSM_H_


typedef enum {
	STATE_STARTUP = 4,
	STATE_IDLE = 0,
	STATE_PRECHARGING = 1,
	STATE_BATTERY_ACTIVE = 2,
	STATE_ERROR = 3
} fsm_state_t;

void fsm_init();

void fsm_update();

fsm_state_t fsm_get_current_state();


#endif /* FSM_H_ */