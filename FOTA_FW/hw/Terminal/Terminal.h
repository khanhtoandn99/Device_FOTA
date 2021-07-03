/*
 * Terminal.h
 *
 *  Created on: Jul 3, 2021
 *      Author: ASUS
 */

#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_

#include "stm32l4xx_hal.h"
#include "cstring"

class Terminal {
public:
	Terminal( UART_HandleTypeDef * _huartx );
	virtual ~Terminal();

	/*___________________________*/
	string rxData ;
	char rxByte[1] = {0} ;
	bool rxDone_Flag = false ;

	/*___________________________*/
	void init() ;
	void irqProcess() ;
	void print( const char* _info ) ;
	void println( const char* _info ) ;

private:
	UART_HandleTypeDef *huartx ;
};

#endif /* TERMINAL_TERMINAL_H_ */
