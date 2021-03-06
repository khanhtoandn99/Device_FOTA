/*
 * Sim7600.cpp
 *
 *  Created on: Jul 3, 2021
 *      Author: ASUS
 */

#include "hw.h"
#include "define.h"

/*____________________________________________________________________________________________________________________________________________*/
Sim7600::Sim7600( UART_HandleTypeDef * _huartx ) {
	huartx = &*_huartx ;
}



/*____________________________________________________________________________________________________________________________________________*/
Sim7600::~Sim7600() {

}



/*____________________________________________________________________________________________________________________________________________*/
void Sim7600::init() {
	HAL_UART_Receive_IT( huartx, (uint8_t*)rxByte, 1 ) ;
}



/*____________________________________________________________________________________________________________________________________________*/
void Sim7600::irqProcess() {
	if ( rxByte[0] == '\r' )
		rxDone_Flag = true ;
	else
		rxData += rxByte[0] ;

	HAL_UART_Receive_IT( huartx, (uint8_t*)rxByte, 1 ) ;
}



/*____________________________________________________________________________________________________________________________________________*/
void Sim7600::memreset() {
	rxDone_Flag = false ;
	rxByte[0] = 0 ;
	rxData.clear() ;
}



/*____________________________________________________________________________________________________________________________________________*/
bool Sim7600::sendCmd( const char* _cmd, const char* _expectedAnswer, unsigned int _timeout ) {
    bool answer=0;
    memreset() ;

    // Bắt đầu gửi lệnh AT ngay tại đây
    char cmd[ strlen( _cmd ) + 2 ] = {0} ;
    sprintf( cmd, "%s\r\r", _cmd ) ;
    HAL_UART_Transmit( huartx, (uint8_t*)cmd, strlen (cmd), _timeout) ;

    // lệnh này để bắt đầu lấy mốc t.g để check Timeout
    uint32_t tickStart = HAL_GetTick() ;

    while( answer == 0 ) {

    	// Kiểm tra thời gian phản hồi, nếu lâu quá timeout thì break, và báo lỗi
		if ( (unsigned long)( HAL_GetTick() - tickStart )  >=  _timeout )
			break ;

		// Nếu có thi la OK, answer == 1, thoát khỏi vòng while
		if ( strstr( rxData.c_str(), _expectedAnswer) != NULL )
			answer = 1;
	}

    HAL_Delay(10) ;
    return answer;
}














