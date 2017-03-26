/*
 * ltc6804_pec.h
 *
 * Created: 31.03.2016 12.53.12
 *  Author: Sondre
 */ 


#ifndef LTC6804_PEC_H_
#define LTC6804_PEC_H_

#include <stdint.h>

uint16_t pec_calculate (uint8_t* data , uint8_t len);


#endif /* LTC6804_PEC_H_ */