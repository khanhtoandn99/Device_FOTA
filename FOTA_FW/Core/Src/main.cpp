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

uint32_t appFW_BaseAddress = 0x0803E800;

static char newAppFW_name[24] = {0} ;

struct BF_dWordHArrTypeDef {
	bool isLastDWord = false ;
	uint8_t data[8] = {0} ;
};

uint8_t parseToHex( char c0, char c1 ) ;

BF_dWordHArrTypeDef CS_getDoubleWord( uint32_t _address ) ;

bool CF_writeBFFWtoFlash( uint32_t _baseAddress ) ;

char *getCurrentVersion() ;

char *getOnSimVersion() ;

bool checkFWnameFormat( char _character ) ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) ;

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
	SCB->VTOR = (uint32_t)0x08019000;
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

  Terminal.println( "----------------------------------------------------------------------------------------------------------------" ) ;
  Terminal.println( "                                             FIRMWARE OVER THE AIR UPDATE" ) ;
  Terminal.println( "----------------------------------------------------------------------------------------------------------------" ) ;
  Terminal.println( "Booting on...\nPlease wait for 30 second !" ) ;

  Sim7600.sendCmd( "AT+CRESET", "", 200 ) ;
  HAL_Delay(30000) ;

//	Flash_erase(500) ;
//	HAL_Delay(1000) ;
//	uint8_t x[24] = "datalogger_1.0.0.bin" ;
//	Flash_write( 0x080FA000, x, strlen((char*)x) ) ;

  Sim7600.sendCmd( "AT", "OK", 200 ) ;
  Sim7600.sendCmd( "AT+FSCD=E:", "OK", 200 ) ;
  Sim7600.sendCmd( "AT+CATR=1", "OK", 200 ) ;
  Sim7600.memreset() ;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// Code start here :


  char curVersionOnFLASH[24] = {0} ;
  memcpy( curVersionOnFLASH, getCurrentVersion(), 24 ) ;

  char onSimVersion[25] = {0} ;
  memcpy( onSimVersion, getOnSimVersion(), 24 ) ;

  Terminal.println( "Checking..." ) ;
  if( strstr( onSimVersion, curVersionOnFLASH ) == NULL ) {
	  // Start write firmware tai day
	  Terminal.println( "*New program version detected!\nPage ereasing... " ) ;
	  for( int i = 0; i <= 150; i++ )
		  Flash_erase( i + 125 ) ;

	  Terminal.print( "Done.\nNow flashing... " ) ;
	  if( CF_writeBFFWtoFlash( appFW_BaseAddress ) == true )
		  Terminal.println( "...Flash successful!" ) ;
	  else
		  Terminal.println( "...Flash failed! Please try again!" ) ;

	  for( int i = 0 ; i < 6 ; i++ ) {
		  HAL_GPIO_TogglePin( user_led_GPIO_Port,  user_led_Pin ) ;
		  HAL_Delay(500) ;

	  }

	  HAL_GPIO_WritePin( user_led_GPIO_Port,  user_led_Pin, (GPIO_PinState)0 ) ;

	  // lưu lại firmware hiện tại
	  Terminal.println( "Saving... " ) ;
	  char savedVersion[25] = {0} ;
	  HAL_Delay(1000) ;
	  memcpy( savedVersion, getOnSimVersion(), 24 ) ;
	  Flash_erase(500) ;
	  HAL_Delay(1000) ;
	  Flash_write( 0x080FA000, (const uint8_t*)savedVersion, strlen( savedVersion ) ) ;
	  Terminal.print( "Done" ) ;

	  Sim7600.sendCmd( "AT+CRESET", "", 100 ) ;
	  HAL_NVIC_SystemReset() ;

  }else {
	  Terminal.println( "Firmware version is up to date." ) ;
	  HAL_NVIC_SystemReset() ;
  }


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
BF_dWordHArrTypeDef CS_getDoubleWord( uint32_t _address ) {
	static BF_dWordHArrTypeDef result ;

	result.isLastDWord = false ;
	memset( result.data, 0x00, 8 ) ;

	// Tạo câu lệnh CMD để lấy dữ liệu recode thứ location
	char cmd[ strlen( newAppFW_name ) + 30 ] = {0} ;
	sprintf( cmd, "AT+CFTRANTX=\"E:/%s\",%lu,8\r\n", newAppFW_name, _address ) ;
	// không thể dùng strstr "OK"để kiểm tra Sim7600.sendCmd đúng hay không được, vì gặp ký tự NULL ( 0x00 ) là nó dừng
	// nhưng dữ liệu firmware của chúng ta chứa ký tự null là chuyện đương nhiên -> ko hợp lý . Nên chỉ gửi thôi, ko kiểm tra.
	Sim7600.sendCmd( cmd, "+CFTRANTX: DATA", 5000 ) ;

	char dataZone[36] = {0} ;
	memcpy( dataZone, strstr( Sim7600.rxData.c_str(), "DATA" ), 35 ) ;

	// Check if is last double word of .bin file
	uint8_t restNbData = parseToHex( dataZone[6], dataZone[7] ) ;
	if( strstr( dataZone, "DATA" ) == NULL )
		result.isLastDWord = true ;

	else if( restNbData < 0x08  ) {
		result.isLastDWord = true ;
		for( int i = 0 ; i < restNbData ; i++ )
			result.data[i] = dataZone[ i + 9 ] ;

	// nếu không thì cứ trả về bình thường
	}else {
		for( int i = 0 ; i < 8 ; i++ )
			result.data[i] = dataZone[ i + 9 ] ;
	}


	return (BF_dWordHArrTypeDef)result ;
}



