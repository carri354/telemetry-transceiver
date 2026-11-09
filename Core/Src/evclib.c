/*
 * evclib.c
 *
 *  Created on: May 30, 2026
 *      Author: carri
 */

// Includes
#include "evclib.h"
#include <stdarg.h>
#include <stdio.h>




// Global Variables
static UART_HandleTypeDef* huart = NULL; // pointer to uart handle (works with any channel)






// **Must be called prior to using DBG**
void DBG_Init(UART_HandleTypeDef* huart_){
	huart = huart_;
}


// DBG Created by Cole Trautman
/* Sends a debug message over the virtual COM port with a type (info, warning, or error)
 * and peripheral tag. Uses the same format specifiers as printf, and currently supports
 * printing decimal and hexadecimal arguments.
 */

void DBG(const char* type, const char* tag, char *c, ...)
{
	#ifdef DEBUG_ON
		uint8_t* s;
		va_list lst;
		va_start(lst, c);

		// Print type string
		s = (uint8_t*)type;
		while(*s != '\0'){
			HAL_UART_Transmit(huart, s, 1, 10);
			s++;
		}

		// Print tag string
		s = (uint8_t*)tag;
		while(*s != '\0'){
			HAL_UART_Transmit(huart, s, 1, 10);
			s++;
		}

		// Print message string
		while(*c != '\0')
		{
			// Print characters that are not format specifiers
			if(*c != '%')
			{
				HAL_UART_Transmit(huart, (uint8_t*)c, 1, 10);
				c++;
				continue;
			}

			c++;

			// End loop if the end of the string is detected
			if(*c == '\0')
			{
				break;
			}

			char str[12];
			s = (uint8_t*)&str[0];

			switch(*c)
			{
				// Convert next argument to a decimal string
				case 'd':
					sprintf(str, "%d", va_arg(lst, int));
					break;
				// Convert next argument to a hexadecimal string, and add the "0x" prefix
				case 'x':
					HAL_UART_Transmit(huart, (uint8_t*)DBG_HEX_PREFIX, 2, 10);
					sprintf(str, "%X", va_arg(lst, int));
					break;
				// Add more cases here (%f, %c, etc.)
			}

			// Print formated string
			while(*s != '\0'){
				HAL_UART_Transmit(huart, s, 1, 10);
				s++;
			}
			c++;
		}

		// Add newln character once message is printed
		HAL_UART_Transmit(huart, (uint8_t*)DBG_NEWLN, 1, 10);

	#endif
}
