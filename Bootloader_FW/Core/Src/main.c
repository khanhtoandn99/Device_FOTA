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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define true 1
#define false 0
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t diaChiFOTAFW = 0x08019000 ;
uint32_t diaChiRunningFW = 0x0803E800  ;
int chooseFW = 0 ;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) ;

void checkAndjumptoNewFW( _Bool _ifHaveNewFW ) ;

char *getCurrentVersion() ;

char *getOnSimVersion() ;

_Bool checkFWnameFormat( char _character ) ;

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Khởi động ngoại vi giao tiếp:
  sim7600_init() ;
  terminal_init() ;

  terminal_println( "----------------------------------------------------------------------------------------------------------------", 200 ) ;
  terminal_println( "                                             BOOTLOADER FIRMWARE", 200 ) ;
  terminal_println( "----------------------------------------------------------------------------------------------------------------", 200 ) ;
  terminal_println( "Booting ... ", 200 ) ;
  // 2. Tiếp theo, kiểm tra version hiện tại và version đang có trên module sim ( đã được tải về từ server )
  // 2.1. Lấy dữ liệu version hiện tại từ bộ nhớ FLASH

//  Flash_erase(500) ;
//  HAL_Delay(1000) ;
//  uint8_t x[24] = "datalogger_1.0.0.bin" ;
//  Flash_write( 0x080FA000, x, strlen((char*)x) ) ;
//  while(1){}

  sim7600_sendCmd( "AT+CRESET\r", "", 1000 ) ;
  HAL_Delay(30000) ;

  terminal_println( "Checking on current version ... ", 200 ) ;
  char currentVersion[24] = {0} ;
  strcat( currentVersion, getCurrentVersion() ) ;

  terminal_println( "Current version: ", 2000 ) ;
  terminal_print( currentVersion, 2000 ) ;
  terminal_print( " Done.", 2000 ) ;

  // 2.2. Lấy dữ liệu version trên module sim đồng thời kiểm tra
  terminal_println( "Checking cloud version ... ", 200 ) ;
  _Bool ifHaveNewVersion = false ;
  // Lấy version trên module sim
  char onSimVersion[24] = {0} ;
  memcpy( onSimVersion, getOnSimVersion(), 24 ) ;
  // Đầu tiên check xem có file nào có chữ "datalogger" không đã
  if( strstr( onSimVersion, "datalogger" ) != NULL ) {
	  // Nếu có file datalogger và detect = 0, Nghĩa là đã có version mới

	  if( strstr( onSimVersion, currentVersion ) == NULL )
		  ifHaveNewVersion = true ;

	  if( ifHaveNewVersion == true ) {
		  terminal_println( "***There is new version: ", 200 ) ;
	  	  terminal_print( onSimVersion, 200 ) ;
	  	  terminal_println( "Going to FOTA", 200 ) ;
		  ifHaveNewVersion = true ;
	  // Nếu không có version mới
	  }else {
		  terminal_println( "Application version is up to date.\n", 200 ) ;
		  terminal_println( "Going to Datalogger application firmware ...", 200 ) ;
		  ifHaveNewVersion = false ;
	  }
  // Đã bị lỗi mất file firmware
  }else {
	  terminal_println( "ERROR!!! Firmware file not found!", 200 ) ;
	  terminal_println( "Going to Datalogger application firmware ...", 200 ) ;
	  ifHaveNewVersion = false ;
  }



  // Kiểm tra và đưa ra quyết định chương trình tiếp theo sẽ nhảy vào là chương trình nào tại hàm này
  terminal_println( "Click Blue button to continue!", 200 ) ;
  while( HAL_GPIO_ReadPin(user_button_GPIO_Port, user_button_Pin) == 1 ) {
	  HAL_GPIO_TogglePin(user_led_GPIO_Port, user_led_Pin ) ;
	  HAL_Delay(100) ;
  }
  checkAndjumptoNewFW( ifHaveNewVersion ) ;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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

/*__________________________________________________________________________________________________________________________________________*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if( huart->Instance == USART1 )
		sim7600_irqProcess() ;
	else if( huart->Instance == USART2 )
		terminal_irqProcess() ;
}



/*__________________________________________________________________________________________________________________________________________*/
void checkAndjumptoNewFW( _Bool _ifHaveNewFW ) {
	terminal_println( "Jumping... ", 200 ) ;
	if ( _ifHaveNewFW == 1 ) {
	  HAL_RCC_DeInit() ;
	  HAL_DeInit() ;
	  SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk || SCB_SHCSR_BUSFAULTENA_Msk || SCB_SHCSR_MEMFAULTENA_Msk ) ;
	  __set_MSP( *((volatile uint32_t*) diaChiFOTAFW ) ) ;

	  uint32_t JumpAddress = *((volatile uint32_t*) ( diaChiFOTAFW + 4 ));
	  void (*reset_handler)(void) = (void*)JumpAddress;
	  reset_handler();

	} else if ( _ifHaveNewFW == 0 ) {
	  HAL_RCC_DeInit() ;
	  HAL_DeInit() ;
	  SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk || SCB_SHCSR_BUSFAULTENA_Msk || SCB_SHCSR_MEMFAULTENA_Msk ) ;
	  __set_MSP( *((volatile uint32_t*) diaChiRunningFW ) ) ;

	  uint32_t JumpAddress2 = *((volatile uint32_t*) ( diaChiRunningFW + 4 ));
	  void (*reset_handler)(void) = (void*)JumpAddress2;
	  reset_handler();

	}
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

	// Add and filter version name
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

	sim7600_sendCmd( "AT+FSCD=E:\r", "OK", 2000 ) ;
	sim7600_sendCmd( "AT+FSLS\r", "OK", 2000 ) ;

	HAL_Delay(100) ;
	memcpy( version, strstr( sim7600_read(), "datalogger" ), 24 ) ;

	// filter version name
	for( int i = 0 ; i < 24 ; i++ )
		if( checkFWnameFormat( version[i] ) == false )
			version[i] = 0x00 ;

	return version ;
}



/*__________________________________________________________________________________________________________________________________________*/
// datalogger_1.1.1.bin => d,a,t,l,o,g,e,r,b,i,n,_,.,'number'
_Bool checkFWnameFormat( char _character ) {
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
