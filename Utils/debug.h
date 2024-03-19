/*
 * debug.h
 *
 *  Created on: Feb 27, 2024
 *      Author: Admin
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include <stdarg.h>
#include "uartstdio.h"

#define UART_DEBUG 3

#if defined(UART_DEBUG) && UART_DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) UARTprintf("DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif



#endif /* DEBUG_H_ */
