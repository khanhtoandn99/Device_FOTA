/*
 * Flash.c
 *
 *  Created on: Jul 1, 2021
 *      Author: ASUS
 */

#include <hw.h>

/*_________________________________________________________________________________________________________________________________*/
void flash_testFunc() {


}



/*_________________________________________________________________________________________________________________________________*/
void Flash_erase( uint32_t _page ) {
	FLASH_EraseInitTypeDef EraseStruct ;
	static uint32_t PageEraseERROR_Addr  ;
	HAL_FLASH_Unlock() ;
	if( _page <= LAST_PAGE_ADDR_BANK1)
		EraseStruct.Banks = FLASH_BANK_1 ;
	else
		EraseStruct.Banks = FLASH_BANK_2 ;
	EraseStruct.NbPages = 1 ;
	EraseStruct.TypeErase = FLASH_TYPEERASE_PAGES ;
	EraseStruct.Page = _page ;

	if( HAL_FLASHEx_Erase( &EraseStruct, &PageEraseERROR_Addr ) != HAL_OK )
		return (void)HAL_FLASH_GetError() ;

	HAL_FLASH_Lock() ;
}



/*_________________________________________________________________________________________________________________________________*/
void Flash_write( uint32_t _address, const char* _data ) {
	HAL_FLASH_Unlock() ;

	unsigned int size = strlen( _data ) + 1 ;
	char dataArr[ size ] ;
	memset( dataArr, 0, size ) ;
	memcpy( dataArr, _data, size - 1 ) ;
	uint64_t data = 0x0000000000000000000000000000000000000000 ;

	for( int i = 0; i < size; i += 8 ){
		for( int j = 0; j < 8; j++ ){
			if( dataArr[ i + j ] == '\0' ) {
				data <<= (8-j)*8 ;
				break ;
			}else {
				data <<= 8 ;
				data |= dataArr[ i + j ] ;
			}
		}
		HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, _address + i, data ) ;
		data = 0x000000000000000000000000000000000000 ;
	}

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
void  Flash_read( uint64_t _fromAddress, uint64_t _toAddress, char* _result ) {
	unsigned int size = ( unsigned int )( _toAddress - _fromAddress )  ;
	char result[ size + 1 ] ;
	memset( result, 0, size ) ;

	for( int i = 0; i < size; i += 8 ) {
		strcat( result, Flash_read_doubleWord( _fromAddress + i ) ) ;
	}

	memcpy( _result, result, strlen( result ) ) ;
}