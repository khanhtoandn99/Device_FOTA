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
#define DATA_START_ADDRESS 		 	((uint32_t)0x0807F800)	//Page 257
#define LENGTH_START_ADDRESS 		((uint32_t)0x0807F000)	//Page 256
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t diaChiFOTAFW = 0x08040000 ;
uint32_t diaChiRunningFW = 0x08060000  ;
int chooseFW = 0 ;

uint64_t Rx_Data[2] ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) ;

void checkAndjumptoNewFW() ;

char *getCurrentVersion() ;

char *getCheckVersion() ;

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

  // 2. Tiếp theo, kiểm tra version hiện tại và version đang có trên server
  // 2.1. Lấy dữ liệu version trên server

  // 2.2. Lấy dữ liệu hiện tại từ bộ nhớ FLASH

  Flash_erase(500) ;

  uint8_t x[20] = "datalogger_1.0.0.bin" ;
  Flash_write( 0x080FA000, x, strlen((char*)x) ) ;

  // Lấy version hiện tại trong FLASH
  char currentVersion[24] = {0} ;
  memcpy( currentVersion, getCurrentVersion(), 24 ) ;

  _Bool isHaveNewVersion = 0 ;
  // Lấy version trên module sim
  if( strstr( getCheckVersion(), currentVersion ) == NULL )
	  // Nghĩa là đã có version mới
	  isHaveNewVersion = 1 ;
  else
	  isHaveNewVersion = 0 ;


  if( isHaveNewVersion == 1) {
	  terminal_println( "There is new version: ", 200 ) ;
  	  terminal_print( getCheckVersion(), 200 ) ;
  	  terminal_println( "Going to FOTA", 200 ) ;
  }else
	  terminal_println( "Version is up to date.", 200 ) ;









//  // Giả sử ta check có 1 firmware mới, ta bắt đầu nhảy vào chương trình FOTA
//  char info[ 150 ] = "Please choose 1 of these 2 Firmwares you would like to start:\n1. FOTA Firmware\n2. Running Firmware\n\n>>\r\n" ;
//  terminal_println( (uint8_t*)info, 2000 ) ;
//  while( chooseFW == 0 ) {
//	  if( terminal_available() ){
//		  if( strstr( terminal_read() , "GOTO_FOTAFW") != NULL )
//			  chooseFW = 1 ;
//		  else if( strstr( terminal_read(), "GOTO_RUNNINGFW" ) != NULL )
//			  chooseFW = 2 ;
//	  }
//  }
//  terminal_memreset() ;
//
//  checkAndjumptoNewFW() ;

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
void checkAndjumptoNewFW() {
	if ( chooseFW == 1 ) {
	  HAL_UART_Transmit( &TERMINAL_TXRX, (uint8_t*)"GOTO_FOTAFW\nOK", strlen( "GOTO_FOTAFW\nOK" ), 2000 ) ;
	  HAL_RCC_DeInit() ;
	  HAL_DeInit() ;
	  SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk || SCB_SHCSR_BUSFAULTENA_Msk || SCB_SHCSR_MEMFAULTENA_Msk ) ;
	  __set_MSP( *((volatile uint32_t*) diaChiFOTAFW ) ) ;

	  uint32_t JumpAddress = *((volatile uint32_t*) ( diaChiFOTAFW + 4 ));
	  void (*reset_handler)(void) = (void*)JumpAddress;
	  reset_handler();

	} else if ( chooseFW == 2 ) {
	  HAL_UART_Transmit( &TERMINAL_TXRX, (uint8_t*)"GOTO_RUNNINGFW\nOK", strlen( "GOTO_RUNNINGFW\nOK" ), 2000 ) ;
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

	for( int i = 0 ; i < 8 ; i++ )
		version[i] = firstDW[i] ;
	for( int i = 0 ; i < 8 ; i++ ) {
		if( secondDW[i] == 0xFF || secondDW[i] == 0x00 )
			version[ i + 8 ] = 0x00 ;
		else
			version[ i + 8 ] = secondDW[i] ;
	}
	for( int i = 0 ; i < 8 ; i++ ) {
		if( thirdDW[i] == 0xFF || thirdDW[i] == 0x00 )
			version[ i + 16 ] = 0x00 ;
		else
			version[ i + 16 ] = thirdDW[i] ;
	}

	return version ;
}



/*__________________________________________________________________________________________________________________________________________*/
char *getCheckVersion() {
	// version: 1.0.0FF
	static char version[24] ;
	memset( version, 0 , 24 ) ;

	sim7600_sendCmd( "AT+FSCD=E:", "OK", 200 ) ;
	sim7600_sendCmd( "AT+FSLS", "OK", 200 ) ;

	memcpy( version, strstr( sim7600_read(), "datalogger:" ), 24 ) ;

	return version ;
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
