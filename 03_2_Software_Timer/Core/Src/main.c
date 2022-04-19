/* Creating a Timer
 * Starting a Timer
 * Changing Period of timer
 * Get Expire time
 * Reseting Timer
 * Stopping Timer
 */

#include "main.h"
#include "FreeRTOS.h"
#include"task.h"
#include "timers.h"
#include <stdio.h>

int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
	ITM_SendChar((*ptr++));
	return len;
}

int i=0;
TickType_t time;
TickType_t period;
TimerHandle_t Handler;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


void Callback01(void const * argument)
{
	BaseType_t stop;
	BaseType_t reset;

	//Returns the Period of the timer
	period= xTimerGetPeriod(Handler);

	printf("timer=%d\n",i);
	i+=1;
	printf("Expire Time=%d\n",time);
	printf("Period=%d\n",period);

	stop=xTimerStop(Handler,0);
	if(stop == pdPASS)
	{
		printf("Timer Stopped\n");
		vTaskDelay(1000);
	}

	reset= xTimerReset( Handler, 0 );
	if(reset==pdPASS)
	{
		printf("Successfully Reset\n");
		vTaskDelay(1000);

	}
}


int main(void)
{

BaseType_t stat;

HAL_Init();
SystemClock_Config();
MX_GPIO_Init();

// Create the timer

Handler=xTimerCreate("time",
		100,
		1, //OneSHort timer=0 , Perodic Timer = 1
		0,
		Callback01);

// Start the Timer
stat=xTimerStart(Handler,0);

// Changing the Period of timer during RunTime
xTimerChangePeriod(Handler,1000,0 );

// Get The Expire/Ending Time of Timer
time=xTimerGetExpiryTime( Handler);


 vTaskStartScheduler();

  while (1)
  {
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
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
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */

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
