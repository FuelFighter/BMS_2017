/*
 * fsm.h
 *
 * Created: 28/03/17 20:10:43 
 *  Author: Sondre
 */ 


#ifndef FSM_H_
#define FSM_H_


typedef enum {
	STATE_IDLE,
	STATE_PRECHARGING,
	STATE_BATTERY_ACTIVE,
	STATE_ERROR
} fsm_state_t;


void fsm_update();

fsm_state_t fsm_get_current_state();


#endif /* FSM_H_ */