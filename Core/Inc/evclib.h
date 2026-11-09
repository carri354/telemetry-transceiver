/*
 * evclib.h
 *
 *  Created on: May 30, 2026
 *      Author: Carlos Arriola
 */

#ifndef INC_EVCLIB_H_
#define INC_EVCLIB_H_


// Includes
#include "usart.h"

// Defines
//Comment out to disable debug messages
#define DEBUG_ON

// String literals for message types and tags
#define DBG_NONE ""
#define DBG_I "[INFO]"
#define DBG_W "[WARNING]"
#define DBG_E "[ERROR]"
#define DBG_DBG "[DBG]"
#define DBG_MAIN "[MAIN]"
#define DBG_HEX_PREFIX "0x"
#define DBG_NEWLN "\n"


/* Prototypes */
void DBG_Init(UART_HandleTypeDef* huart_);
void DBG(const char* type, const char* tag, char *c, ...);

// Typedef

// CAN TRC IDs
// Transmitter_Receiver_Category-ID
enum CAN_IDs{
	Auto_ID1 = 0,
	Auto_ID2 = 1,
	Brake_ID = 2,
	Accel_ID = 3,
	Dash_Motor_ID = 4, // Cruise control, Dead man's switch
	Any_Lights_ID = 5,
	Any_HornWiper_ID = 6,
	Motor_Any_Current_ID = 7,
	Motor_Any_Speed_ID = 8,
	Telem_Any_GPS_ID = 9,
	IMU_ID = 10,
	BlindSpot_ID = 11

};




#endif /* INC_EVCLIB_H_ */
