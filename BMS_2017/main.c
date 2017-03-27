/*
 * BMS_2017.c
 *
 * Created: 18/03/17 13:02:27
 * Author : Sondre
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "UniversalModuleDrivers/can.h"

int main(void)
{
	can_init();
	sei();
	
	CanMessage_t msg;
	msg.length = 8;
	msg.id = 0x440;

    while (1) {
	    msg.data[0] = 0xCA;
	    msg.data[1] = 0xFE;
	    msg.data[2] = 0xBA;
	    msg.data[3] = 0xBE;
	    msg.data[4] = 0x00;
	    msg.data[5] = 0xB0;
	    msg.data[6] = 0x0B;
	    msg.data[7] = 0x1E;

		can_send_message(&msg);

		_delay_ms(20);
    }
}

