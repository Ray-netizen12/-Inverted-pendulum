#include "stm32f1xx_hal.h"                  // Device header
#include "stm32f1xx_hal_adc.h"
#include "adc.h"

void AD_Init(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
}

uint16_t AD_GetValue(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	uint32_t adc_value = 0;

	/* 1. 配置 ADC 通道 */
	sConfig.Channel = ADC_CHANNEL_8; 

	/* 保持与原有配置一致的 Rank 和 SamplingTime */
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

	/* 2. 重新配置 ADC 通道 */
	/* 这一步相当于标准库的 ADC_RegularChannelConfig */
	/* 它会更新 hadc1 句柄内部的通道设置 */
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		/* 如果配置出错，可以在这里添加错误处理 */
		Error_Handler();
	}

	/* 3. 启动 ADC 并等待转换完成 */
	/* HAL_ADC_Start 启动转换 */
	/* HAL_ADC_PollForConversion 会一直等待，直到转换结束 (EOC 标志位置位) */
	/* HAL_MAX_DELAY 表示无限等待 */
	if (HAL_ADC_Start(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	/* 4. 读取转换结果 */
	/* 这一步相当于标准库的 ADC_GetConversionValue */
	adc_value = HAL_ADC_GetValue(&hadc1);

	/* 5. 停止 ADC */
	/* 因为你的标准库代码是单次转换模式 (DISABLE)，所以这里也停止 ADC */
	/* 这符合“转换一次后停止”的行为 */
	HAL_ADC_Stop(&hadc1);

	return (uint16_t)adc_value;
}
