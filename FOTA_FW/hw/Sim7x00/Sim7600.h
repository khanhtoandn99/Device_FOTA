/*
 * Sim7600.h
 *
 *  Created on: Jul 3, 2021
 *      Author: ASUS
 */

#ifndef SIM7X00_SIM7600_H_
#define SIM7X00_SIM7600_H_

#include "stm32l4xx_hal.h"
#include "string"
#include "cstring"

using namespace std ;


class Sim7600 {
public:
	Sim7600( UART_HandleTypeDef * _huartx );
	virtual ~Sim7600();

	/*___________________*/
	string rxData ;
	char rxByte[1] = {0} ;
	bool rxDone_Flag = false ;

	/*___________________*/

	void    init() ;
	void    irqProcess() ;
	bool    sendCmd( const char* _cmd, const char* _expectedAnswer, unsigned int _timeout ) ;
	void    memreset() ;


private:
	UART_HandleTypeDef *huartx ;
};

#endif /* SIM7X00_SIM7600_H_ */
