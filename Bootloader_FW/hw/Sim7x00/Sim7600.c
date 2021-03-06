/*
 * Sim7600.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: ASUS
 */

#include "hw.h"

_Bool sim_rxDone_Flag = 0 ;
static char sim_rxData[ SIM_RXDATA_MAX ] = {0} ;
char sim_rxByte[ 1 ] = {0} ;
unsigned int sim_rxCnt = 0 ;



void sim7600_init() {
	sim7600_memreset() ;
	HAL_UART_Receive_IT( &SIM_UART_TXRX, (uint8_t*)sim_rxByte, 1 ) ;
}



void sim7600_irqProcess() {
	if(  sim_rxByte[0] == '\r' ){
		sim_rxDone_Flag = 1 ;
		memset( sim_rxByte, 0, 1 ) ;
	}else{
		sim_rxData[ sim_rxCnt ] = sim_rxByte[0] ;
		if ( sim_rxCnt < SIM_RXDATA_MAX ) sim_rxCnt++ ;
	}
	HAL_UART_Receive_IT( &SIM_UART_TXRX, (uint8_t*)sim_rxByte, 1 ) ;
}



_Bool sim7600_sendCmd( const char* _cmd, const char* _expectedAnswer, unsigned int _timeOut ) {
	_Bool answer=0;

    sim7600_memreset() ;

    HAL_UART_Transmit( &SIM_UART_DEBUG, (uint8_t*)("\n@sim7600e: "), strlen ("\n@sim7600e: "), _timeOut) ;
    HAL_UART_Transmit( &SIM_UART_DEBUG, (uint8_t*)_cmd, strlen (_cmd), _timeOut) ;
    HAL_UART_Transmit( &SIM_UART_DEBUG, (uint8_t*)("\n"), strlen ("\n"), _timeOut) ;

    // Bắt đầu gửi lệnh AT ngay tại đây
    HAL_UART_Transmit( &SIM_UART_TXRX, (uint8_t*)_cmd, strlen (_cmd), _timeOut) ;

    // lệnh này để bắt đầu lấy mốc t.g để check Timeout
    uint32_t tickStart = HAL_GetTick() ;


    while( answer == 0 ) {
    	// Kiểm tra thời gian phản hồi, nếu lâu quá timeout thì break, và báo lỗi
		if ( (unsigned long)( HAL_GetTick() - tickStart )  >=  _timeOut )
			break ;

		// Nếu có thi la OK, answer == 1, thoát khỏi vòng while
		if ( strstr( sim_rxData, _expectedAnswer) != NULL )
			answer = 1;
	}

    // In thông tin mà module sim đã phản hồi về :
    HAL_UART_Transmit( &SIM_UART_DEBUG, (uint8_t*)sim_rxData, strlen( sim_rxData ), _timeOut) ;
    HAL_UART_Transmit( &SIM_UART_DEBUG, (uint8_t*)("\n"), strlen ("\n"), _timeOut) ;

    return answer;
}



void sim7600_memreset() {
    sim_rxCnt = 0 ;
    sim_rxDone_Flag = 0 ;
    memset( sim_rxData, 0, SIM_RXDATA_MAX );
}



_Bool sim7600_available() {
	return sim_rxDone_Flag ;
}



char* sim7600_read() {
	static char simRead[ SIM_RXDATA_MAX ] = {0} ;
	memset( simRead, 0 , SIM_RXDATA_MAX ) ;
	memcpy( simRead, sim_rxData, SIM_RXDATA_MAX ) ;
	return simRead ;
}
