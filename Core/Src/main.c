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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "LED.h"
#include "OLED_Data.h"
#include "Timer.h"
#include "Key.h"
#include "RP.h"
#include "Motor.h"
#include "PWM.h"
#include "Encoder.h"
#include "Serial.h"
#include "AD.h"
#include "pid.h"
#include <stdint.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CENTER_ANGLE 2057
#define CENTER_RANGE 750 //可调控区间
#define START_PWM 35
#define START_TIME 100
#define OFFSET_PWM 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t KeyNum;
uint8_t RunState;//运行状态，范围：0~1，0为停止，1为运行
uint16_t Angle;//角度值，范围：0~4096
int16_t Speed,Location;

//内环pid
PIDController Angle_pid = {
    .Target = CENTER_ANGLE,
    .Kp = 0.3,
    .Ki = 0.01,
    .Kd = 0.4,
    .OutMax = 100,
    .OutMin = -100,
    .Difout = 0,
};

//外环pid
PIDController Location_pid = {
    .Target = 0,
    .Kp = 1.0,
    .Ki = 0.005,
    .Kd = 16,
    .OutMax = 100,
    .OutMin = -100,
    .Difout = 0,
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();	//调用OLED初始化函数，需先初始化GPIO
  LED_Init();	//调用LED初始化函数
  //Key_Init();	//已在MX_GPIO_Init()中初始化了按键引脚
  RP_Init();	//调用RP初始化函数
  AD_Init();	//调用AD初始化函数
  Motor_Init();	//调用直流电机初始化函数
  Encoder_Init();	//调用编码器初始化函数
  Timer_Init();	//调用定时器初始化函数
  //Serial_Init();	//调用串口初始化函数


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    KeyNum = Key_GetNum();
    if (KeyNum == 1)
		{
      if(RunState == 0)
			{  
        RunState = 21;//按键1按下，切换起摆状态
      }
      else
      {
        RunState = 0;//按键1按下，切换停止状态
      }
		}
    if (KeyNum == 2)//左转一圈
    {
      Location_pid.Target -= 202;
    }
    if (KeyNum == 3)//右转一圈
    {
      Location_pid.Target += 202;
    }

    // LED指示倒立摆运动状态
    if(RunState == 4)
    {
      LED_ON();
    }
    else
    {
      LED_OFF();
    }
    // Angle_pid.Kp = RP_GetValue(1) / 4095.0 * 2;//设定Kp的范围为0~2
    // Angle_pid.Ki = RP_GetValue(2) / 4095.0 * 2;
    // Angle_pid.Kd = RP_GetValue(3) / 4095.0 * 2;

    // Location_pid.Kp = RP_GetValue(1) / 4095.0 * 2;//设定Kp的范围为0~2
    // Location_pid.Ki = RP_GetValue(2) / 4095.0 * 2;
    // Location_pid.Kd = RP_GetValue(3) / 4095.0 * 5;



    OLED_Printf(0, 0, OLED_6X8, "Angle:");
    OLED_Printf(0, 12, OLED_6X8, "Kp:");
    OLED_ShowFloatNum(18, 12, Angle_pid.Kp, 1, 3, OLED_6X8);
    OLED_Printf(0, 20, OLED_6X8, "Ki:");
    OLED_ShowFloatNum(18, 20, Angle_pid.Ki, 1, 3, OLED_6X8);
    OLED_Printf(0, 28, OLED_6X8, "Kd:");
    OLED_ShowFloatNum(18, 28, Angle_pid.Kd, 1, 3, OLED_6X8);
	  OLED_Printf(0, 40, OLED_6X8, "Tar:");
    OLED_ShowNum(24, 40, (int16_t)Angle_pid.Target, 4,  OLED_6X8);
    OLED_Printf(0, 48, OLED_6X8, "Act:");
    OLED_ShowNum(24, 48, (int16_t)Angle, 4,  OLED_6X8);
    OLED_Printf(0, 56, OLED_6X8, "Out:");
    OLED_ShowSignedNum(24, 56, (int16_t)Angle_pid.Out, 3,  OLED_6X8);

    OLED_Printf(64, 0, OLED_6X8, "Location:");
    OLED_Printf(64, 12, OLED_6X8, "Kp:");
    OLED_ShowFloatNum(82, 12, Location_pid.Kp, 1, 3, OLED_6X8);
    OLED_Printf(64, 20, OLED_6X8, "Ki:");
    OLED_ShowFloatNum(82, 20, Location_pid.Ki, 1, 3, OLED_6X8);
    OLED_Printf(64, 28, OLED_6X8, "Kd:");
    OLED_ShowFloatNum(82, 28, Location_pid.Kd, 1, 3, OLED_6X8);
	  OLED_Printf(64, 40, OLED_6X8, "Tar:");
    OLED_ShowSignedNum(82, 40, (int16_t)Location_pid.Target, 4,  OLED_6X8);
    OLED_Printf(64, 48, OLED_6X8, "Act:");
    OLED_ShowSignedNum(82, 48, (int16_t)Location, 4,  OLED_6X8);
    OLED_Printf(64, 56, OLED_6X8, "Out:");
    OLED_ShowSignedNum(82, 56, (int16_t)Location_pid.Out, 3,  OLED_6X8);

	  OLED_Update();

    Serial_Printf("%d,%d,%d,%d\r\n", (int16_t)Angle_pid.Target, (int16_t)Angle, (int16_t)Angle_pid.Out, (int16_t)Angle_pid.Difout);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t Count1 = 0, Count2 = 0,Count3 = 0;
  static int16_t Counttime;
  static int16_t Angle0, Angle1, Angle2;
  // 1. 判断是哪个定时器触发的中断
  if (htim->Instance == TIM1) 
  {
    Key_Tick();

    Angle = AD_GetValue();
    Speed = Encoder_Get();
    Location += Speed;

    //状态机
    if (RunState == 0) // 停止状态下，电机不运行
    {
      Motor_SetPWM(0);
    }
    else if (RunState == 1) 
    {
      Count3++;
      if (Count3 == 40) 
      {
        Count3 = 0;
        // 读取角度值
        Angle2 = Angle1;
        Angle1 = Angle0;
        Angle0 = Angle;

        if(Angle0 > CENTER_ANGLE + CENTER_RANGE 
        && Angle1 > CENTER_ANGLE + CENTER_RANGE 
        && Angle2 > CENTER_ANGLE + CENTER_RANGE
        && Angle1 < Angle0
        && Angle1 < Angle2)
        {
          RunState = 21; // 切换到左起摆状态
        }
        if(Angle0 < CENTER_ANGLE - CENTER_RANGE 
        && Angle1 < CENTER_ANGLE - CENTER_RANGE 
        && Angle2 < CENTER_ANGLE - CENTER_RANGE
        && Angle1 > Angle0
        && Angle1 > Angle2)
        {
          RunState = 31; // 切换到右摆状态
        }

        //达到中心稳定区间
        if(Angle0 > CENTER_ANGLE - CENTER_RANGE 
        && Angle0 < CENTER_ANGLE + CENTER_RANGE
        && Angle1 > CENTER_ANGLE - CENTER_RANGE 
        && Angle1 < CENTER_ANGLE + CENTER_RANGE)
        {
          Location = 0; // 重置位置
          Location_pid.Target = 0;
          RunState = 4; // 切换到PID模式
        }
      }
    }
    //21~24为左起摆
    else if (RunState == 21) 
    {
      Motor_SetPWM(START_PWM);
      Counttime = START_TIME;
      RunState = 22; 
    }
    else if (RunState == 22) 
    {
      Counttime--;
      if (Counttime <= 0)
      {
        RunState = 23; // 切换到下一个状态
      }
    }
    else if (RunState == 23) 
    {
      Motor_SetPWM(-START_PWM);
      Counttime = START_TIME; // 重置计时器
      RunState = 24; 
    }
    else if (RunState == 24) 
    {
      Counttime--;
      if (Counttime <= 0)
      {
        Motor_SetPWM(0); // 停止电机
        RunState = 1; 
      }
    }
    //31~34为右起摆
    else if (RunState == 31) 
    {
      Motor_SetPWM(-START_PWM);
      Counttime = START_TIME;
      RunState = 32; 
    }
    else if (RunState == 32) 
    {
      Counttime--;
      if (Counttime <= 0)
      {
        RunState = 33; // 切换到下一个状态
      }
    }
    else if (RunState == 33) 
    {
      Motor_SetPWM(START_PWM);
      Counttime = START_TIME; // 重置计时器
      RunState = 34; 
    }
    else if (RunState == 34) 
    {
      Counttime--;
      if (Counttime <= 0)
      {
        Motor_SetPWM(0); // 停止电机
        RunState = 1; 
      }
    }
    //4为pid模式
    else if (RunState == 4)
    {
      //判断是否倒下
      if (Angle < CENTER_ANGLE - CENTER_RANGE || Angle > CENTER_ANGLE + CENTER_RANGE)
      {
        RunState = 0; // 如果角度超出可控范围，停止运行
      }
      //pid计算
      Count2++;
      if (Count2 >= 50) // 每20次中断计算一次外环
      {
        Count2 = 0;
        Location_pid.Actual = Location;
        PID_Update(&Location_pid);
        Angle_pid.Target = CENTER_ANGLE - Location_pid.Out ;// 外环输出作为内环目标角度
      }

      Count1++;
      if (Count1 >= 5) // 每5次中断计算一次PID
      {
        Count1 = 0;
        Angle_pid.Actual = Angle;
        PID_Update(&Angle_pid);
        Motor_SetPWM(Angle_pid.Out);
      }
    }
    
  }
}

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
