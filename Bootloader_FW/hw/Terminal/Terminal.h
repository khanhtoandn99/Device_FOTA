/*
 * Terminal.h
 *
 *  Created on: Jul 1, 2021
 *      Author: ASUS
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_


#define TERMINAL_TXRX huart2

#define TERMINAL_RXDATA_MAX 1000


void  terminal_init() ;

void  terminal_irqProcess() ;

void  terminal_memreset() ;

_Bool terminal_available() ;

char*  terminal_read() ;

void  terminal_print( const char* _info, unsigned int _timeout ) ;

void  terminal_println( const char* _info, unsigned int _timeout ) ;


#endif /* TERMINAL_H_ */
