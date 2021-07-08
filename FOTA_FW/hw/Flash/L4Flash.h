/*
 * L4Flash.h
 *
 *  Created on: Jul 2, 2021
 *      Author: ASUS
 */

#ifndef FLASH_L4FLASH_H_
#define FLASH_L4FLASH_H_

#include "stm32l4xx_hal.h"
#include "stdint.h"
#include "string.h"


void  Flash_erase( uint32_t _page ) ;

void  Flash_write_doubleWord( uint32_t _address, const uint8_t* _data ) ;

void  Flash_write( uint32_t _address, const uint8_t* _data, int _dataLength ) ;

char* Flash_read_doubleWord( uint64_t _Address ) ;

void  Flash_read_str( uint64_t _fromAddress, uint64_t _toAddress, char* _result ) ;



#endif /* FLASH_L4FLASH_H_ */
