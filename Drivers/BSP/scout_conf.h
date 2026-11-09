/*
 * scout_conf.h
 *
 *  Created on: Oct 19, 2026
 *      Author: carri
 */

#ifndef BSP_SCOUT_CONF_H_
#define BSP_SCOUT_CONF_H_

// This is the only correct GPIO pinout for the Scout board
#define SPI1_CS_Pin GPIO_PIN_8
#define SPI1_CS_GPIO_Port GPIOB
#define CAN_INT_N_Pin GPIO_PIN_0
#define CAN_INT_N_GPIO_Port GPIOA
#define DBG_LED_1_Pin GPIO_PIN_6
#define DBG_LED_1_GPIO_Port GPIOA
#define DBG_LED_2_Pin GPIO_PIN_7
#define DBG_LED_2_GPIO_Port GPIOA
#define RF_TX_EN_N_Pin GPIO_PIN_9
#define RF_TX_EN_N_GPIO_Port GPIOA
#define GPS_RESET_N_Pin GPIO_PIN_12
#define GPS_RESET_N_GPIO_Port GPIOB
#define RF_TX_EN_Pin GPIO_PIN_15
#define RF_TX_EN_GPIO_Port GPIOC

#endif /* BSP_SCOUT_CONF_H_ */
