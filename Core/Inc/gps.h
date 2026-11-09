/*
 * gps.h
 *
 *  Created on: Oct 9, 2026
 *      Author: carri
 *
 *      GPS Driver Code for the ublox NEO-M8N.
 *      To interface with the GPS module, define a gps handle and provide it to the init function.
 *      Afterwards, functions like gps_update_position will update the members of the provided handle.
 *  	To extract latitude and longitude:
 *
 *  	1) gps_init(&h_gps, &huart1);
 *      2) gps_update_position(&h_gps);
 *      3) uint32_t longitude = h_gps->lon;
 *      4) uint32_t latitude = h_gps->lat;
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stm32wlxx.h"
#include <assert.h>
#include <string.h>

typedef struct{
	UART_HandleTypeDef *huart;
	uint8_t buf[101];

	uint8_t sw_version[30];
	uint8_t hw_version[10];

	// Position
//	float f_Longitude;
//	float f_Latitude;
	int32_t lon;
	int32_t lat;



	// Velocity
//	int32_t velN;
//	int32_t velE;
//	int32_t velD;
//	int32_t gspeed; // ground speed

	// Time
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t fixType;
}GPS_Handle_t;

#endif /* INC_GPS_H_ */


// Exported Functions / User-Interface
void gps_init(GPS_Handle_t *gps, UART_HandleTypeDef *huart_);


void gps_poll_pvt(GPS_Handle_t *gps); // Does position, time, and velocity

void gps_get_version(GPS_Handle_t *gps); // useful for verifying that the gps is alive


// Internal Definitions


// Internal functions
void ubx_send_message(GPS_Handle_t *gps, const uint8_t *msg, uint16_t msg_size);
void ubx_receive_message(GPS_Handle_t *gps, uint16_t msg_size);


// UBX messages

// UBX-CFG
	static const uint8_t setAutomotiveMode[]={0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x04,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x82,0xC4};
	static const uint8_t ubx_mon_ver[]={0xB5,0x62,0x0A,0x04, 0x00, 0x00, 0x0E, 0x34};
	// Enable GNSS satellites
	static const uint8_t cfg_gnss[] = {0xB5,0x62,0x06,0x3E,0x24,0x00,0x00,0x00,0x20,0x04,0x00,0x08,0x10,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x03,0x00,0x01,0x00,0x01,0x01,0x02,0x04,0x08,0x00,0x01,0x00,0x01,0x01,0x06,0x08,0x0E,0x00,0x01,0x00,0x01,0x01,0xDF,0xFB};
 	static const uint8_t enable_uart_port[] = {
 	    0xB5, 0x62,                   // Sync Chars
 	    0x06, 0x00,                   // Class (CFG), ID (PRT)
 	    0x14, 0x00,                   // Length of Payload: 20 bytes (Little-Endian)
 	    0x01,                         // Port ID: 1 (UART1)
 	    0x00,                         // Reserved
 	    0x00, 0x00,                   // TX Ready: 0 (Disabled)
 	    0xD0, 0x08, 0x00, 0x00,       // Mode bitmask: 8N1 (0x000008D0)
		// 0x00, 0xC2, 0x01, 0x00,    // Baud Rate: 115200 (0x0001C200)
		0x80, 0x25, 0x00, 0x00,       // Baud Rate: 9600 (0x00002580)
 	    0x01, 0x00,                   // Inbound Protocols: UBX only (0x0001)
 	    0x01, 0x00,                   // Outbound Protocols: UBX only (0x0001)
 	    0x00, 0x00,                   // Flags: 0
 	    0x00, 0x00,                   // Reserved
	    // 0x3A, 0x21                 // Old Fletcher-16 Checksum (115200)
	    0x9A, 0x79                    // Fletcher-16 Checksum (CK_A, CK_B) for 9600
 	};
// UBX-NAV
static const uint8_t getPVTData[]={0xB5,0x62,0x01,0x07,0x00,0x00,0x08,0x19};
