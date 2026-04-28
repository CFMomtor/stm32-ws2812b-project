#ifndef __BSP_ws2812b_H__
#define __BSP_ws2812b_H__


#include "main.h"


#define WS2812B_NUM  64    //灯珠数量
#define WS2812B_BUF_SINGLE  24   //每个灯有24bit数据依次按G R B排列
//总共要发的数据位数一颗灯要24bit 8bit控制一个颜色（R、G、B）24bit + 100复位信号
#define WS2812B_BUF_LEN  (RESET_WORD + WS2812B_BUF_SINGLE * WS2812B_NUM + DUMMY_WORD)   

#define RESET_WORD  40  //发送低电平时间，表示数据复位
#define DUMMY_WORD  40  //占位或延时用，无实际数据作用


#define DATUM_0  22  //约0.4us高电平
#define DATUM_1  68  //约0.8us高电平

#define LOW      20   //ws2812b的低档，只用 20% 适用于夜间，睡眠辅助
#define MODI     47   //ws2812b的中档，适用于日常看书，办公学习
#define HIGH     70   //ws2812b的高档，适用于高亮作业，临时强光，注意不能长时间使用对2812b损耗极大

#define MAX_BRIGHT 180   //限制峰值亮度 180

#define WATER_LED_TIME  50   //流水灯间隔时间
#define WATER_BRINGHT_MAX  150  //流水灯上限亮度，防止发热

/* 定义单个LED三原色值结构体 */
typedef struct 
{
    uint8_t  R;   //红色通道值(0 ～ 255)
    uint8_t  G;   //绿色通道值(0 ～ 255)
    uint8_t  B;   //蓝色通道值(0 ～ 255)   
}RGB_Color_TypeDef;




extern  const uint8_t gamma_table_180 [181];
extern uint16_t ws2812b_buf[WS2812B_BUF_LEN];
extern uint8_t water_pos;





// 声明句柄变量 huart1 htim2 htim4 hdma_tim2_ch1 ，定义在别的文件中，方便多个文件访问I2C1接口
//extern UART_HandleTypeDef huart1;

//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim4;
//extern DMA_HandleTypeDef hdma_tim2_ch1;



/* 函数声明 */
void WS2812B_Init(void);
void WS2812B_Setcolor_single( void );
void WS2812B_Flush(void );
void WS2812B_ClearAll(void );
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void MX_USART1_UART_Init(void);
void WS2812B_SetLEDColor (uint16_t index,const RGB_Color_TypeDef color);
void ws2812_dir_step(void );
void WS2812B_Breathe(uint16_t index ,const RGB_Color_TypeDef color);
void WS2812B_OFF_All(void );
void WS2812B_RunHorsesingle(void);
void WS2812B_Flow3group(uint16_t index ,uint8_t delay_ms );
void WS2812B_ON_All(void );
void WS2812B_warm_yellow(void );
void WS2812B_warm_white(void );
void WS2812B_cool_white(void );
void WS2812B_cool_sunlight_white(void );    
void Brightness_Set(uint8_t percent);
void Bringhtness_Step(int8_t step);
uint8_t ws2812b_8x8(uint8_t x,uint8_t y);
void WS2812B_Water_Run(void );




#endif /* __BSP_ws2812b_H__ */


