/*
 * can_sender.h
 *
 * Created: 06/04/17 17:47:41
 *  Author: Sondre
 */ 


#ifndef CAN_SENDER_H_
#define CAN_SENDER_H_

#include <stdint.h>


#define CANID_FCN_DATA 0x400

#define CANID_GRP_BMS 0x040

#define CANID_BMS_CELLS(x) (CANID_FCN_DATA | CANID_GRP_BMS | (x))
#define CANID_BMS_TEMPS (CANID_FCN_DATA | CANID_GRP_BMS | 0x3)
#define CANID_BMS_CURRENT_VOLTAGE (CANID_FCN_DATA | CANID_GRP_BMS | 0x4)

#define CANID_BMS_STATE (CANID_FCN_DATA | CANID_GRP_BMS | 0x8)
#define CANID_BMS_ERROR_FLAGS (CANID_FCN_DATA | CANID_GRP_BMS | 0x9)

#define CANID_BMS_DEBUG (CANID_FCN_DATA | CANID_GRP_BMS | 0xF)


void can_sender_send_status_messages();

void can_sender_send_data_messages();

void can_sender_send_debug(uint8_t value);
void can_sender_send_debug_16(uint16_t value);


#endif /* CAN_SENDER_H_ */