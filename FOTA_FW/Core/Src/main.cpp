/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw.h"
#include "string"
#include "cstring"
#include "define.h"

using namespace std;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

Sim7600 Sim7600( &huart1 ) ;
Terminal Terminal( &huart2 ) ;

struct HFR_strTypeDef {
	char startCode[2] = {':'} ;
	char byteCount[3] = {0} ;
	char address[5] = {0} ;
	char recordType[3] = {0} ;
	char data[33] = {0} ;
	char checkSum[3] = {0} ;

};


struct HFR_hexArrTypeDef {
	uint8_t startCode[2] = {':'} ;
	uint8_t byteCount[2] = {0} ;
	uint8_t address[3] = {0} ;
	uint8_t recordType[2] = {0} ;
	uint8_t data[17] = {0} ;
	uint8_t checkSum[2] = {0} ;

};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) ;

uint8_t parseToHex( char c0, char c1 ) ;

HFR_strTypeDef getStrRecord( int _dir ) ;

HFR_hexArrTypeDef parse_strToHexArrRecord( HFR_strTypeDef _strRecord ) ;

int checkRecord( HFR_strTypeDef _strRecord ) ;

bool checkAllRecordData() ;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	SCB->VTOR = (uint32_t)0x08040000;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  Sim7600.init() ;
  Terminal.init() ;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// First config
  Sim7600.sendCmd( "AT", "OK", 200 ) ;
  Sim7600.sendCmd( "AT+FSCD=E:", "OK", 200 ) ;
  Sim7600.sendCmd( "AT+CATR=1", "OK", 200 ) ;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if( Sim7600.rxDone_Flag == true ) {
//		  Terminal.println( Sim7600.rxData.c_str() ) ;
		  Sim7600.memreset() ;
	  }

	  if( HAL_GPIO_ReadPin( user_btn_GPIO_Port, user_btn_Pin ) == 0 ) {
		  HAL_Delay( 200 ) ;
		  Terminal.println( "Now check all record data... " ) ;
		  if( checkAllRecordData() == true )
			  Terminal.println( "... Done. Checking record data successfull!" ) ;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



/*____________________________________________________________________________________________________________________________________________*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if( huart->Instance == USART1 )
		Sim7600.irqProcess() ;
	else if( huart->Instance == USART2 )
		Terminal.irqProcess() ;
}



/*____________________________________________________________________________________________________________________________________________*/
HFR_strTypeDef getStrRecord( int _dir ) {
	static HFR_strTypeDef record ;
	record = {0,0,0,0,0,0} ;

	static unsigned int nowLocation = 0 ;
	static unsigned int nextLocation = 0 ;

	if( _dir == 0 ) {
		nowLocation = 0 ;
		nextLocation = 0 ;
	}
	else if( _dir == 1 ) {}
	else if( _dir == 2 ) nowLocation = nextLocation ;

	// Tạo câu lệnh CMD để lấy dữ liệu recode thứ location
	char cmd[ strlen( FIRMWARE_PATH ) + 50 ] = {0} ;
	sprintf( cmd, "AT+CFTRANTX=\"%s\",%d,100", FIRMWARE_PATH, nowLocation ) ; // SỐ 50 trong string Cmd là mỗi lần kiểm tra record, sẽ lấy 50 ký tự
	Sim7600.sendCmd( cmd, "OK", 2000 ) ;

	// Eg:
	//	AT+CFTRANTX="E:/Bootloader_FW.hex",0,15
	//	+CFTRANTX: DATA,015
	//	:020000040800F2
	//
	//	+CFTRANTX: 0
	//
	//	OK
	char str1[ strlen( strstr( Sim7600.rxData.c_str(), "DATA" ) ) ] = {0} ;
	strcpy( str1, strstr( Sim7600.rxData.c_str(), "DATA" ) ) ;

	// Eg:
	//	DATA,015
	//	:020000040800F2
	//
	//	+CFTRANTX: 0
	//
	//	OK

	char str2[ strlen( strstr( str1, ":" ) ) ] = {0} ;
	strcpy( str2, strstr( str1, ":" ) ) ;

	// Eg:
	//	:020000040800F2
	//
	//	+CFTRANTX: 0
	//
	//	OK

	// Xác định nextLocation trước đã
	int x = 0 ;
	while(1) {
		x++ ;
		nextLocation++ ;
		if( str2[ x ] == ':' ) break ;
	}

	// Xác định Nội dung sẽ trả về


	// get start code
	record.startCode[0] = str2[0] ;

	// get byte count
	record.byteCount[0] = str2[1] ;
	record.byteCount[1] = str2[2] ;

	// get address
	record.address[0] = str2[3] ;
	record.address[1] = str2[4] ;
	record.address[2] = str2[5] ;
	record.address[3] = str2[6] ;

	// get record type
	record.recordType[0] = str2[7] ;
	record.recordType[1] = str2[8] ;

	// Nếu record type mà bằng 0x01 => đã đến line cuối. Reset lại location, tránh bị lỗi data.
	if( parseToHex( record.recordType[0], record.recordType[1] ) == 0x01 ) {
		nowLocation = 0 ;
		nextLocation = 0 ;
	}

	// get data
	int data_len = parseToHex( record.byteCount[0], record.byteCount[1] ) ;
	for( int i = 0 ; i < data_len*2 ; i++ ) {
		record.data[i] = str2[ i + 9 ] ;
	}

	// get checksum
	record.checkSum[0] = str2[ data_len*2 + 9 ] ;
	record.checkSum[1] = str2[ data_len*2 + 10 ] ;

	return record ;
}



/*____________________________________________________________________________________________________________________________________________*/
/*
 * parse from 2 character type 'A' and 'B' to 0xAB
 */
uint8_t parseToHex( char c0, char c1 ){
	uint8_t result = 0x00 ;
    if ('0' <= c0 && c0 <= '9') result = c0 - '0';
    if ('a' <= c0 && c0 <= 'f') result = 10 + c0 - 'a';
    if ('A' <= c0 && c0 <= 'F') result = 10 + c0 - 'A';

    result <<= 4 ;

    if ('0' <= c1 && c1 <= '9') result |= c1 - '0';
    if ('a' <= c1 && c1 <= 'f') result |= 10 + c1 - 'a';
    if ('A' <= c1 && c1 <= 'F') result |= 10 + c1 - 'A';

    return result ;

}



/*____________________________________________________________________________________________________________________________________________*/
HFR_hexArrTypeDef parse_strToHexArrRecord( HFR_strTypeDef _strRecord ){
	HFR_hexArrTypeDef result = { 0, 0, 0, 0, 0, 0 };
	result = { 0, 0, 0, 0, 0, 0 };

	// get start code
	result.startCode[0]  = _strRecord.startCode[0] ;
	// get byte count
	result.byteCount[0]  = (uint8_t)parseToHex( _strRecord.byteCount[0], _strRecord.byteCount[1] ) ;
	// get address
	result.address[0]    = (uint8_t)parseToHex( _strRecord.address[0], _strRecord.address[1] ) ;
	result.address[1]    = (uint8_t)parseToHex( _strRecord.address[2], _strRecord.address[3] ) ;
	// get record type
	result.recordType[0] = (uint8_t)parseToHex( _strRecord.recordType[0], _strRecord.recordType[1] ) ;
	// get data
	for( int i = 0; i < (int)result.byteCount[0]; i++ )
		result.data[ i ] = (uint8_t)parseToHex( _strRecord.data[ i*2 ], _strRecord.data[ i*2 + 1 ] ) ;
	// get checksum
	result.checkSum[0]   = (uint8_t)parseToHex( _strRecord.checkSum[0], _strRecord.checkSum[1] ) ;

	return result ;
}



/*____________________________________________________________________________________________________________________________________________*/
int checkRecord( HFR_strTypeDef _strRecord ){
	// Đầu tiên, cần convert record dạng string về dạng mảng hexa
	HFR_hexArrTypeDef hexArrRecord = parse_strToHexArrRecord( _strRecord ) ;

	uint8_t sum = 0x00 ;
	sum += hexArrRecord.byteCount[0] ;
	sum += hexArrRecord.address[0] ;
	sum += hexArrRecord.address[1] ;
	sum += hexArrRecord.recordType[0] ;
	// Nếu bytecount = 0  thì nghĩa là đã tới cuối line record
	if( hexArrRecord.recordType[0] == 0x01 ) return 2 ;

	for( uint8_t i = 0; i < hexArrRecord.byteCount[0]; i++ )
		sum += hexArrRecord.data[i] ;

	sum = ~sum ;
	sum = sum + 0x01 ;

	if( sum == hexArrRecord.checkSum[0] )
		return 1 ;
	else
		return 0 ;

}



bool checkAllRecordData() {
	getStrRecord(0) ;
	HFR_strTypeDef strRecord ;
	int result = 1 ;
	int lastTime = HAL_GetTick() ;
	while(1) {
		strRecord = getStrRecord(2) ;
		result = checkRecord( strRecord ) ;
		// nếu = 0 thì đã bị lỗi
		if( result == 0 ) {
			Terminal.println( "Record ERROR at line address: " ) ;
			Terminal.print( strRecord.address ) ;
			return false ;
		// Kiểm tra record line cuối cùng ?
		}else if( result == 2 )
			break ;
		// Còn nếu == 1 thì có nghĩa là record line này đã đúng, chuyển sang line mới
		else if( result == 1 ) {
//			Terminal.println( strRecord.startCode ) ;
//			Terminal.print( strRecord.byteCount ) ;
//			Terminal.print( strRecord.address ) ;
//			Terminal.print( strRecord.recordType ) ;
//			Terminal.print( strRecord.data ) ;
//			Terminal.print( strRecord.checkSum ) ;
//			Terminal.print( " -> TRUE" ) ;
		}

		if( HAL_GetTick() - lastTime >= 20 ) {
			lastTime = HAL_GetTick() ;
			HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;
		}
	}
	return true ;
}



/*____________________________________________________________________________________________________________________________________________*/
bool writeRecordToFlash( HFR_strTypeDef _strRecord ){
	bool result = true ;
	// Đầu tiên, cần convert record dạng string về dạng mảng hexa
	HFR_hexArrTypeDef dataArr = parse_strToHexArrRecord( _strRecord ) ;


	return result ;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
