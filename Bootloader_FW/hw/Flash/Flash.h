/*
 * Flash.h
 *
 *  Created on: Jul 1, 2021
 *      Author: ASUS
 */

#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

#include "stm32l4xx_hal.h"
#include "stdint.h"
#include "string.h"

#define    LAST_PAGE_ADDR_BANK1  0x0807F800

void  flash_testFunc() ;

void  Flash_erase( uint32_t _page ) ;

void  Flash_write( uint32_t _address, const char* _data ) ;

char* Flash_read_doubleWord( uint64_t _Address ) ;

void  Flash_read( uint64_t _fromAddress, uint64_t _toAddress, char* _result ) ;

#endif /* FLASH_FLASH_H_ */
