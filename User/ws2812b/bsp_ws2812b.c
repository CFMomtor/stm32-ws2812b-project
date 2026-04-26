#include "ws2812b/bsp_ws2812b.h"
#include "ws2812b/app_ws2812b.h"
#include "tim_pwm/bsp_tim.h"
#include "dma/bsp_dma.h"
#include "usart/bsp_usart.h"


//伽马校正表(0～180)
const uint8_t gamma_table_180 [181] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
    1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  4,  4,
    4,  5,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, 11, 12,
   12, 13, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 22, 23, 24,
   24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 35, 36, 37, 38, 39, 41,
   42, 43, 44, 46, 47, 48, 50, 51, 53, 54, 56, 57, 59, 60, 62, 64,
   65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 87, 89, 91, 93,
   95, 97, 100,102,104,106,109,111,113,116,118,120,123,125,128,130,
   133,135,138,141,143,146,149,151,154,157,160,163,165,168,171,174,
   177,180,183,186,189,192,195,198,201,204,207,210,213,216,219,222,
   225,228,231,235,238,241,244,248,251,254,255,255,255,255,255,255,
   255,255,255,255
};




/**  G：0000 0000  R：0000 0000 B：0000 0000 RESET：>=280us  即280/1.25 = 224位复位数据
  *
*/

/* 亮度百分比，初始值为 47% 对应中档 */
static  uint8_t linght_num = 47;

uint16_t ws2812b_buf[WS2812B_BUF_LEN];  // 发送的数据缓冲区


//uint8_t ws_tx_busy = 0;
uint8_t i = 0;             //呼吸灯的亮度值0 ～ 180
int8_t ws2812_dir = 1;     //呼吸灯方向 1自加 -1 自减




//LED颜色值数组

uint8_t led_buf[WS2812B_NUM] [3];
uint16_t led_count = 0;

//led_buf[i][0] = G通道，led_buf[i][1] = R通道， led_buf[i][2] = B通道

//初始化：把复位电平先填满
void WS2812B_Init(void) 
{
   MX_DMA_Init();
   MX_TIM2_Init();
//   MX_USART1_UART_Init();
//   HAL_TIM_Base_Start_IT(&htim4); 
    
    
    for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 0;
    
        led_buf[i][1] = 0;
    
        led_buf[i][2] = 0;
     
    }
    
      
}


//  led      g          r         b
//   2   1111 1111  1111 1111 0000 0000
//
//0000000001


//伽马公式,优化呼吸灯效果 Lout = Lin的2.2次方/255的2.2次方 * 255
uint8_t Gamma_Int(uint8_t in)
{
   if(in > 180) in = 180;
    return gamma_table_180[in] ;
}




//设置百分比
void Brightness_Set(uint8_t percent)
{
    if(percent > 70)
        linght_num = 70;
    linght_num = percent;
    show_2812B_linght(ws2812b_new_mode);
}

//设置每次亮度加减
void Bringhtness_Step(int8_t step)
{
    int16_t new_linght = (int16_t )linght_num + step;
    if(new_linght < 0)
        new_linght = 0;
    if(new_linght > 70)
        new_linght = 70;
    linght_num = (uint8_t )new_linght;
    
    show_2812B_linght(ws2812b_new_mode);
}


//设置ws2812b的缓冲区
static  void WS2812B_Setcolor_single( void )
{
    uint16_t i ,j;
    uint16_t pos = RESET_WORD ;  //DMA缓冲区偏移，前面保留复位信号
//    uint32_t color = (uint32_t)(g) << 16 | (uint32_t )(r ) << 8 | b ;     //1111 1111  1111 1111 0000 0000
    
    
    //遍历每个LED
    for (i = 0 ; i < WS2812B_NUM ; i ++)
    {
        //遍历每个颜色通道的八位数据
        for (j = 0 ; j < 8 ; j++)
            {
                
                ws2812b_buf[pos + j]      = (led_buf [i][0] & (0x80 >> j )) ? DATUM_1 : DATUM_0;
                ws2812b_buf[pos + j + 8]  = (led_buf [i][1] & (0x80 >> j )) ? DATUM_1 : DATUM_0;
                ws2812b_buf[pos + j + 16] = (led_buf [i][2] & (0x80 >> j )) ? DATUM_1 : DATUM_0;

            }
            pos += 24;
    }
        
    for (i = 0 ; i < DUMMY_WORD ; i++)
    {
        ws2812b_buf[pos + i] = 0;
    }
    

}    


