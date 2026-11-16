
#ifndef MCP_H
#define MCP_H

#include "main.h"
#include <stdint.h>
#include <stdio.h>

/* request bytes */
#define READ_STAT 0xA0
#define CANSTAT_OPMOD_MASK 0xE0 // used to mask for current op mode from CANSTAT

/* SPI Instruction Sets */
#define MCP_INST_RESET          0xC0
#define MCP_INST_READ           0x03
#define MCP_INST_WRITE          0x02
#define MCP_INST_READ_STATUS    0xA0
#define MCP_INST_RX_STATUS      0xB0
#define MCP_INST_BIT_MODIFY     0x05
#define MCP_INST_READ_RX0_BUFFER 0x90
#define MCP_INST_READ_RX1_BUFFER 0x94

#define CANINTF_RX0IF 0x01
#define CANINTF_RX1IF 0x02

/* bit timing */
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28
#define PRESCALER 0x04
#define PRSEG 0x05
#define PS1 0x08
#define PS2 0x02

/* Register addresses */
#define REG_BFPCTRL 0x0C
#define REG_TXRTSCTRL 0x0D
#define REG_CANSTAT 0x0E
#define REG_CANCTRL 0x0F
#define REG_TEC 0x1C
#define REG_REC 0x1D
#define REG_CNF3 0x28
#define REG_CNF2 0x29
#define REG_CNF1 0x2A
#define REG_CANINTE 0x2B
#define REG_CANINTF 0x2C
#define REG_EFLG 0x2D
#define REG_TXB0CTRL 0x30
#define REG_TXB1CTRL 0x40
#define REG_TXB2CTRL 0x50
#define REG_RXB0CTRL 0x60
#define REG_RXB1CTRL 0x70

/* Masks for Modes */
#define MASK_REQOP_CONFIG 0x80
#define MASK_REQOP_NORMAL 0x00
#define MASK_REQOP_SLEEP 0x20
#define MASK_REQOP_LOOPBACK 0x40
#define MASK_REQOP_LISTEN 0x60

typedef enum {
    MODE_MCP_NORMAL,
    MODE_MCP_SLEEP,
    MODE_MCP_LOOPBACK,
    MODE_MCP_LISTEN,
    MODE_MCP_CONFIG
} MCP_Modes_t;

typedef enum {
    MCP_OK,
    MCP_SPI_ERROR,
    MCP_TIMEOUT,
    MCP_INVALID_MODE,
    MCP_INVALID_ARG,
    MCP_CONFIG_ERROR,
    MCP_NO_MESSAGE
} MCP_Status_t;

MCP_Status_t MCP_ReadStatus(uint8_t * result);
MCP_Status_t MCP_ReadRegister(uint8_t address, uint8_t * result);
MCP_Status_t MCP_WriteRegister(uint8_t address, uint8_t data);
MCP_Status_t MCP_BitModify(uint8_t address, uint8_t data, uint8_t mask);
MCP_Status_t MCP_Reset(void);
MCP_Status_t MCP_InitBitTiming(void);
MCP_Status_t MCP_ChangeModes(MCP_Modes_t mode);
MCP_Status_t MCP_Init(void);
MCP_Status_t MCP_ReadRxBuffer(uint8_t * rxb0, uint8_t * rxb1, uint8_t * msgCount);

#endif /* MCP_H */
