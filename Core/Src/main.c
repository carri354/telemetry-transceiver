/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "spi.h"
#include "subghz.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "evclib.h"
#include "gps.h"
#include "lora.h"
#include "mcp2515.h"
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
uint8_t message[8] = "Hello!!";



uint8_t outbuf[8] = "whatttt\0";
uint8_t size;

uint8_t rxb0[20];
uint8_t rxb1[20];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t canIrqPending = 0;

/* GPIO Interrupt Handler */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin) {
	// if (gpio_pin == GPIO_PIN_0) {
	HAL_GPIO_WritePin(DBG_LED_1_GPIO_Port, DBG_LED_1_Pin, GPIO_PIN_RESET);
	canIrqPending = 1;
	// }
}


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	HAL_GPIO_TogglePin(DBG_LED_2_GPIO_Port, DBG_LED_2_Pin);
//}
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
	MX_SUBGHZ_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_RTC_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPS_RESET_N_GPIO_Port, GPS_RESET_N_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPS_RESET_N_GPIO_Port, GPS_RESET_N_Pin, GPIO_PIN_SET);
	HAL_Delay(20);

	// NOTE: THE LEDs are active low
	HAL_GPIO_WritePin(DBG_LED_1_GPIO_Port, DBG_LED_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DBG_LED_2_GPIO_Port, DBG_LED_2_Pin, GPIO_PIN_SET);


	DBG_Init(&huart2);
	MCP_Status_t status = MCP_Init();
	DBG(DBG_W, DBG_MAIN, "MCP Init Status: %d", status);
	lora_init(LORA_SF7, LORA_BW_250);
	lora_setupTX();



	//	lora_init(LORA_SF7, LORA_BW_125);
	//	lora_setupRX();
	//	lora_startContinuousRx((uint32_t)0xFFFFFF);



	DBG(DBG_MAIN,DBG_I, "Scout Online");
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */



		//		uint8_t msgCount = 0;
		//		if (canIrqPending) {
		//			DBG(DBG_MAIN, DBG_I, "CAN Message Received!");
		//			HAL_GPIO_WritePin(DBG_LED_1_GPIO_Port, DBG_LED_1_Pin, GPIO_PIN_SET);
		//			MCP_ReadRxBuffer(rxb0, rxb1, &msgCount);
		//			HAL_UART_Transmit(&huart2, rxb0, RX_BUFFER_SIZE, HAL_MAX_DELAY);
		//			HAL_UART_Transmit(&huart2, rxb1, RX_BUFFER_SIZE, HAL_MAX_DELAY);
		//			canIrqPending = 0;
		//		}


		DBG(DBG_MAIN, DBG_I,"Sending Message");
		lora_send(message, LORA_TIMEOUT);
		HAL_Delay(10);


		//		int status = lora_receive(outbuf, &size, 8);
		//		if(status == 0){
		//			DBG(DBG_MAIN,DBG_I,"Received message of size: %d\n", size);
		//			DBG(DBG_MAIN,DBG_I,"Message: %s", outbuf);
		//		}else{
		//			DBG(DBG_MAIN, DBG_E, "Status: %d", status);
		//		}
		//
		//		HAL_Delay(100);


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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
			|RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
	RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
			|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
			|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_2);
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