//发送刷新
void WS2812B_Flush(void )
{
//    if(ws_tx_busy != 0)
//        return ;
//    ws_tx_busy = 1;
//    
//    HAL_TIM_PWM_Stop_DMA (&htim2  ,TIM_CHANNEL_1);
    
    WS2812B_Setcolor_single();
//    if(htim2.State != HAL_TIM_STATE_READY )
//    {
//        while(1);
//    }
//    
    //启动DMA发送PWM
      HAL_StatusTypeDef status =  HAL_TIM_PWM_Start_DMA (&htim2  ,TIM_CHANNEL_1,(uint32_t *)ws2812b_buf,sizeof (ws2812b_buf)/sizeof (ws2812b_buf[0]));
//     
}    



//DMA发送完成中断回调
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance  == TIM2  )
    {
        HAL_TIM_PWM_Stop_DMA (htim ,TIM_CHANNEL_1 );
    }
    

}


//关闭WS2812B灯
void WS2812B_OFF_All(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 0;
    
        led_buf[i][1] = 0;
    
        led_buf[i][2] = 0;
        
        WS2812B_Flush();
    } 
}



//点亮WS2812B(中性白,适合办公读书学习)  4000K
void WS2812B_ON_All(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 245  * linght_num / 100;   //G
    
        led_buf[i][1] = 255  * linght_num / 100;   //R
    
        led_buf[i][2] = 230  * linght_num / 100;   //B
        
        WS2812B_Flush();
    } 
}

//点亮WS2812B(经典暖黄,睡前、卧室、放松场景)  2700K
void WS2812B_warm_yellow(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 210 * linght_num / 100;   //G
    
        led_buf[i][1] = 255 * linght_num / 100;   //R
    
        led_buf[i][2] = 150 * linght_num / 100;   //B
        
        WS2812B_Flush();
    } 
}



//点亮WS2812B(暖白，日常阅读、客厅)  3000K
void WS2812B_warm_white(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 225 * linght_num / 100;   //G
    
        led_buf[i][1] = 255 * linght_num / 100;   //R
    
        led_buf[i][2] = 180 * linght_num / 100;   //B
        
        WS2812B_Flush();
    } 
}



//点亮WS2812B(冷白，专注学习、精细作业)  5000K
void WS2812B_cool_white(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 245 * linght_num / 100;   //G
    
        led_buf[i][1] = 240 * linght_num / 100;   //R
    
        led_buf[i][2] = 255 * linght_num / 100;   //B
        
        WS2812B_Flush();
    } 
}



//点亮WS2812B(日光冷白，强光需求、白天补光)  6500K
void WS2812B_cool_sunlight_white(void )
{
   for(uint16_t i = 0 ; i < WS2812B_NUM ; i++)
    {   
        led_buf[i][0] = 240 * linght_num / 100;   //G
    
        led_buf[i][1] = 230 * linght_num / 100;   //R
    
        led_buf[i][2] = 255 * linght_num / 100;   //B
        
        WS2812B_Flush();
    } 
}







//设置指定LED为静态颜色
void WS2812B_SetLEDColor (uint16_t index,const RGB_Color_TypeDef color)
{
    if(index >= WS2812B_NUM )return ;
    
    led_buf [index][1] = color.R * linght_num / 100 ;
    led_buf [index][0] = color.G * linght_num / 100;
    led_buf [index][2] = color.B * linght_num / 100;
    
    WS2812B_Flush();
}


