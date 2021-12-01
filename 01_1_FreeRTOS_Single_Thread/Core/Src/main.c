#include "stm32f4xx_hal.h"
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

#define GREEN  GPIO_PIN_12


void GPIO_Init(void);

void  vGreenLedControllerTask(void *pvParameters);



int main()
{
	 GPIO_Init();

	 xTaskCreate(vGreenLedControllerTask,
							 "Green Led Controller",
								128,
								NULL,
								4,
								NULL
								);





   vTaskStartScheduler();

	 while(1){};
}


void  vGreenLedControllerTask(void *pvParameters)
{
	int i=0;
	TickType_t  xTimeInTicks = pdMS_TO_TICKS( 500 );
	printf("Tick = %d \n",xTimeInTicks);

	while(1)
	{

		printf("Green =  %d \n",i);
		i=i+1;

				HAL_GPIO_WritePin(GPIOD,GREEN,1);
				vTaskDelay( 500 );

				HAL_GPIO_WritePin(GPIOD,GREEN,0);
			    vTaskDelay( 500 );


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
