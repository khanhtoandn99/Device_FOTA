///*
// * hexFile_functions.cpp
// *
// *  Created on: Jul 7, 2021
// *      Author: ASUS
// */
//
//#include "string"
//#include "cstring"
//#include "define.h"
//#include "hw.h"
//
//using namespace std;
//
//
//struct HFR_strTypeDef {
//	char startCode[2] = {':'} ;
//	char byteCount[3] = {0} ;
//	char address[5] = {0} ;
//	char recordType[3] = {0} ;
//	char data[33] = {0} ;
//	char checkSum[3] = {0} ;
//
//};
//
//
//struct HFR_hexArrTypeDef {
//	uint8_t startCode[2] = {':'} ;
//	uint8_t byteCount[2] = {0} ;
//	uint8_t address[3] = {0} ;
//	uint8_t recordType[2] = {0} ;
//	uint8_t data[17] = {0} ;
//	uint8_t checkSum[2] = {0} ;
//
//};
//
//
//Sim7600 Sim7600( &huart1 ) ;
//Terminal Terminal( &huart2 ) ;
//
//uint8_t parseToHex( char c0, char c1 ) ;
//
//HFR_strTypeDef getStrRecord( int _dir ) ;
//
//HFR_hexArrTypeDef parse_strToHexArrRecord( HFR_strTypeDef _strRecord ) ;
//
//int checkRecord( HFR_strTypeDef _strRecord ) ;
//
//bool checkAllRecordData() ;
//
//bool writeFirmwareToFLASH() ;
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//HFR_strTypeDef getStrRecord( int _dir ) {
//	static HFR_strTypeDef record ;
//	record = {0,0,0,0,0,0} ;
//
//	static unsigned int nowLocation = 0 ;
//	static unsigned int nextLocation = 0 ;
//
//	if( _dir == 0 ) {
//		nowLocation = 0 ;
//		nextLocation = 0 ;
//	}
//	else if( _dir == 1 ) {}
//	else if( _dir == 2 ) nowLocation = nextLocation ;
//
//	// Tạo câu lệnh CMD để lấy dữ liệu recode thứ location
//	char cmd[ strlen( FIRMWARE_PATH ) + 50 ] = {0} ;
//	sprintf( cmd, "AT+CFTRANTX=\"%s\",%d,100", FIRMWARE_PATH, nowLocation ) ; // SỐ 50 trong string Cmd là mỗi lần kiểm tra record, sẽ lấy 50 ký tự
//	Sim7600.sendCmd( cmd, "OK", 2000 ) ;
//
//	// Eg:
//	//	AT+CFTRANTX="E:/Bootloader_FW.hex",0,15
//	//	+CFTRANTX: DATA,015
//	//	:020000040800F2
//	//
//	//	+CFTRANTX: 0
//	//
//	//	OK
//	char str1[ strlen( strstr( Sim7600.rxData.c_str(), "DATA" ) ) ] = {0} ;
//	strcpy( str1, strstr( Sim7600.rxData.c_str(), "DATA" ) ) ;
//
//	// Eg:
//	//	DATA,015
//	//	:020000040800F2
//	//
//	//	+CFTRANTX: 0
//	//
//	//	OK
//
//	char str2[ strlen( strstr( str1, ":" ) ) ] = {0} ;
//	strcpy( str2, strstr( str1, ":" ) ) ;
//
//	// Eg:
//	//	:020000040800F2
//	//
//	//	+CFTRANTX: 0
//	//
//	//	OK
//
//	// Xác định nextLocation trước đã
//	int x = 0 ;
//	while(1) {
//		x++ ;
//		nextLocation++ ;
//		if( str2[ x ] == ':' ) break ;
//	}
//
//	// Xác định Nội dung sẽ trả về
//
//
//	// get start code
//	record.startCode[0] = str2[0] ;
//
//	// get byte count
//	record.byteCount[0] = str2[1] ;
//	record.byteCount[1] = str2[2] ;
//
//	// get address
//	record.address[0] = str2[3] ;
//	record.address[1] = str2[4] ;
//	record.address[2] = str2[5] ;
//	record.address[3] = str2[6] ;
//
//	// get record type
//	record.recordType[0] = str2[7] ;
//	record.recordType[1] = str2[8] ;
//
//	// Nếu record type mà bằng 0x01 => đã đến line cuối. Reset lại location, tránh bị lỗi data.
//	if( parseToHex( record.recordType[0], record.recordType[1] ) == 0x01 ) {
//		nowLocation = 0 ;
//		nextLocation = 0 ;
//	}
//
//	// get data
//	int data_len = parseToHex( record.byteCount[0], record.byteCount[1] ) ;
//	for( int i = 0 ; i < data_len*2 ; i++ ) {
//		record.data[i] = str2[ i + 9 ] ;
//	}
//
//	// get checksum
//	record.checkSum[0] = str2[ data_len*2 + 9 ] ;
//	record.checkSum[1] = str2[ data_len*2 + 10 ] ;
//
//	return record ;
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
///*
// * parse from 2 character type 'A' and 'B' to 0xAB
// */
//uint8_t parseToHex( char c0, char c1 ){
//	uint8_t result = 0x00 ;
//    if ('0' <= c0 && c0 <= '9') result = c0 - '0';
//    if ('a' <= c0 && c0 <= 'f') result = 10 + c0 - 'a';
//    if ('A' <= c0 && c0 <= 'F') result = 10 + c0 - 'A';
//
//    result <<= 4 ;
//
//    if ('0' <= c1 && c1 <= '9') result |= c1 - '0';
//    if ('a' <= c1 && c1 <= 'f') result |= 10 + c1 - 'a';
//    if ('A' <= c1 && c1 <= 'F') result |= 10 + c1 - 'A';
//
//    return result ;
//
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//HFR_hexArrTypeDef parse_strToHexArrRecord( HFR_strTypeDef _strRecord ){
//	HFR_hexArrTypeDef result = { 0, 0, 0, 0, 0, 0 };
//	result = { 0, 0, 0, 0, 0, 0 };
//
//	// get start code
//	result.startCode[0]  = _strRecord.startCode[0] ;
//	// get byte count
//	result.byteCount[0]  = (uint8_t)parseToHex( _strRecord.byteCount[0], _strRecord.byteCount[1] ) ;
//	// get address
//	result.address[0]    = (uint8_t)parseToHex( _strRecord.address[0], _strRecord.address[1] ) ;
//	result.address[1]    = (uint8_t)parseToHex( _strRecord.address[2], _strRecord.address[3] ) ;
//	// get record type
//	result.recordType[0] = (uint8_t)parseToHex( _strRecord.recordType[0], _strRecord.recordType[1] ) ;
//	// get data
//	for( int i = 0; i < (int)result.byteCount[0]; i++ )
//		result.data[ i ] = (uint8_t)parseToHex( _strRecord.data[ i*2 ], _strRecord.data[ i*2 + 1 ] ) ;
//	// get checksum
//	result.checkSum[0]   = (uint8_t)parseToHex( _strRecord.checkSum[0], _strRecord.checkSum[1] ) ;
//
//	return result ;
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//int checkRecord( HFR_strTypeDef _strRecord ){
//	// Đầu tiên, cần convert record dạng string về dạng mảng hexa
//	HFR_hexArrTypeDef hexArrRecord = parse_strToHexArrRecord( _strRecord ) ;
//
//	uint8_t sum = 0x00 ;
//	sum += hexArrRecord.byteCount[0] ;
//	sum += hexArrRecord.address[0] ;
//	sum += hexArrRecord.address[1] ;
//	sum += hexArrRecord.recordType[0] ;
//	// Nếu bytecount = 0  thì nghĩa là đã tới cuối line record
//	if( hexArrRecord.recordType[0] == 0x01 ) return 2 ;
//
//	for( uint8_t i = 0; i < hexArrRecord.byteCount[0]; i++ )
//		sum += hexArrRecord.data[i] ;
//
//	sum = ~sum ;
//	sum = sum + 0x01 ;
//
//	if( sum == hexArrRecord.checkSum[0] )
//		return 1 ;
//	else
//		return 0 ;
//
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//bool checkAllRecordData() {
//	getStrRecord(0) ;
//	HFR_strTypeDef strRecord ;
//	int result = 1 ;
//	int lastTime = HAL_GetTick() ;
//	while(1) {
//		strRecord = getStrRecord(2) ;
//		result = checkRecord( strRecord ) ;
//		// nếu = 0 thì đã bị lỗi
//		if( result == 0 ) {
//			Terminal.println( "Record ERROR at line address: " ) ;
//			Terminal.print( strRecord.address ) ;
//			return false ;
//		// Kiểm tra record line cuối cùng ?
//		}else if( result == 2 )
//			break ;
//		// Còn nếu == 1 thì có nghĩa là record line này đã đúng, chuyển sang line mới
//		else if( result == 1 ) {
////			Terminal.println( strRecord.startCode ) ;
////			Terminal.print( strRecord.byteCount ) ;
////			Terminal.print( strRecord.address ) ;
////			Terminal.print( strRecord.recordType ) ;
////			Terminal.print( strRecord.data ) ;
////			Terminal.print( strRecord.checkSum ) ;
////			Terminal.print( " -> TRUE" ) ;
//		}
//
//		if( HAL_GetTick() - lastTime >= 20 ) {
//			lastTime = HAL_GetTick() ;
//			HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;
//		}
//	}
//	return true ;
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//bool writeRecordToFlash( HFR_strTypeDef _strRecord ){
//	bool result = true ;
//
//
//	return result ;
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
//bool writeFirmwareToFLASH() {
//	// đầu tiên, xác định địa chỉ base address
//	uint32_t baseAddr = 0x00 ;
//	HFR_hexArrTypeDef ha_firstRecord = parse_strToHexArrRecord( getStrRecord(0) ) ;
//	if( ha_firstRecord.recordType[0] == 0x04 ) {
//		for( int i = 0 ; i < (int)ha_firstRecord.byteCount[0] ; i++ ) {
//			baseAddr <<= 8 ;
//			baseAddr |= ha_firstRecord.data[i] ;
//		}
//		baseAddr <<= 8*( 4 - ha_firstRecord.byteCount[0] ) ;
//	}
//
//	// Xác định page address bắt đầu xóa
//
//
//	// Bắt đầu ghi
//	HFR_hexArrTypeDef ha_wRecord = { 0, 0, 0, 0, 0, 0 } ;
//	uint32_t flashAddr = baseAddr + 0x00 ;
//	int lastTime = HAL_GetTick() ;
//	bool isDone = false ;
//
//	while(1) {
//		flashAddr = 0x00 ;
//		ha_wRecord = { 0, 0, 0, 0, 0, 0 } ;
//		ha_wRecord = parse_strToHexArrRecord( getStrRecord(2) ) ;
//
//		// kiểm tra đã write đến dòng cuối cùng hay chưa ?
//		if( (ha_wRecord.recordType[0] == 0x01)
//				&& (ha_wRecord.byteCount[0] == 0x00)
//				&& (( ha_wRecord.address[0] + ha_wRecord.address[1] ) == 0x00) ) {
//			Terminal.println( "...100%... Done!" ) ;
//			isDone = true ;
//			break ;
//		}
//
//		// kiểm tra có phải record cho phép ghi dữ liệu ko
//		if( (ha_wRecord.byteCount[0] != 0x00) && (ha_wRecord.recordType[0] == 0x00) ) {
//			// vd : 0x08000000
//			flashAddr |= ha_wRecord.address[0] ;
//			flashAddr <<= 8 ;
//			flashAddr |= ha_wRecord.address[1] ;
//			flashAddr += baseAddr ;
//
//
//			Flash_write( flashAddr, (uint8_t*)ha_wRecord.data, (int)ha_wRecord.byteCount[0] ) ;
//
//		}
//
//		if( flashAddr == 0x08010C60 )
//			Terminal.println( "...10%..." ) ;
//		else if( flashAddr == 0x080118E0 )
//			Terminal.println( "...20%..." ) ;
//		else if( flashAddr == 0x08012560 )
//			Terminal.println( "...30%..." ) ;
//		else if( flashAddr == 0x080131E0 )
//			Terminal.println( "...40%..." ) ;
//		else if( flashAddr == 0x08013E60 )
//			Terminal.println( "...50%..." ) ;
//		else if( flashAddr == 0x08014AE0 )
//			Terminal.println( "...60%..." ) ;
//		else if( flashAddr == 0x08015760 )
//			Terminal.println( "...70%..." ) ;
//		else if( flashAddr == 0x080163E0 )
//			Terminal.println( "...80%..." ) ;
//		else if( flashAddr == 0x080176A0 )
//			Terminal.println( "...90%..." ) ;
//
//		if( HAL_GetTick() - lastTime >= 10 ) {
//			lastTime = HAL_GetTick() ;
//			HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;
//		}
//
//
//	}
//	if( isDone == true )
//		return true ;
//	else
//		return false ;
//}
//
//
//
///*____________________________________________________________________________________________________________________________________________*/
