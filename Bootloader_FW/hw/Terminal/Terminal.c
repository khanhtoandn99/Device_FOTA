/*
 * Terminal.c
 *
 *  Created on: Jul 1, 2021
 *      Author: ASUS
 */

#include "hw.h"

_Bool terminal_rxDone_Flag = 0 ;
static char terminal_rxData[ TERMINAL_RXDATA_MAX ] = {0} ;
char rxByte[1] = {0} ;
unsigned int rxCnt = 0 ;

void terminal_init() {
	terminal_memreset() ;
	HAL_UART_Receive_IT( &TERMINAL_TXRX, (uint8_t*)rxByte, 1 ) ;
}



void terminal_irqProcess() {
	if(  rxByte[0] == '\r' ){
		terminal_rxDone_Flag = 1 ;
		memset( rxByte, 0, 1 ) ;
	}else{
		terminal_rxData[ rxCnt ] = rxByte[0] ;
		if ( rxCnt < TERMINAL_RXDATA_MAX ) rxCnt++ ;
	}
	HAL_UART_Receive_IT( &TERMINAL_TXRX, (uint8_t*)rxByte, 1 ) ;
}



void terminal_memreset() {
    rxCnt = 0 ;
    terminal_rxDone_Flag = 0 ;
    memset( terminal_rxData, 0, TERMINAL_RXDATA_MAX );
}



_Bool terminal_available() {
	return terminal_rxDone_Flag ;
}



char* terminal_read() {
	return terminal_rxData ;
}



void terminal_print( const char* _info, unsigned int _timeout ) {
	HAL_UART_Transmit( &TERMINAL_TXRX, (uint8_t*)_info, strlen( _info ), _timeout ) ;
}



void terminal_println( const char* _info, unsigned int _timeout ) {
	char info[ strlen( _info ) + 2 ]  ;
	memset( info, 0, strlen( _info ) + 2 ) ;
	sprintf( info, "\n%s", _info ) ;

	HAL_UART_Transmit( &TERMINAL_TXRX, (uint8_t*)info, strlen( info ), _timeout ) ;
}
