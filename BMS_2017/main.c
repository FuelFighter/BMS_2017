/*
 * BMS_2017.c
 *
 * Created: 18/03/17 13:02:27
 * Author : Sondre
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void)
{
    while (1) {
		battery_measure_cell_voltages();
		battery_measure_temperature_and_current();
		hvm_measure();
		
		fsm_update();
    }
}

