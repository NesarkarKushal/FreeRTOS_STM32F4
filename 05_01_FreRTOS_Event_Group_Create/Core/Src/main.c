#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
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

#define BIT_0 ( 1 << 0 )
#define BIT_4 ( 1 << 4 )

EventGroupHandle_t Eventhandle;
void TaskA(void const * argument)
{
	EventBits_t uxBits;

  for(;;)
  {
		uxBits = xEventGroupSetBits(Eventhandle, BIT_0 | BIT_4 );
		vTaskDelay(1000);
		xEventGroupClearBits( Eventhandle,BIT_0);
  }
}

void TaskB(void const * argument)
{
	EventBits_t var1;
  for(;;)
  {
	  var1 = xEventGroupWaitBits( Eventhandle,BIT_0 | BIT_4 ,pdTRUE,pdTRUE,100 );

	  if( ( var1 & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
	  {
	  /* xEventGroupWaitBits() returned because both bits were set. */
		  printf("Both Bits were SET\n");
		  vTaskDelay(1000);
	  }
	  else if( ( var1 & BIT_0 ) != 0 )
	  {
	  /* xEventGroupWaitBits() returned because just BIT_0 was set. */
		  printf("Bit 0 was SET\n");
		  vTaskDelay(1000);
	  }
	  else if( ( var1 & BIT_4 ) != 0 )
	  {
		  printf("Bit 4 was SET\n");
		  vTaskDelay(1000);
	  }
	  else
	  {

		  printf("Nither was SET\n");
		  vTaskDelay(1000);
	  }
  }
}


int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  Eventhandle=xEventGroupCreate();
  if(Eventhandle==NULL)
  {
	  printf("Event is not created\n");
  }
  else
  {
	   xTaskCreate(TaskA," Sender",128,NULL,3,NULL);
	   xTaskCreate(TaskB," Receiver1",128,NULL,2,NULL);
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }

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
