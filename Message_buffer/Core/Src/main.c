#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "message_buffer.h"

#include <stdio.h>

int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
	ITM_SendChar((*ptr++));
	return len;
}

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

MessageBufferHandle_t msg_tx,msg_rx;

void TaskA(void const * argument)
{
	size_t xBytesSent;
	uint8_t ucArrayToSend[] = { 0, 1, 2, 3 };
	char *pcStringToSend = "String to send";
	const TickType_t x100ms = pdMS_TO_TICKS( 100 );


	xBytesSent = xMessageBufferSend( msg_rx,
									ucArrayToSend,
									sizeof( ucArrayToSend ),
									x100ms );

//	if( xBytesSent != sizeof( ucArrayToSend ) )
//	{
//		printf("Failed to send no space\n");
//	}

	vTaskDelay(2000);


//	xBytesSent = xMessageBufferSend( msg_rx,
//									( void * ) pcStringToSend,
//									strlen( pcStringToSend ), 0 );
//
//	if( xBytesSent != strlen( pcStringToSend ) )
//	{
//		printf("Failed to send no space\n");
//	}
}

void TaskB(void const * argument)
{
	uint8_t ucRxData[3];
	size_t xReceivedBytes;
	int i;
	int size;
	const TickType_t xBlockTime = pdMS_TO_TICKS( 2000 );
	size_t check;

	size=sizeof( ucRxData );

	check=xMessageBufferSpacesAvailable(msg_rx);
	printf("check",check);


	for(;;)
	{
	xReceivedBytes = xMessageBufferReceive( msg_rx,
											ucRxData,
											sizeof( ucRxData ),
											xBlockTime );


	if( xReceivedBytes > 0 )
	{
		for(i=0;i<size;i++)
		{
			printf("received=%d\n",ucRxData[i]);
			vTaskDelay(10);
		}
		printf("done-------\n");
	}
}

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  msg_tx=xMessageBufferCreate(100);
  msg_rx=xMessageBufferCreate(100);

  if(msg_tx==NULL)
  {
	  printf("Failed to Create Tx message box as there is no space\n");
  }
  else if(msg_rx==NULL)
  {
	  printf("Failed to Create Rx Message box as there is no space\n");
  }
  else
  {
	  xTaskCreate(TaskA,"TX",128,NULL,1,NULL);
	  xTaskCreate(TaskB,"RX",128,NULL,1,NULL);
	  printf("Threads are created\n");
  }

  vTaskStartScheduler();
  while (1)
  {

  }
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
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

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
