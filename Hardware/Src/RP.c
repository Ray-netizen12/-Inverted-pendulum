#include "stm32f1xx_hal.h"                    // Device header
#include "stm32f1xx_hal_adc.h"
#include "adc.h"

/**
  * 函    数：RP初始化
  * 参    数：无
  * 返 回 值：无
  */
void RP_Init(void)
{
	/*ADC校准*/
	HAL_ADCEx_Calibration_Start(&hadc2);
}

/**
  * 函    数：获取AD转换的值
  * 参    数：ADC_Channel 指定AD转换的通道，范围：ADC_Channel_x，其中x可以是0/1/2/3
  * 返 回 值：AD转换的值，范围：0~4095
  */
uint16_t RP_GetValue(uint8_t ADC_Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	uint32_t adc_value = 0;

	/* 1. 根据传入的通道号，配置通道结构体 */
	/* 将1, 2, 3,4映射到 HAL 库的 ADC_CHANNEL_0, ADC_CHANNEL_1... */
	switch (ADC_Channel)
	{
		case 1:
		sConfig.Channel = ADC_CHANNEL_2;
		break;
		case 2:
		sConfig.Channel = ADC_CHANNEL_3;
		break;
		case 3:
		sConfig.Channel = ADC_CHANNEL_4;
		break;
		case 4:
		sConfig.Channel = ADC_CHANNEL_5;
		break;
		default:
		return 0; // 如果通道号无效，返回0
	}

	/* 保持与原有配置一致的 Rank 和 SamplingTime */
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

	/* 2. 重新配置 ADC 通道 */
	/* 这一步相当于标准库的 ADC_RegularChannelConfig */
	/* 它会更新 hadc2 句柄内部的通道设置 */
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		/* 如果配置出错，可以在这里添加错误处理 */
		Error_Handler();
	}

	/* 3. 启动 ADC 并等待转换完成 */
	/* HAL_ADC_Start 启动转换 */
	/* HAL_ADC_PollForConversion 会一直等待，直到转换结束 (EOC 标志位置位) */
	/* HAL_MAX_DELAY 表示无限等待 */
	if (HAL_ADC_Start(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	/* 4. 读取转换结果 */
	/* 这一步相当于标准库的 ADC_GetConversionValue */
	adc_value = HAL_ADC_GetValue(&hadc2);

	/* 5. 停止 ADC */
	/* 因为你的标准库代码是单次转换模式 (DISABLE)，所以这里也停止 ADC */
	/* 这符合“转换一次后停止”的行为 */
	HAL_ADC_Stop(&hadc2);

	return (uint16_t)adc_value;
}
