#include "ws2812b/app_ws2812b.h"
#include "ws2812b/bsp_ws2812b.h"
#include "dwt/bsp_dwt.h"
#include "key/bsp_key.h"

/* 静态颜色定义 ------------------------------ */
/* 定义常用 RGB 颜色值结构体，便于 LED 控制或显示使用 */
const RGB_Color_TypeDef RED     = {255 , 0, 0};       // 红色
const RGB_Color_TypeDef ORANGE  = {127, 106, 0};     // 橙色
const RGB_Color_TypeDef YELLOW  = {127, 216, 0};     // 黄色
const RGB_Color_TypeDef GREEN   = {0, 255, 0};       // 绿色
const RGB_Color_TypeDef CYAN    = {0, 255, 255};     // 青色
const RGB_Color_TypeDef BLUE    = {0, 0, 255};       // 蓝色
const RGB_Color_TypeDef PURPLE  = {238, 130, 238};   // 紫色
const RGB_Color_TypeDef BLACK   = {0, 0, 0};         // 黑色
const RGB_Color_TypeDef WHITE   = {255, 255, 255};   // 白色
const RGB_Color_TypeDef MAGENTA = {255, 0, 220};     // 品红/洋红色


const RGB_Color_TypeDef breathe_color[] = {
    {255 , 0, 0},
    {127, 106, 0},
    {127, 216, 0},
    {0, 255, 0},
    {0, 255, 255},
    {0, 0, 255},
    {238, 130, 238}
};


uint8_t breathe_color_num = 0;  //呼吸灯颜色索引变量
uint8_t breathe_color_falg = 0; //呼吸灯变颜色标志位，为1时改变颜色


volatile WS2812B_BASIC_MODETypedef ws2812b_basic_mode = WS2812_MODE_OFF;
WS2812B_BASIC_MODETypedef ws2812b_new_mode = WS2812_BASIC_MODE_ON;


void show_2812B_linght(WS2812B_BASIC_MODETypedef mode)
{
    
    switch (mode)
    {
        case WS2812_MODE_OFF :
            WS2812B_OFF_All();
            break;
        
        case WS2812_BASIC_MODE_ON :
            ws2812b_new_mode = WS2812_BASIC_MODE_ON;
            WS2812B_ON_All();
            break;
        
        
        case WS2812_MODE_WARM_TONE :
            WS2812B_warm_yellow();
            break;
        
        case WS2812_MODE_COOL_TONE :
            WS2812B_cool_white();
            break;
        default :
            break ;
    }   
}


void WS2812B_Basic_Mode(void)
{
    const  RGB_Color_TypeDef breathe_color[7] ={RED,ORANGE,YELLOW,GREEN,CYAN,BLUE,PURPLE};
    switch (ws2812b_basic_mode)
    {
        case WS2812_MODE_OFF :
            WS2812B_OFF_All();
            break;
        
        case WS2812_BASIC_MODE_ON :
            WS2812B_ON_All();
            break;
        
        
        case WS2812_MODE_WARM_TONE :
            WS2812B_warm_yellow();
            break;
        
        case WS2812_MODE_COOL_TONE :
            WS2812B_cool_white();
            break;
        
        case WS2812_Breathe_MODE :
            WS2812B_Breathe(64,breathe_color[breathe_color_num]);
            break;
        
        case WS2812_MODE_L :
            Brightness_Set(20);
            break; 

        case WS2812_MODE_MIOD :
            Brightness_Set(47);
            break;
        
        case WS2812_MODE_HIGH :
            Brightness_Set(70);
            break;
        default :
                break ;
            
       
        
        
            
     }
            
    
}


/**
  * @brief  WS2813E LED 示例任务函数
  * @note   演示 LED 静态颜色和呼吸灯效果
  * @retval 无
  */
void WS2812B_Task(void)
{
//	WS2812B_Init();  				// 配置 PWM、DMA 并初始化 LED 缓存
//	
//	while(1)
//	{
//       
            /*流水灯效果*/
    static uint8_t step = 0;
    WS2812B_Flowsingle(step ,RED,20);
    step = (step + 1) % WS2812B_NUM ;
            /* 结束 */
            
        
////        DWT_DelayS  (3);
//        WS2812B_Flow3group(64,20);
        
		//#if 1		//单个LED实验
    // 设置 LED0 为静态红色
        
//    WS2812B_SetLEDColor(0, RED);
//    HAL_Delay(2000);  // 保持红色 2 秒

    // 设置 LED0 为绿色呼吸灯效果
//    WS2812B_Breathe(0, GREEN, 5);  // delay_ms=5 控制呼吸速度

    // 设置 LED0 为静态蓝色
//    WS2812B_SetLEDColor(0, BLUE);
//    HAL_Delay(2000);  // 保持蓝色 2 秒

    // 设置 LED0 为紫色呼吸灯效果
//    WS2812B_Breathe(0, PURPLE, 5); // delay_ms=5 控制呼吸速度
		
		//#else 	//两个LED实验
			
		// 设置 LED0 为静态红色
		//WS2813E_SetLEDColor(0, RED);
		//WS2813E_Breathe(1, YELLOW, 5); // LED1 黄灯呼吸，速度参数 5

		// 设置 LED1 为静态绿色
		//WS2813E_SetLEDColor(1, GREEN);
		//WS2813E_Breathe(0, MAGENTA, 5); // LED0 品红呼吸，速度参数 5

		//#endif
		
//	}
}



/*****************************END OF FILE***************************************/

