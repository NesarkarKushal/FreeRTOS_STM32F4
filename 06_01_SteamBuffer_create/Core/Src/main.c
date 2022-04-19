#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "stream_buffer.h"

//#include "FreeRTOS/source/stream_buffer.c"

#include <stdio.h>

int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
	ITM_SendChar((*ptr++));
	return len;
}

StreamBufferHandle_t  rx,tx;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void TaskA()
{
	size_t send;
	int i;
	uint8_t data[] ={1,2,3,4,5};
	uint8_t size;
	size=sizeof(data);

	 xStreamBufferReset(rx);
	 xStreamBufferReset(tx);

	xStreamBufferSetTriggerLevel(tx,size);


	for(;;)
	{
		send = xStreamBufferSend(rx,
								 data,
								 size,
								 100 );
		vTaskDelay(1000);
		for(i=0;i<size;i++)
		{
			printf("data sent=%d\n",data[i]);
			vTaskDelay(100);
		}
		printf("Data Sent\n");

	}
}

void TaskB()
{
	uint8_t rx_data[6];
	uint8_t size;
	int i;
	size=sizeof(rx_data);
	xStreamBufferSetTriggerLevel(rx,size);

	for(;;)
	{
		xStreamBufferReceive(tx,
							rx_data,
							size,
							100);
		vTaskDelay(1000);
		if(xStreamBufferIsFull(rx)==pdTRUE)
		{
			printf("its full\n");
		}
		else
		{
		for(i=0;i<size;i++)
		{
			printf("Data Recived=%d\n",rx_data[i]);
			vTaskDelay(1000);
		}
			printf("Data Received\n");

		}
	}
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  tx = xStreamBufferCreate(100,10 );
  rx = xStreamBufferCreate(100,10 );



  if(tx != pdFALSE)
  {
	  xTaskCreate(TaskA,"Sender",128,NULL,2,NULL);
	  xTaskCreate(TaskB,"Receiver",128,NULL,1,NULL);

	  printf("Threads Created\n");
  }
  else
  {
	  printf("Buffer Not Created\n");
  }


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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD13 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}


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
