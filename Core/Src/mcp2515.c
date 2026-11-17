/*
 * mcp2515.c
 *
 *  Created on: Nov 16, 2026
 *      Author: carri
 */



#include "mcp2515.h"
#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "stm32wl55xx.h"
#include "stm32wlxx_hal_def.h"
#include "stm32wlxx_hal_gpio.h"
#include "stm32wlxx_hal_spi.h"
#include <stdint.h>
#include <string.h>

MCP_Status_t MCP_ReadStatus(uint8_t * result) {
    if (result == NULL) return MCP_INVALID_ARG;

    uint8_t tx[2] = {MCP_INST_READ_STATUS, 0x00};
    uint8_t rx[2] = {0};

    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;

    *result = rx[1];
    return MCP_OK;
}

MCP_Status_t MCP_ReadRegister(uint8_t address, uint8_t * result) {
    if (result == NULL) return MCP_INVALID_ARG;

    uint8_t tx[3] = {MCP_INST_READ, address, 0x00};
    uint8_t rx[3] = {0};

    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 3, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;

    *result = rx[2];
    return MCP_OK;
}

MCP_Status_t MCP_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t tx[3] = {MCP_INST_WRITE, address, data};

    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, tx, 3, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;
    return MCP_OK;
}

MCP_Status_t MCP_BitModify(uint8_t address, uint8_t data, uint8_t mask) {
    uint8_t tx[4] = {MCP_INST_BIT_MODIFY, address, mask, data & mask};

    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, tx, 4, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;
    return MCP_OK;
}

MCP_Status_t MCP_Reset(void) {
    uint8_t tx = 0xC0;

    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, &tx, 1, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;
    return MCP_OK;
}

MCP_Status_t MCP_InitBitTiming(void) {
    uint8_t cnf1 = 0x01;
    uint8_t cnf2 = 0xBC;
    uint8_t cnf3 = 0x01;

    uint8_t status = MCP_WriteRegister(REG_CNF1, cnf1);
    if (status != MCP_OK) return status;

    status = MCP_WriteRegister(REG_CNF2, cnf2);
    if (status != MCP_OK) return status;

    status = MCP_WriteRegister(REG_CNF3, cnf3);
    if (status != MCP_OK) return status;

    return MCP_OK;
}

MCP_Status_t MCP_ChangeModes(MCP_Modes_t mode) {
    uint8_t reg;

    switch (mode) {
        case MODE_MCP_NORMAL:
            reg = MASK_REQOP_NORMAL;
            break;
        case MODE_MCP_SLEEP:
            reg = MASK_REQOP_SLEEP;
            break;
        case MODE_MCP_LOOPBACK:
            reg = MASK_REQOP_LOOPBACK;
            break;
        case MODE_MCP_LISTEN:
            reg = MASK_REQOP_LISTEN;
            break;
        case MODE_MCP_CONFIG:
            reg = MASK_REQOP_CONFIG;
            break;
        default:
            reg = MASK_REQOP_NORMAL;
            break;
    }

    MCP_Status_t status = MCP_BitModify(REG_CANCTRL, reg, CANSTAT_OPMOD_MASK);
    if (status != MCP_OK) return MCP_CONFIG_ERROR;

    HAL_Delay(100);

    uint8_t canstat = 0;
    status = MCP_ReadRegister(REG_CANSTAT, &canstat);
    if (status != MCP_OK) return status;

    if ((canstat & CANSTAT_OPMOD_MASK) == reg) return MCP_OK;

    return MCP_TIMEOUT;
}

MCP_Status_t MCP_Init(void) {
    MCP_Status_t status = MCP_Reset();
    if (status != MCP_OK) return status;

    HAL_Delay(100);

    uint8_t canstat = 0;
    status = MCP_ReadRegister(REG_CANSTAT, &canstat);
    if (status != MCP_OK) return status;
    if ((canstat & CANSTAT_OPMOD_MASK) != MASK_REQOP_CONFIG) return MCP_CONFIG_ERROR;

    status = MCP_WriteRegister(REG_RXB0CTRL, 0x64);
    if (status != MCP_OK) return status;

    status = MCP_WriteRegister(REG_RXB1CTRL, 0x60);
    if (status != MCP_OK) return status;

    // Enable RXB0 and RXB1 interrupts
    status = MCP_BitModify(REG_CANINTE, 0x03, 0x03);
    if (status != MCP_OK) return status;

    status = MCP_InitBitTiming();
    if (status != MCP_OK) return status;

    status = MCP_ChangeModes(MODE_MCP_NORMAL);
    return status;
}

static MCP_Status_t MCP_ReadSingleRxBuffer(uint8_t inst, uint8_t * buffer) {
    if (buffer == NULL) return MCP_INVALID_ARG;

    uint8_t tx[14] = {0};
    uint8_t rx[14] = {0};

    tx[0] = inst;
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 14, 100);
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);

    if (status != HAL_OK) return MCP_SPI_ERROR;
    memcpy(buffer, &rx[1], 13);
    return MCP_OK;
}

MCP_Status_t MCP_ReadRxBuffer(uint8_t * rxb0, uint8_t * rxb1, uint8_t * msgCount) {
    if (rxb0 == NULL || rxb1 == NULL || msgCount == NULL) return MCP_INVALID_ARG;

    uint8_t canintf = 0;
    MCP_Status_t status = MCP_ReadRegister(REG_CANINTF, &canintf);

    *msgCount = 0;

    if (canintf & CANINTF_RX0IF) {
        status = MCP_ReadSingleRxBuffer(MCP_INST_READ_RX0_BUFFER, rxb0);

        if (status != MCP_OK) return status;
        (*msgCount)++;
    }

    if (canintf & CANINTF_RX1IF) {
        status = MCP_ReadSingleRxBuffer(MCP_INST_READ_RX1_BUFFER, rxb1);

        if (status != MCP_OK) return status;
        (*msgCount)++;
    }

    if (*msgCount == 0) return MCP_NO_MESSAGE;

    return MCP_OK;
}
