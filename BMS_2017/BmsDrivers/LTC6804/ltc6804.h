/*
 * ltc6804.h
 *
 * Created: 21/03/2016 23:41:24
 *  Author: Sondre
 */ 


#ifndef LTC6804_H_
#define LTC6804_H_

#include <stdint.h>
#include <stdbool.h>


#define LTC_T_REFUP 5
#define LTC_T_CYCLE_US 2480


typedef enum {
	RegisterGroup_CONFIG,
	RegisterGroup_CELLA,
	RegisterGroup_CELLB,
	RegisterGroup_CELLC,
	RegisterGroup_CELLD,
	RegisterGroup_AUXA,
	RegisterGroup_AUXB,
	RegisterGroup_STATA,
	RegisterGroup_STATB
} ltc_RegisterGroup_t;

typedef enum {
	Command_ADCV,			// Start Cell Voltage ADC Conversion and Poll status
	Command_ADOW_PUP0,		// Start Open Wire ADC Conversion and Poll Status (pull-down current)
	Command_ADOW_PUP1,		// Start Open Wire ADC Conversion and Poll Status (pull-up current)
	Command_CVST,			// Start Self-Test Cell Voltage Conversion and Poll Status
	Command_ADAX,			// Start GPIOs ADC Conversion and Poll Status
	Command_AXST,			// Start Self-Test GPIOs Conversion and Poll Status
	Command_ADSTAT,			// Start Status group ADC Conversion and Poll Status
	Command_STATST,			// Start Self-Test Status group Conversion and Pol Status
	Command_ADCVAX,			// Start Combined Cell VOltage and GPIO1, GPIO2 Conversion and Poll Status
	Command_Diagn			// Diagnose MUX and Poll Status
} ltc_Command_t;

typedef enum {
	DischargeTimeout_DISABLED,
	DischargeTimeout_SEC30,
	DischargeTimeout_MIN1,
	DischargeTimeout_MIN2,
	DischargeTimeout_MIN3,
	DischargeTimeout_MIN4,
	DischargeTimeout_MIN5,
	DischargeTimeout_MIN10,
	DischargeTimeout_MIN15,
	DischargeTimeout_MIN20,
	DischargeTimeout_MIN30,
	DischargeTimeout_MIN40,
	DischargeTimeout_MIN60,
	DischargeTimeout_MIN75,
	DischargeTimeout_MIN90,
	DischargeTimeout_MIN120
} ltc_DischargeTimeOutValue_t;

typedef enum {
	MuxStatus_NO_FAILURE,
	MuxStatus_FAILURE
} ltc_MuxFailStatus_t;

typedef enum {
	ThermalStatus_NO_SHUTDOWN,
	ThermalStatus_SHUTDOWN
} ltc_ThermalShutdownStatus_t;

typedef enum {
	NO_ERROR = 0,
	ERROR_TIMEOUT = (1 << 0),
	ERROR_PEC_MISMATCH = (1 << 1),
	ERROR_UNKNOWN_COMMAND = (1 << 2)
} ltc_ErrorCode_t;


typedef union {
	uint8_t rawData[8];

	struct {
		uint8_t adcModeOpt			: 1;
		uint8_t swtrd				: 1;
		uint8_t refon				: 1;
		uint8_t gpioValu			: 5;
		uint8_t UNUSED1				: 8;
		uint16_t UNUSED2			: 16;
		uint16_t dischargeEnable_bf	: 12;
		ltc_DischargeTimeOutValue_t dischargeTimeout	: 4;
	} config;

	struct {
		uint16_t cellVoltage0_3_6_9;
		uint16_t cellVoltage1_4_7_10;
		uint16_t cellVoltage2_5_8_11;
	} cv;

	struct {
		uint16_t gpioVoltage1;
		uint16_t gpioVoltage2;
		uint16_t gpioVoltage3;
	} auxA;

	struct {
		uint16_t gpioVoltage4;
		uint16_t gpioVoltage5;
		uint16_t refVoltage;
	} auxB;

	struct {
		uint16_t sumOfCellsVoltage;
		uint16_t internalTemperature;
		uint16_t analogPowerSupplyVoltage;
	} statusA;

	struct {
		uint16_t digitalPowerSupplyVoltage;
		uint16_t UNUSED1;
		uint8_t UNUSED2;
		ltc_ThermalShutdownStatus_t thermalShutdownStatus	: 1;
		ltc_MuxFailStatus_t muxFailStatus					: 1;
		uint8_t UNUSED3 									: 2;
		uint8_t deviceRevisionCode							: 4;
	} statusB;

} ltc_RegisterData_t;


ltc_ErrorCode_t ltc_sendCommand (ltc_Command_t command);

ltc_ErrorCode_t ltc_readRegisterGroup (ltc_RegisterGroup_t registerGroup, ltc_RegisterData_t* registerData);

ltc_ErrorCode_t ltc_writeConfigurationRegister (ltc_RegisterData_t* configurationRegister);


#endif /* LTC6804_H_ */