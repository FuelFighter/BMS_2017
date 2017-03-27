/*
 * ltc6804_definitions.h
 *
 * Created: 07.02.2015 13:11:25
 *  Author: Mikael
 */ 


#ifndef LTC6804_DEFINITIONS_H_
#define LTC6804_DEFINITIONS_H_


#define COMMAND_LENGTH 2
#define REGISTER_DATA_LENGTH 6
#define PEC_LENGTH 2
#define T_WAKE_US 300


#define DUMMY_BYTE 0xA5

#define WRCFG_CMD 0x0001UL			
#define WRCFG_PEC 0x3d6eUL

#define RDCFG_CMD 0x0002UL			
#define RDCFG_PEC 0x2b0aUL

#define RDCVA_CMD 0x0004UL			
#define RDCVA_PEC 0x07c2UL

#define RDCVB_CMD 0x0006UL			
#define RDCVB_PEC 0x9a94UL

#define RDCVC_CMD 0x0008UL			
#define RDCVC_PEC 0x5e52UL

#define RDCVD_CMD 0x000AUL			
#define RDCVD_PEC 0xc304UL

#define RDAUXA_CMD 0x000cUL		
#define RDAUXA_PEC 0xefccUL

#define RDAUXB_CMD 0x000eUL
#define RDAUXB_PEC 0x729aUL

#define RDSTATA_CMD 0x0010UL			
#define RDSTATA_PEC 0xed72UL

#define RDSTATB_CMD 0x0012UL			
#define RDSTATB_PEC 0x7024UL

#define ADCV_CMD 0x0360UL			/* MD[1:0] = 0b10, DCP = 0b0, CH[2:0] = 0b000 */
#define ADCV_PEC 0xf46cUL

#define ADOW_PUP0_CMD 0x0328UL	/* MD[1:0] = 0b10, PUP = 0b0, DCP = 0b0, CH[2:0] = 0b00 */
#define ADOW_PUP0_PEC 0xfbe8UL

#define ADOW_PUP1_CMD 0x0368UL	/* MD[1:0] = 0b10, PUP = 0b1, DCP = 0b0, CH[2:0] = 0b00 */
#define ADOW_PUP1_PEC 0x1c62UL

#define CVST_CMD 0x0307UL			/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define CVST_PEC 0x0240UL

#define ADAX_CMD 0x0560UL			/* MD[1:0] = 0b10, CHG[2:0] = 0b000 */
#define ADAX_PEC 0xd3a0UL

#define AXST_CMD 0x0507UL			/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define AXST_PEC 0x258cUL

#define ADSTAT_CMD 0x0568UL		/* MD[1:0] = 0b10, CHST[2:0] = 0b000 */
#define ADSTAT_PEC 0x3baeUL

#define STATST_CMD 0x050fUL		/* MD[1:0] = 0b10, ST[1:0] = 0b00 */
#define STATST_PEC 0xcd82UL

#define ADCVAX_CMD 0x056fUL		/* MD[1:0] = 0b10, DCP = 0b0 */
#define ADCVAX_PEC 0x9c54UL

#define DIAGN_CMD 0x0715UL
#define DIAGN_PEC 0x785eUL


#endif /* LTC6804_DEFINITIONS_H_ */