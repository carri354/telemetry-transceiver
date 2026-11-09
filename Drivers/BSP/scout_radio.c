/*
 * scout_radio.c
 *
 *  Created on: Oct 16, 2026
 *      Author: carri
 */

#include "scout_radio.h"

scout_radio_Switch_TypeDef state;

int32_t scout_radio_init(void){
	// Start in receiving mode by default
	  HAL_GPIO_WritePin(RF_TX_EN_GPIO_Port, RF_TX_EN_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(RF_TX_EN_N_GPIO_Port, RF_TX_EN_N_Pin, GPIO_PIN_SET);
	  state = SCOUT_SWITCH_RX;
	  return 0;
}

int32_t scout_radio_DeInit(void){
	// End in receiving mode
	HAL_GPIO_WritePin(RF_TX_EN_GPIO_Port, RF_TX_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RF_TX_EN_N_GPIO_Port, RF_TX_EN_N_Pin, GPIO_PIN_SET);
	state = SCOUT_SWITCH_RX;
	return 0;
}


int32_t scout_radio_ConfigRFSwitch(scout_radio_Switch_TypeDef Config){
	switch(Config){
		case SCOUT_SWITCH_RX:
			HAL_GPIO_WritePin(RF_TX_EN_GPIO_Port, RF_TX_EN_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RF_TX_EN_N_GPIO_Port, RF_TX_EN_N_Pin, GPIO_PIN_SET);
			state = SCOUT_SWITCH_RX;
		break;
		case SCOUT_SWITCH_RFO_HP:
			HAL_GPIO_WritePin(RF_TX_EN_GPIO_Port, RF_TX_EN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RF_TX_EN_N_GPIO_Port, RF_TX_EN_N_Pin, GPIO_PIN_RESET);
			state = SCOUT_SWITCH_RFO_HP;
		break;
	}
	return 0;
}


int32_t scout_radio_GetTxConfig(void){
	return state;
}

int32_t scout_radio_IsTCXO(void){
	return 0;
}


int32_t scout_radio_IsDCDC(void){
	return 1;
}


int32_t scout_radio_GetRFOMaxPowerConfig(){
	return 22;
}
