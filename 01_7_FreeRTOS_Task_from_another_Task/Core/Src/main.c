#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core

#include <stdio.h>

int _write(int file,char *ptr, int len)
{
	int i=0;
	for(i=0;i<len;i++)
		ITM_SendChar((*ptr++));
	return len;

}

#define GREEN  GPIO_PIN_12
#define ORANGE GPIO_PIN_13
#define RED    GPIO_PIN_14
#define BLUE   GPIO_PIN_15

void GPIO_Init(void);

void  vBlueLedControllerTask(void *pvParameters);
void  vRedLedControllerTask(void *pvParameters);
void  vOrangeLedControllerTask(void *pvParameters);
void  vGreenLedControllerTask(void *pvParameters);

int main()
{
	 GPIO_Init();

	 xTaskCreate(vBlueLedControllerTask,
							 "Blue Led Controller",
								100,
								NULL,
								1,
								NULL
								);
	 printf("Lets Begins\n");


   vTaskStartScheduler();

	 while(1){};
}



void  vBlueLedControllerTask(void *pvParameters)
{
	int i;

		 xTaskCreate(vRedLedControllerTask,
							 "Red Led Controller",
								100,
								NULL,
								1,
								NULL
								);

		 printf("Red Created\n");

	 xTaskCreate(vOrangeLedControllerTask,
							 "Orange Led Controller",
								100,
								NULL,
								1,
								NULL
								);
	 printf("Orange Created\n");

	 xTaskCreate(vGreenLedControllerTask,
							 "Green Led Controller",
								100,
								NULL,
								1,
								NULL
								);
	 printf("Green Created\n");


//	 printf("Tasks Has been Created from Another Task\n");

	while(1)
	{
//		HAL_GPIO_TogglePin(GPIOD,BLUE);
//		for(i=0;i<70000;i++){}
	}

}


void  vRedLedControllerTask(void *pvParameters)
{
	int i;
	printf("Red\n");
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOD,RED);
		for(i=0;i<70000;i++){}
	}
}

void  vOrangeLedControllerTask(void *pvParameters)
{
	int i;

	while(1)
	{
		printf("Orange\n");

		HAL_GPIO_TogglePin(GPIOD,ORANGE);
		for(i=0;i<70000;i++){}
	}
}

void  vGreenLedControllerTask(void *pvParameters)
{
	int i;

	while(1)
	{
		printf("Green\n");
		HAL_GPIO_TogglePin(GPIOD,GREEN);
		for(i=0;i<70000;i++){}
	}
}

void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

