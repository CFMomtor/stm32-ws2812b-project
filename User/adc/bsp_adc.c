/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc/bsp_adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};   //定义通道配置结构体

  /** Common config 
  */
  hadc1.Instance = ADC1;                      //选择芯片内部 ADC1 模块
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE; //配置扫描模式 
  hadc1.Init.ContinuousConvMode = DISABLE;    //配置转换模式( 单次 or 连续 )
  hadc1.Init.DiscontinuousConvMode = DISABLE; //间断/不连续分段转换模式
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;  //ADC 触发源选择(软件触发)
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;  //采样数据对齐方式 (右对齐)
  hadc1.Init.NbrOfConversion = 1;              //规则组序列总通道数
  /* 把上面所有整体模式配置写入寄存器，初始化 ADC */
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;                        //选择 ADC 硬件通道：PA1引脚模拟输入采集
  sConfig.Rank = ADC_REGULAR_RANK_1;                      //通道排队序号
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;      //采样时间
  /* 把引脚通道，速度，排序单独通道配置写入生效 */
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)  
  {
    Error_Handler();
  }

}

/* 底层硬件初始化函数 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};   //定义 GPIO 引脚配置结构体
  if(adcHandle->Instance==ADC1)             //判断是不是 ADC1 外设
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();            //开启 ADC1 模块内部时钟
  
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启 GPIOA 端口时钟
    /**ADC1 GPIO Configuration    
    PA1     ------> ADC1_IN1 
    */
    GPIO_InitStruct.Pin = GM_DO_Pin;          //选择要配置的引脚
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;  //设置引脚模式(模拟输入)
    /* 把上面的引脚配置写入寄存器，生效初始化 PA1 */
    HAL_GPIO_Init(GM_DO_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}
/* 底层硬件反初始化函数 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PA1     ------> ADC1_IN1 
    */
    HAL_GPIO_DeInit(GM_DO_GPIO_Port, GM_DO_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
