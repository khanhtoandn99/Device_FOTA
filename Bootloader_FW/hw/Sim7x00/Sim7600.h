/*
 * Sim7600.h
 *
 *  Created on: Jun 30, 2021
 *      Author: ASUS
 */

#ifndef SIM7600_H_
#define SIM7600_H_

#include "stm32l4xx_hal.h"

#define SIM_UART_TXRX  huart1
#define SIM_UART_DEBUG huart2

#define SIM_RXDATA_MAX 1000


_Bool sim7600_available() ;

void  sim7600_init() ;

void  sim7600_irqProcess() ;

_Bool sim7600_sendCmd( const char* _cmd, const char* _expectedAnswer, unsigned int _timeOut ) ;

void  sim7600_memreset() ;

char* sim7600_read() ;


#endif /* SIM7600_H_ */
