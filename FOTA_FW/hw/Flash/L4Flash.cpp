/*
 * Flash.c
 *
 *  Created on: Jul 1, 2021
 *      Author: ASUS
 */

#include <hw.h>


/*_________________________________________________________________________________________________________________________________*/
void Flash_erase( uint32_t _page ) {
	FLASH_EraseInitTypeDef EraseStruct ;
	static uint32_t PageEraseERROR_Addr  ;
	HAL_FLASH_Unlock() ;

	EraseStruct.Banks = FLASH_BANK_BOTH ;
	EraseStruct.NbPages = 1 ;
	EraseStruct.TypeErase = FLASH_TYPEERASE_PAGES ;
	EraseStruct.Page = _page ;

	if( HAL_FLASHEx_Erase( &EraseStruct, &PageEraseERROR_Addr ) != HAL_OK )
		return (void)HAL_FLASH_GetError() ;

	HAL_FLASH_Lock() ;
}



/*_________________________________________________________________________________________________________________________________*/
void  Flash_write( uint32_t _address, const uint8_t* _data, int _dataLength ) {
	HAL_FLASH_Unlock() ;

	// round to double word ( 8 byte )
	unsigned int dwLen = 0 ;
	if( ( _dataLength % 8 == 0 ) && ( _dataLength >= 8 ) ) dwLen = _dataLength ;
	else if( ( _dataLength % 8 > 0 ) && ( _dataLength > 8 ) ) dwLen = _dataLength + ( 8 - ( _dataLength % 8 ) ) ;
	else if( _dataLength < 8 ) dwLen = _dataLength + ( 8 - ( _dataLength % 8 ) ) ;

	uint8_t dataArr[ dwLen + 1 ] ;
	memset( (char*)dataArr, 0xFF, _dataLength + 1 ) ;
	memcpy( (char*)dataArr, (char*)_data, _dataLength ) ;

	uint64_t data = 0x0000000000000000 ;

	for( unsigned int i = 0; i < dwLen; i += 8 ) {
		data |= dataArr[ i + 7 ] ; data <<= 8 ;
		data |= dataArr[ i + 6 ] ; data <<= 8 ;
		data |= dataArr[ i + 5 ] ; data <<= 8 ;
		data |= dataArr[ i + 4 ] ; data <<= 8 ;
		data |= dataArr[ i + 3 ] ; data <<= 8 ;
		data |= dataArr[ i + 2 ] ; data <<= 8 ;
		data |= dataArr[ i + 1 ] ; data <<= 8 ;
		data |= dataArr[ i + 0 ] ;

		HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, _address + i, data ) ;
		data = 0x0000000000000000 ;
	}


	HAL_FLASH_Lock() ;
}



/*_________________________________________________________________________________________________________________________________*/
void  Flash_write_doubleWord( uint32_t _address, const uint8_t* _data ) {
	HAL_FLASH_Unlock() ;

	uint8_t dataArr[9] ;
	memset( (char*)dataArr, 0x00, 9 ) ;
	memcpy( (char*)dataArr, (char*)_data, 8 ) ;

	uint64_t data = 0x00 ;

	data |= dataArr[7] ; data <<= 8 ;
	data |= dataArr[6] ; data <<= 8 ;
	data |= dataArr[5] ; data <<= 8 ;
	data |= dataArr[4] ; data <<= 8 ;
	data |= dataArr[3] ; data <<= 8 ;
	data |= dataArr[2] ; data <<= 8 ;
	data |= dataArr[1] ; data <<= 8 ;
	data |= dataArr[0] ;

	HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, _address, data ) ;

	HAL_FLASH_Lock() ;
}



/*_________________________________________________________________________________________________________________________________*/
char* Flash_read_doubleWord( uint64_t _Address ) {
	static char result[ 8 + 1 ] ;
	char temp[8] = {0} ;
	memset( result, 0x00 , 8 + 1 ) ;
	memset( temp, 0xFF , 8 ) ;
	uint64_t data[1] ;
	*data = *(__IO uint64_t *)_Address ;

	for( int i = 0; i < 8; i++ ){
		temp[ 7-i ] &= data[0] ;
		data[0] >>= 8 ;
	}

	memcpy( result, temp, 8 ) ;
	return result ;
}



/*_________________________________________________________________________________________________________________________________*/
void  Flash_read_str( uint64_t _fromAddress, uint64_t _toAddress, char* _result ) {
	unsigned int size = ( unsigned int )( _toAddress - _fromAddress )  ;
	char result[ size + 1 ] ;
	memset( result, 0, size ) ;

	for( unsigned int i = 0; i < size; i += 8 ) {
		strcat( result, Flash_read_doubleWord( _fromAddress + i ) ) ;
	}

	memcpy( _result, result, strlen( result ) ) ;
}
