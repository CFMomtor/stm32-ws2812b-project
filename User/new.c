#include "main.h"
#include "adc/bsp_adc.h"
#include "dma/bsp_dma.h"
#include "key/bsp_key.h"
#include "tim_pwm/bsp_tim.h"
#include "ws2812b/bsp_ws2812b.h"
#include "dwt/bsp_dwt.h"
#include "ws2812b/app_ws2812b.h"
#include "key/app_key.h"


uint16_t Num1,Num2;



void SystemClock_Config(void);






int main(void)
{
    
  
    HAL_Init();   //ÂàùÂ?ãÂåñHALÂ∫?
   
 
    
    SystemClock_Config();           // ÈÖçÁΩÆÁ≥ªÁªüÊó∂ÈíüÔºåË?æÁΩÆ‰∏? 72 MHz 
    
    KEY_GPIO_Config();              //ÂàùÂ?ãÂåñÊåâÈîÆ
    
    WS2812B_Init();                 //ÂàùÂ?ãÂåñWS2812BÊ®°Âùó
    
    MX_USART1_UART_Init();          //‰∏≤Âè£ÂàùÂ?ãÂåñ
    
    HAL_TIM_Base_Start_IT(&htim4);
    
        
 
  while (1)
  {
      
      KEY_WS2812BMode();
      WS2812B_Basic_Mode();
   

  }
}



/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
     /* Initialization Error */
    while(1);
  }
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

