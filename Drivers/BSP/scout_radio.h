/*
 * scout_radio.h
 *
 *  Created on: Oct 16, 2026
 *      Author: carri
 *
 *  This file includes BSP functions for the Illini EV Concept Scout Telemetry Transceiver
 */

#ifndef BSP_SCOUT_RADIO_H_
#define BSP_SCOUT_RADIO_H_

#include "stm32wlxx_hal.h"
#include "scout_conf.h"

typedef enum{
	  SCOUT_SWITCH_RX     = 0,
	  SCOUT_SWITCH_RFO_HP = 1,
}scout_radio_Switch_TypeDef;

/**
  * @brief  Init Radio Switch
  * @return BSP status
  */
int32_t scout_radio_init(void);

/**
  * @brief  DeInit Radio Switch
  * @return BSP status
  */
int32_t scout_radio_DeInit(void);

/**
  * @brief  Configure Radio Switch.
  * @param  Config: Specifies the Radio RF switch path to be set.
  *         This parameter can be one of following parameters:
  *           @arg RADIO_SWITCH_OFF
  *           @arg RADIO_SWITCH_RX
  *           @arg RADIO_SWITCH_RFO_LP
  *           @arg RADIO_SWITCH_RFO_HP
  * @return BSP status
  */
int32_t scout_radio_ConfigRFSwitch(scout_radio_Switch_TypeDef Config);

/**
  * @brief  Return Board Configuration
  * @retval scout_radio_CONF_RFO_LP_HP
  * @retval scout_radio_CONF_RFO_LP
  * @retval scout_radio_CONF_RFO_HP
  */
int32_t scout_radio_GetTxConfig(void);

/**
  * @brief  Get If TCXO is to be present on board
  * @note   never remove called by MW,
  * @retval return 1 if present, 0 if not present
  */
int32_t scout_radio_IsTCXO(void);

/**
  * @brief  Get If DCDC is to be present on board
  * @note   never remove called by MW,
  * @retval return 1 if present, 0 if not present
  */
int32_t scout_radio_IsDCDC(void);

/**
  * @brief  Return RF Output Max Power Configuration of matching circuit
  * @note   never remove called by MW,
  * @retval return Max Power configuration of matching circuit for Low Power or High Power mode in dBm
  */
int32_t scout_radio_GetRFOMaxPowerConfig();

#endif /* BSP_SCOUT_RADIO_H_ */
