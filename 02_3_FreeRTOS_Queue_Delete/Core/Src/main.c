#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
		ITM_SendChar((*ptr++));
	return len;
}

typedef struct Transmit
{
	uint32_t data[5];
} Trans;

Trans write;

typedef struct Receive
{
	uint32_t GET[5];
} Recv;

Recv read;

QueueHandle_t queue;



void TaskA(void const * argument)
{
	int i=0;
	BaseType_t xStatus;
	write.data[0]=10;
	write.data[1]=20;
	write.data[2]=30;
	write.data[3]=40;
	write.data[4]=50;

	for(i=0;i<5;i++)
	{
//		xStatus=xQueueSendToFront(queue,&write.data[i],100);
		xStatus=xQueueSendToBack(queue,&write.data[i],100);

		  if(xStatus == pdTRUE)
		  {
			  printf("Sending = %d\n",write.data[i]);
			  vTaskDelay(1000);
		  }
	}
}


void TaskB(void const * argument)
{
  int i=0;
  for(;;)
  {
		BaseType_t xStatus1;
		for(i=0;i<5;i++)
		{
		xStatus1=xQueueReceive(queue,&read.GET[i],100);
		  if(xStatus1 ==pdTRUE)
		  {
				  printf(" Received =%d \n",read.GET[i]);
				  vTaskDelay(1000);

		  }
	  }
		  vQueueDelete(queue);

  }
}


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  xTaskCreate(TaskA,
		  	  "Transmitter",
			  128,
			  NULL,
			  1,
			  NULL);

  xTaskCreate(TaskB,
		  	  "Receiver",
			  128,
			  NULL,
			  1,
			  NULL);


  queue=xQueueCreate(5,4);

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
