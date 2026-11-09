/*
 * lora.c
 *
 *  Created on: Oct 16, 2026
 *      Author: carri
 */
#include "lora.h"
#include "evclib.h"

static uint8_t payload_size = 8;
static PacketParams_t PacketParams = {0};
static Lora_TX_State_t lora_tx_state = TX_NULL;
static Lora_RX_State_t lora_rx_state = RX_NULL;

void lora_init(RadioLoRaSpreadingFactors_t SpreadingFactor, RadioLoRaBandwidths_t BW ){
	// Note that this will start in RX by default
	SUBGRF_Init((DioIrqHandler)&lora_isr);
	SUBGRF_SetStandby(STDBY_RC);
	// 915 MHz
	SUBGRF_SetRfFrequency(915000000);


	ModulationParams_t ModulationParams = {0};
	ModulationParams.PacketType = PACKET_TYPE_LORA;
	ModulationParams.Params.LoRa.SpreadingFactor = SpreadingFactor;
	ModulationParams.Params.LoRa.Bandwidth = BW;
	ModulationParams.Params.LoRa.CodingRate = LORA_CR_4_5;
	ModulationParams.Params.LoRa.LowDatarateOptimize = 0;

	SUBGRF_SetModulationParams(&ModulationParams);


	SUBGRF_SetPacketType(ModulationParams.PacketType);


	PacketParams.PacketType = PACKET_TYPE_LORA;
	PacketParams.Params.LoRa.CrcMode = LORA_CRC_ON;
	PacketParams.Params.LoRa.PayloadLength = 0;
	// Convenience for testing
	PacketParams.Params.LoRa.HeaderType = LORA_PACKET_FIXED_LENGTH;
	PacketParams.Params.LoRa.PreambleLength = 8;

	SUBGRF_SetPacketParams(&PacketParams);

	SUBGRF_SetBufferBaseAddress(0x00, 0x00 );
}

void lora_isr( RadioIrqMasks_t irq){
	if(IRQ_TX_DONE & irq){
		lora_tx_state = TX_READY;
	}

	if(IRQ_RX_DONE & irq){

	}

	if(IRQ_RX_TX_TIMEOUT & irq){
		DBG(DBG_MAIN, DBG_E, "LoRa Timeout Error");
	}

	if(IRQ_CRC_ERROR & irq){
		DBG(DBG_MAIN, DBG_E, "LoRa CRC Error");
	}



	SUBGRF_ClearIrqStatus(irq);
}

void lora_setupTX(){
	SUBGRF_SetStandby(STDBY_RC);
	SUBGRF_SetSwitch(RFO_HP, RBI_SWITCH_RFO_HP);

	PacketParams.Params.LoRa.PayloadLength = 0;
	SUBGRF_SetPacketParams(&PacketParams);

	lora_tx_state = TX_READY;
}

void lora_send(uint8_t *CAN_msg, uint32_t timeout){
	// For now, we assume we are just sending 8 byte messages
	if(lora_tx_state != TX_READY){
		DBG(DBG_MAIN, DBG_E, "LoRa TX is not ready!");
		return;
	}
	lora_tx_state = TX_BUSY;
	PacketParams.Params.LoRa.PayloadLength = payload_size;

	SUBGRF_SetPacketParams(&PacketParams);
	SUBGRF_SendPayload(CAN_msg, payload_size, timeout);

}

void lora_setupRX(){

	SUBGRF_SetStandby(STDBY_RC);
	SUBGRF_SetSwitch(0, RBI_SWITCH_RX);

	PacketParams.Params.LoRa.PayloadLength = 0xFF;
	SUBGRF_SetPacketParams(&PacketParams);

	lora_rx_state = RX_READY;
}

void lora_startContinuousRx(uint8_t timeout){
	// For now, we assume we are just sending 8 byte messages
	if(lora_rx_state != RX_READY){
		DBG(DBG_MAIN, DBG_E, "LoRa RX is not ready!");
		return;
	}
	lora_rx_state = RX_BUSY;
	PacketParams.Params.LoRa.PayloadLength = payload_size;
	SUBGRF_SetPacketParams(&PacketParams);

	SUBGRF_SetRx(timeout);

}

uint8_t lora_receive(uint8_t *buffer, uint8_t *size, uint8_t maxSize){

	return SUBGRF_GetPayload(buffer, size, maxSize);
}
