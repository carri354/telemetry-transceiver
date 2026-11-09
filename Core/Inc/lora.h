/*
 * lora.h
 *
 *  Created on: Oct 16, 2026
 *      Author: carri
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_

#include "radio_driver.h"


typedef enum{
	TX_NULL, // Unitialized
	TX_READY,
	TX_BUSY
}Lora_TX_State_t;

typedef enum{
	RX_NULL, // Unitialized
	RX_READY,
	RX_BUSY
}Lora_RX_State_t;

// General

/* lora_init
 * @brief Enables SMPS, sets RF switch to RX mode, initializes modulation params, packet params
 */
void lora_init(RadioLoRaSpreadingFactors_t SpreadingFactor, RadioLoRaBandwidths_t BW );
void lora_isr( RadioIrqMasks_t irq);

// Tx

/* lora_setupTX
 * @brief Changes payload length to 0x00, sets RF switch to TX mode
 * @assumes lora_init has already been called
 */
void lora_setupTX();
void lora_send();

// Rx

/* lora_setupTX
 * @brief Changes payload length to 0xFF , sets RF switch to RX mode
 * @assumes lora_init has already been called
 */
void lora_setupRX();
void lora_startContinuousRx(uint8_t timeout);
uint8_t lora_receive(uint8_t *buffer, uint8_t *size, uint8_t maxSize);

#endif /* INC_LORA_H_ */
