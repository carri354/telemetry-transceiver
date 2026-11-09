/*
 * gps.c
 *
 *  Created on: Oct 9, 2026
 *      Author: carri
 */

#include "gps.h"

/*
 * gps_init
 *
 * On entry, gps_init assumes that memory has been allocated
 * for the gps handle
 *
 * 1) Initializes all gps fields to zero
 * 2) Sets mode to automotive
 * 3) Enables UART port
 * 4) Enables GPS satellites
 */
void gps_init(GPS_Handle_t *gps, UART_HandleTypeDef *huart_){
	assert(gps!=NULL);
	// Step 1
	gps->huart = huart_;

	for (int i = 0; i < 101; i++) {
		gps->buf[i] = 0;
	}

	for (int i = 0; i < 30; i++) {
		gps->sw_version[i] = 0;
	}
	for (int i = 0; i < 10; i++) {
		gps->hw_version[i] = 0;
	}

	gps->lon = 0;
	gps->lat = 0;



	gps->year = 0;
	gps->month = 0;
	gps->day = 0;
	gps->hour = 0;
	gps->min = 0;
	gps->sec = 0;
	gps->fixType = 0;

	// Step 2
	ubx_send_message(gps, setAutomotiveMode, sizeof(setAutomotiveMode)/sizeof(uint8_t));
	// Step 3
	ubx_send_message(gps, enable_uart_port, sizeof(enable_uart_port)/sizeof(uint8_t));
	// Step 4
	ubx_send_message(gps, cfg_gnss, sizeof(cfg_gnss)/sizeof(uint8_t));
}


/*
 * gps_get_version
 * Polls for hardware and software version, places it into the gps handle
 */
void gps_get_version(GPS_Handle_t *gps){
	assert(gps != NULL);
	ubx_send_message(gps, ubx_mon_ver, sizeof(ubx_mon_ver)/sizeof(uint8_t));
	ubx_receive_message(gps, sizeof(ubx_mon_ver)/sizeof(uint8_t));
	// Copy swVersion and hwVersion into gps handle
	memcpy(gps->sw_version, gps->buf, sizeof(gps->sw_version));
	memcpy(gps->hw_version, gps->buf + 30, sizeof(gps->hw_version));

}


/*
 * gps_poll_pvt
 * Polls for Position, Velocity, AND Time
 *
 * Puts the result into the gps handle
 */
void gps_poll_pvt(GPS_Handle_t *gps){
	ubx_send_message(gps, getPVTData, sizeof(getPVTData)/sizeof(uint8_t));
	ubx_receive_message(gps, sizeof(getPVTData)/sizeof(uint8_t));
	// Get Position
	memcpy(&gps->lon, gps->buf + 24, sizeof(int32_t));
	memcpy(&gps->lat, gps->buf + 28, sizeof(int32_t));

	// Get velocity (TODO)

	// Get Timestamp
	memcpy(&gps->year, gps->buf + 4, sizeof(uint16_t));
	gps->month = gps->buf[6];
	gps->day = gps->buf[7];
	gps->hour = gps->buf[8];
	gps->min = gps->buf[9];
	gps->sec = gps->buf[10];
	gps->fixType = gps->buf[20];
}


/**
 * ubx_send_message
 * Transmits a ubx message as specified by https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf?utm_content=UBX-13003221
 * Hard-coded messages can be found in gps.h, CFG messages are necessary for startup
 *
 * On entry, ubx_send_message assumes:
 * 		-gps is a valid pointer to an initialized gps object
 * 		-msg contains a valid ubx message
 * 		-msg_size is the size (in bytes) of the message
 * On return, ubx_send_message guarantees:
 * 		- A message has been sent to the gps module
 *
 * Performance guarantee:
 * 		- This function is polling
 */
void ubx_send_message(GPS_Handle_t *gps, const uint8_t *msg, uint16_t msg_size){
	assert(gps != NULL);
	HAL_UART_Transmit(gps->huart, msg, msg_size, HAL_MAX_DELAY);
}
/**
 * ubx_receive_message
 * Receives a ubx message as specified by https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf?utm_content=UBX-13003221
 * Typically used right after a message send
 *
 * On entry, ubx_receive_message assumes:
 * 		-gps is a valid pointer to an initialized gps object
 * 		-msg_size is the size (in bytes) of the message
 * On return, ubx_send_message guarantees:
 * 		- A message has been received to the gps buffer
 *
 * Performance guarantee:
 * 		- This function is polling
 */
void ubx_receive_message(GPS_Handle_t *gps, uint16_t msg_size){
	HAL_UART_Receive(gps->huart, gps->buf, msg_size, HAL_MAX_DELAY);
}
