/*
 * ltc6804_definitions.h
 *
 * Created: 07.02.2015 13:11:25
 *  Author: Mikael
 */ 


#ifndef LTC6804_DEFINITIONS_H_
#define LTC6804_DEFINITIONS_H_


#define REGISTER_DATA_LENGTH 6
#define PEC_LENGTH 2
#define COMMAND_LENGTH 2
#define REGISTER_TRANSMISSION_LENGTH (COMMAND_LENGTH + PEC_LENGTH + (REGISTER_DATA_LENGTH + PEC_LENGTH) * NUMBER_OF_SLAVES)
#define COMMAND_TRANSMISSION_LENGTH (COMMAND_LENGTH + PEC_LENGTH)


#define DUMMY_BYTE 0xA5

#define WRCFG_CMD 0x0001			
#define WRCFG_PEC 0x3d6e

#define RDCFG_CMD 0x0002			
#define RDCFG_PEC 0x2b0a

#define RDCVA_CMD 0x0004			
#define RDCVA_PEC 0x07c2

#define RDCVB_CMD 0x0006			
#define RDCVB_PEC 0x9a94

#define RDCVC_CMD 0x0008			
#define RDCVC_PEC 0x5e52

#define RDCVD_CMD 0x000A			
#define RDCVD_PEC 0xc304

#define RDAUXA_CMD 0x000C			
#define RDAUXA_PEC 0xefcc

#define RDAUXB_CMD 0x000E			
#define RDAUXB_PEC 0x729a

#define RDSTATA_CMD 0x0010			
#define RDSTATA_PEC 0xed72

#define RDSTATB_CMD 0x0012			
#define RDSTATB_PEC 0x7024

#define ADCV_CMD 0x0360			/* MD[1:0] = 0b10, DCP = 0b0, CH[2:0] = 0b000 */
#define ADCV_PEC 0xf46c

#define ADOW_PUP0_CMD 0x0328	/* MD[1:0] = 0b10, PUP = 0b0, DCP = 0b0, CH[2:0] = 0b00 */
#define ADOW_PUP0_PEC 0xfbe8

#define ADOW_PUP1_CMD 0x0368	/* MD[1:0] = 0b10, PUP = 0b1, DCP = 0b0, CH[2:0] = 0b00 */
#define ADOW_PUP1_PEC 0x1c62

#define CVST_CMD 0x0307			/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define CVST_PEC 0x0240

#define ADAX_CMD 0x0560			/* MD[1:0] = 0b10, CHG[2:0] = 0b000 */
#define ADAX_PEC 0xd3a0

#define AXST_CMD 0x0507			/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define AXST_PEC 0x258c

#define ADSTAT_CMD 0x0568		/* MD[1:0] = 0b10, CHST[2:0] = 0b000 */
#define ADSTAT_PEC 0x3bae

#define STATST_CMD 0x050f		/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define STATST_PEC 0xcd82

#define ADCVAX_CMD 0x056f		/* MD[1:0] = 0b10, DCP = 0b0 */
#define ADCVAX_PEC 0x9c54

#define DIAGN_CMD 0x0715
#define DIAGN_PEC 0x785e


#endif /* LTC6804_DEFINITIONS_H_ */