//呼吸灯方向改变函数，在TIM4中断回调函数中调用
void ws2812_dir_step(void )
{
    i += ws2812_dir ;
    if(i >= MAX_BRIGHT)
    {
        i = MAX_BRIGHT;
        ws2812_dir = -1;
        breathe_color_falg = 1;
    }
    else if(i <= 0)
    {
        i = 0;
        ws2812_dir = 1;
        if(breathe_color_falg)
        {
            breathe_color_num = (breathe_color_num + 1) % 7;
            breathe_color_falg = 0;
        }
    }
}




//呼吸灯
void WS2812B_Breathe(uint16_t index ,const RGB_Color_TypeDef color)
{
//    if(index >= WS2812B_NUM )return ;
    
    
    
    
    uint8_t line = (uint8_t ) ((uint16_t )i * 180 / MAX_BRIGHT);  //将呼吸变量映射到伽马表的 0 ～ 180范围
    uint8_t gamma_val = Gamma_Int(line );  //查表取出的伽马输出值
    
//    uint8_t new_color_brt = (uint16_t )gamma_val * MAX_BRIGHT /255 ;   //把伽马输出值映射回0 ～ 180 限制峰值
    
    RGB_Color_TypeDef temp;
    
//    for (i=0;i <= 180;i++) 
//    {
        temp.R  = (color.R * gamma_val) / 255;
        temp.G   = (color.G * gamma_val) / 255;
        temp.B = (color.B * gamma_val) / 255;
        
        for (uint8_t j = 0; j < index ;j ++)
        {
            led_buf [j][1] = temp.R ;
            led_buf [j][0] = temp.G ;
            led_buf [j][2] = temp.B ;
        }
        
         WS2812B_Flush();
        //HAL_Delay (delay_ms );
    //}   
    
    
    
//    for (i=0;i <= 180;i++)
//    {
//        temp.R  = (color.R * (255 - i)) / 255;
//        temp.G   = (color.G * (255 - i)) / 255;
//        temp.B = (color.B * (255 - i)) / 255;
//        
//        for(uint8_t j = 0; j < index ;j ++)
//        {
//            led_buf [j][1] = temp.R ;
//            led_buf [j][0] = temp.G ;
//            led_buf [j][2] = temp.B ;
//        }
//        
        //WS2812B_Flush();
//        HAL_Delay (delay_ms );
    //}    
    
    
}





    /* 单色 led 灯流水效果 (上下流水灯跑马效果) */

void WS2812B_Flowsingle(uint16_t index ,const RGB_Color_TypeDef color,uint8_t delay_ms  )
{
//    WS2812B_OFF_All();
     for(uint8_t i = 0;i<index ;i++)
        {
            WS2812B_SetLEDColor(i,color);
            HAL_Delay (delay_ms );                            
        }        
        WS2812B_OFF_All(); 
}        
                

//设置三种颜色每三颗灯为一组切换点亮       
void WS2812B_Flow3group(uint16_t index , uint8_t delay_ms  )
{
    uint8_t LED_Count = 0 ;
    const RGB_Color_TypeDef colornum[3] = {
        {255,0,0},  //红色
        {0,255,0},  //绿色
        {0,0,255}   //蓝色
    };
    
      WS2812B_OFF_All();
     for(uint8_t i = 0;i<index ;i++)
        {
            
            WS2812B_SetLEDColor(i,colornum[led_count]);
            HAL_Delay (delay_ms );
            LED_Count ++;
            if(LED_Count<3)
                led_count = 0;
            else if(LED_Count < 6)
                 led_count = 1;
            else if(LED_Count < 9)
                 led_count = 2;
            else
            {                
                LED_Count = 0;
                 led_count = 0;
            }               
        }        
    
        WS2812B_Flush();
    
}
        
        
        






