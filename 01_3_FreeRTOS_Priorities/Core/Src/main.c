
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>


int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
		ITM_SendChar((*ptr++));
	return len;

}

TaskHandle_t xHandle1,xHandle2,xHandle3,xHandle4;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

#define GREEN  GPIO_PIN_12
#define ORANGE GPIO_PIN_13
#define RED    GPIO_PIN_14
#define BLUE   GPIO_PIN_15

//#define pdMS_TO_TICKS(500)
void  vBlueLedControllerTask(void *pvParameters)
{
	int i;

	while(1)
	{
				printf("Blue\n");
				HAL_GPIO_WritePin(GPIOD,BLUE,1);
				vTaskDelay( 500 );
				HAL_GPIO_WritePin(GPIOD,BLUE,0);
				vTaskDelay( 500 );

	}
}


void  vRedLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{

		printf("Red\n");


				HAL_GPIO_WritePin(GPIOD,RED,1);
				vTaskDelay( 500);

				HAL_GPIO_WritePin(GPIOD,RED,0);
				vTaskDelay( 500 );

	}
}


void  vOrangeLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		printf("Orange\n");

				HAL_GPIO_WritePin(GPIOD,ORANGE,1);
				vTaskDelay( 500 );

				HAL_GPIO_WritePin(GPIOD,ORANGE,0);
		vTaskDelay( 500 );

	}
}


void  vGreenLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		printf("Green\n");

		vTaskPrioritySet( xHandle1, 1 );
		vTaskPrioritySet( xHandle2, 2 );
		vTaskPrioritySet( xHandle3, 3 );

				HAL_GPIO_WritePin(GPIOD,GREEN,1);
				vTaskDelay( 500 );

				HAL_GPIO_WritePin(GPIOD,GREEN,0);
			vTaskDelay( 500 );


	}
}

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  xTaskCreate(vBlueLedControllerTask,
  							 "Blue Led Controller",
  								128,
  								NULL,
  								1,
  								&xHandle1
  								);

  	xTaskCreate(vRedLedControllerTask,
  							 "Red Led Controller",
  								128,
  								NULL,
  								1,
								&xHandle2
  								);




  	xTaskCreate(vOrangeLedControllerTask,
  							 "Orange Led Controller",
  								128,
  								NULL,
  								1,
								&xHandle3
  								);

  	 xTaskCreate(vGreenLedControllerTask,
  							 "Green Led Controller",
  								128,
  								NULL,
  								4,
								&xHandle4
  								);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
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