/*____________________________________________________________________________________________________________________________________________*/
bool CF_writeBFFWtoFlash( uint32_t _baseAddress ) {
	// Before start
	Sim7600.sendCmd( "AT", "OK", 200 ) ;
	Sim7600.sendCmd( "AT+FSCD=E:", "OK", 200 ) ;
	Sim7600.sendCmd( "AT+CATR=1", "OK", 200 ) ;
	Sim7600.memreset() ;

	memset( newAppFW_name, 0, 25 ) ;
	memcpy( newAppFW_name, getOnSimVersion(), 24 ) ;

	bool result = false ;
	uint32_t address = 0x00 ;
	BF_dWordHArrTypeDef wDWord = { 0, 0 } ;
	while(1) {
		// Lấy double word
		wDWord = CS_getDoubleWord( address ) ;
		HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;

		// Kiểm tra có phải đã đến data cuối cùng chưa ?
		if( ( wDWord.isLastDWord == true ) && ( address > 0x800 ) ) {
			Flash_write_doubleWord( address + _baseAddress, (const uint8_t*)wDWord.data ) ;
			result = true ;
			break ;

		// Trường hợp này là bị lỗi chớ k phải đã hoàn thành
		}else if( ( wDWord.isLastDWord == true ) && ( address < 0x800 )  ){
			break ;

		// còn nếu chưa thì cứ tiếp tục
		}else {
			Flash_write_doubleWord( address + _baseAddress, (const uint8_t*)wDWord.data ) ;
			address += 0x08 ;
		}

		HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;
		// Debug
		if( address == 0x0C60 )
			Terminal.println( "...10%..." ) ;
		else if( address == 0x18E0 )
			Terminal.println( "...20%..." ) ;
		else if( address == 0x2560 )
			Terminal.println( "...30%..." ) ;
		else if( address == 0x31E0 )
			Terminal.println( "...40%..." ) ;
		else if( address == 0x3E60 )
			Terminal.println( "...50%..." ) ;
		else if( address == 0x4AE0 )
			Terminal.println( "...60%..." ) ;
		else if( address == 0x5760 )
			Terminal.println( "...70%..." ) ;
		else if( address == 0x63E0 )
			Terminal.println( "...80%..." ) ;
		else if( address == 0x76A0 )
			Terminal.println( "...90%..." ) ;

		HAL_GPIO_TogglePin( user_led_GPIO_Port, user_led_Pin ) ;


	}

	if( result == true )
		Terminal.println( "...100%... Done." ) ;

	return result ;
}



/*__________________________________________________________________________________________________________________________________________*/
char *getCurrentVersion() {
	// version: 1.0.0FF
	static char version[24] ;
	memset( version, 0 , 24 ) ;

	char firstDW[8] = {0} ;
	char secondDW[8] = {0} ;
	char thirdDW[8] = {0} ;
	memcpy( firstDW, Flash_read_doubleWord(0x080FA000), 8 ) ;
	memcpy( secondDW, Flash_read_doubleWord(0x080FA008), 8 ) ;
	memcpy( thirdDW, Flash_read_doubleWord(0x080FA010), 8 ) ;

	for( int i = 0 ; i < 8 ; i++ )
		version[i] = firstDW[i] ;
	for( int i = 0 ; i < 8 ; i++ ) {
		if( checkFWnameFormat( (char)secondDW[i] ) == 1 )
			version[ i + 8 ] = secondDW[i] ;
		else
			version[ i + 8 ] = 0x00 ;
	}
	for( int i = 0 ; i < 8 ; i++ ) {
		if( checkFWnameFormat( thirdDW[i] ) == 1 )
			version[ i + 16 ] = thirdDW[i] ;
		else
			version[ i + 16 ] = 0x00 ;
	}

	return version ;
}



/*__________________________________________________________________________________________________________________________________________*/
char *getOnSimVersion() {
	// version: 1.0.0FF
	static char version[25] ;
	memset( version, 0 , 25 ) ;

	Sim7600.sendCmd( "AT\r", "OK", 2000 ) ;
	Sim7600.sendCmd( "AT+FSCD=E:\r", "OK", 2000 ) ;
	Sim7600.sendCmd( "AT+FSLS\r", "OK", 2000 ) ;

	memcpy( version, strstr( Sim7600.rxData.c_str(), "datalogger" ), 24 ) ;

	// filter version name
	for( int i = 0 ; i < 24 ; i++ )
		if( checkFWnameFormat( version[i] ) == false )
			version[i] = 0x00 ;

	return version ;
}



/*__________________________________________________________________________________________________________________________________________*/
// datalogger_1.1.1.bin => d,a,t,l,o,g,e,r,b,i,n,_,.,'number'
bool checkFWnameFormat( char _character ) {
	switch ( _character ) {
		case 'd':
			return 1 ;
		case 'a':
			return 1 ;
		case 't':
			return 1 ;
		case 'l':
			return 1 ;
		case 'o':
			return 1 ;
		case 'g':
			return 1 ;
		case (char)'e':
			return 1 ;
		case 'r':
			return 1 ;
		case 'b':
			return 1 ;
		case 'i':
			return 1 ;
		case 'n':
			return 1 ;
		case '_':
			return 1 ;
		case '.':
			return 1 ;

		default:
			if( ( _character >= 0x30 && _character <= 0x39 ) )
				return 1 ;
			else
				return 0 ;
			break;
	}
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
