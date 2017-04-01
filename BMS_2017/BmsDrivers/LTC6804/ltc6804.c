/*
 * ltc6804.c
 *
 * Created: 21/03/2016 23:46:29
 *  Author: Sondre
 */ 

#include "ltc6804.h"
#include "ltc6804_pec.h"
#include "ltc6804_definitions.h"
#include "../../UniversalModuleDrivers/spi.h"
#include <util/delay.h>
#include <stdint.h>


static void ltc_wakeUp () {
	spi_trancieve(NULL, NULL, 1, true);
	_delay_us(T_WAKE_US);
}

ltc_ErrorCode_t ltc_sendCommand (ltc_Command_t command) {
	uint32_t cmd_pec;
	
	switch (command) {
	case Command_ADCV:
		cmd_pec = (ADCV_CMD << 16) | ADCV_PEC;
		break;
	case Command_ADOW_PUP0:
		cmd_pec = (ADOW_PUP0_CMD << 16) | ADOW_PUP0_PEC;
		break;
	case Command_ADOW_PUP1:
		cmd_pec = (ADOW_PUP1_CMD << 16) | ADOW_PUP1_PEC;
		break;
	case Command_CVST:
		cmd_pec = (CVST_CMD << 16) | CVST_PEC;
		break;
	case Command_ADAX:
		cmd_pec = (ADAX_CMD << 16) | ADAX_PEC;
		break;
	case Command_AXST:
		cmd_pec = (AXST_CMD << 16) | AXST_PEC;
		break;
	case Command_ADSTAT:
		cmd_pec = (ADSTAT_CMD << 16) | ADSTAT_PEC;
		break;
	case Command_STATST:
		cmd_pec = (STATST_CMD << 16) | STATST_PEC;
		break;
	case Command_ADCVAX:
		cmd_pec = (ADCVAX_CMD << 16) | ADCVAX_PEC;
		break;
	case Command_Diagn:
		cmd_pec = (DIAGN_CMD << 16) | DIAGN_PEC;
		break;
	default: return ERROR_UNKNOWN_COMMAND;
	}
	
	uint8_t cmd_buffer[4];
	cmd_buffer[0] = (cmd_pec >> 24) & 0xFF;
	cmd_buffer[1] = (cmd_pec >> 16) & 0xFF;
	cmd_buffer[2] = (cmd_pec >> 8) & 0xFF;
	cmd_buffer[3] = (cmd_pec >> 0) & 0xFF;
	
	ltc_wakeUp();
	spi_trancieve(cmd_buffer, NULL, 4, true);
	
	return NO_ERROR;
}
	
ltc_ErrorCode_t ltc_readRegisterGroup (ltc_RegisterGroup_t registerGroup, ltc_RegisterData_t* registerData) {
	uint32_t cmd_pec;
	
	switch (registerGroup) {
		case RegisterGroup_CONFIG:
			cmd_pec = (RDCFG_CMD << 16) | RDCFG_PEC;
			break;
		case RegisterGroup_CELLA:
			cmd_pec = (RDCVA_CMD << 16) | RDCVA_PEC;
			break;
		case RegisterGroup_CELLB:
			cmd_pec = (RDCVB_CMD << 16) | RDCVB_PEC;
			break;
		case RegisterGroup_CELLC:
			cmd_pec = (RDCVC_CMD << 16) | RDCVC_PEC;
			break;
		case RegisterGroup_CELLD:
			cmd_pec = (RDCVD_CMD << 16) | RDCVD_PEC;
			break;
		case RegisterGroup_AUXA:
			cmd_pec = (RDAUXA_CMD << 16) | RDAUXA_PEC;
			break;
		case RegisterGroup_AUXB:
			cmd_pec = (RDAUXB_CMD << 16) | RDAUXB_PEC;
			break;
		case RegisterGroup_STATA:
			cmd_pec = (RDSTATA_CMD << 16) | RDSTATA_PEC;
			break;
		case RegisterGroup_STATB:
			cmd_pec = (RDSTATB_CMD << 16) | RDSTATB_PEC;
			break;
		default: return ERROR_UNKNOWN_COMMAND;
	}
	
	uint8_t cmd_buffer[4];
	cmd_buffer[0] = (cmd_pec >> 24) & 0xFF;
	cmd_buffer[1] = (cmd_pec >> 16) & 0xFF;
	cmd_buffer[2] = (cmd_pec >> 8) & 0xFF;
	cmd_buffer[3] = (cmd_pec >> 0) & 0xFF;

	// Do transmission
	ltc_wakeUp();
	spi_trancieve_tx_then_rx(cmd_buffer, COMMAND_LENGTH + PEC_LENGTH, registerData->rawData, REGISTER_DATA_LENGTH + PEC_LENGTH);

	// Check pec
	uint16_t calculated_pec = pec_calculate(registerData->rawData, REGISTER_DATA_LENGTH);
	uint16_t recieved_pec = (registerData->rawData[6] << 8) | registerData->rawData[7];
	if (calculated_pec != recieved_pec)
	{
		return ERROR_PEC_MISMATCH;
	}
	
	return NO_ERROR;
}

ltc_ErrorCode_t ltc_writeConfigurationRegister (ltc_RegisterData_t* registerData)
{
	// Setup command
	uint8_t cmd_buffer[4];
	cmd_buffer[0] = WRCFG_CMD >> 8;
	cmd_buffer[1] = WRCFG_CMD & 0xFF;
	cmd_buffer[2] = WRCFG_PEC >> 8;
	cmd_buffer[3] = WRCFG_PEC & 0xFF;
	
	// Append pec to register data
	uint16_t pec = pec_calculate((uint8_t*)registerData->rawData, REGISTER_DATA_LENGTH);
	registerData->rawData[6] = (pec >> 8) & 0xFF;
	registerData->rawData[7] = (pec >> 0) & 0xFF;
	
	// Do transmission
	ltc_wakeUp();
	spi_trancieve(cmd_buffer, NULL, COMMAND_LENGTH + PEC_LENGTH, false);
	spi_trancieve(registerData->rawData, NULL, REGISTER_DATA_LENGTH + PEC_LENGTH, true);
	
	return NO_ERROR;
}