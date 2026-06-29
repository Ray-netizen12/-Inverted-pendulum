#include "stm32f1xx_hal.h"                  // Device header
#include "tim.h"

void Timer_Init(void)
{
	MX_TIM1_Init();//每1ms中断一次
	HAL_TIM_Base_Start_IT(&htim1);	//启动定时器1
}

/*写在main.c中User CODE BEGIN 4中
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // 1. 判断是哪个定时器触发的中断
  if (htim->Instance == TIM1) 
  {

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  }
}
*/
