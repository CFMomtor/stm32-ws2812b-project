#include "key/bsp_key.h"
#include "tim_pwm/bsp_tim.h"
#include "ws2812b/bsp_ws2812b.h"
#include "ws2812b/app_ws2812b.h"


/*    7     6          5           4            3            2           1             0 */
/*      REPEA(重复) LONG(长按) DOUBLE(双击) SINGLE(单击) UP(松开时刻) DOWN(按下时刻) HOLD(按住不放) */

/**
 * @brief  按键状态标志数组
 * @note   依次对应：单击、双击、长按、连续重复触发标志
 */
volatile  uint8_t KEY1_flay[KEY_NUM] = {0};  





/**
 * @brief  按键GPIO端口底层初始化配置
 * @param  无
 * @retval 无
 * @note   配合定时器TIM4实现按键多模式检测
 */
void KEY_GPIO_Config(void)
{
   
    MX_TIM4_Init();                               //初始化按键扫描定时基础时钟
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};       //结构体变量内存清零

    /* 使能GPIOB端口时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 设置引脚为上拉输入 */
    GPIO_InitStruct.Pin = KEY1_Pin | KEY2_Pin | KEY3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                
    GPIO_InitStruct.Pull = GPIO_PULLUP;                       
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);
    
}




/**
  * @brief  获取按键当前状态(按下，松开)
  * @param  n : 按键索引号 0 ～ 2
  * @retval KEY_PRESSED 按下 / KEY_UNPRESSED 按键松开
  * @note   按键上拉输入，低电平有效触发按下
  */
uint8_t  KEY_GetState(uint8_t n )
{
    GPIO_PinState key_val;
    switch ( n )
    {
        case 0 : key_val = HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY1_Pin ); break ;
        case 1 : key_val = HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY2_Pin ); break ;
        case 2 : key_val = HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY3_Pin ); break ;
        default : return KEY_UNPRESSED;
    }
    //低电平判断按键按下
     if(key_val == GPIO_PIN_RESET)
    {
       return  KEY_PRESSED;
    }        
       
    return KEY_UNPRESSED ;
    
}    
   

/**
 * @brief  按键组合标志位匹配检测
 * @param  n: 按键索引号 0 ～ 2
 * @param  Flag: 需要判断的按键行为掩码标志 KEY_HOLD KEY_DOWN KEY_UP LONG DOUBLE SINGLE REPEA
 * @retval 1:标志匹配成功 0:标志未匹配
 * @note   匹配成功后自动清除单次触发类标志，长按保持标志不清除
 */
uint8_t key_check(uint8_t n ,uint8_t Flag)
{
    if(KEY1_flay[n] & Flag )
    {
        if(Flag != KEY_HOLD)
        {
            KEY1_flay[n] &= ~Flag ;
        }
        return 1;
    }
    return 0;
}






/**
 * @brief  按键状态切换扫描
 * @param  无
 * @retval 无
 */
void key_Scan_10ms(void)
{
    static uint8_t key_cnt = 0;                             //按键消抖计数器
    static uint8_t i = 0;                                   //按键按下时间计数器
    static uint8_t CurrState[KEY_NUM],PrevState[KEY_NUM];   //当前按键状态，上一次按键状态
    static uint8_t S[KEY_NUM];                              //按键状态，单击 双击 长按 重复
    static uint16_t Time[KEY_NUM];                          //长按时间判断
    
    /* 全局倒计时统一递减 */
    for(i = 0;i < KEY_NUM ;i++)
    {
        if(Time[i] > 0)
        {
            Time[i] --;
        }
    }
    
    
    key_cnt ++;
    if(key_cnt >=  3)       //3次消抖采样
    {
        key_cnt = 0;
        
        for(i = 0 ; i < KEY_NUM ; i++)
        {
            /* 保留历史 + 读取最新电平 */
            PrevState[i] = CurrState[i] ;
            CurrState[i] = KEY_GetState (i);
            
            if(CurrState[i] == KEY_PRESSED) 
            {
                KEY1_flay[i] |= KEY_HOLD ;
            }
            else 
            {
                KEY1_flay[i] &= ~KEY_HOLD;
            }
            
            /*  边沿捕获 上升沿/下降沿*/
            if(CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
            {
               KEY1_flay[i] |= KEY_DOWN;
            }
            
            if(CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
            {
               KEY1_flay[i] |= KEY_UP ;
            }
            
        
            switch (S[i])
             {
                 case 0 : 
                    if(CurrState[i] == KEY_PRESSED )
                    {
                        Time[i] = KEY_TIME_LONG ;            //设定长按时间
                        S[i] = 1;
                    };
                    break ;
                 case 1 :
                     if(CurrState[i] == KEY_UNPRESSED )
                    {
                        Time[i] = KEY_TIME_DOUBLE ;        //设定双击时间200ms
                        S[i] = 2;
                    }
                    else if(Time[i] == 0)
                    {
                        Time[i] = KEY_TIME_REPEA ;       //设定重复时间 1s
                        KEY1_flay[i] |= LONG;
                        S[i] = 4;
                    }
                    break ;
                 case 2 : 
                     if(CurrState[i] == KEY_PRESSED )
                     {
                        KEY1_flay[i] |= DOUBLE;
                        S[i] = 3;
                    
                     }
                    else if(Time[i] == 0)
                    {
                        KEY1_flay[i] |= SINGLE;
                        S[i] = 0;
                        
                    }
                    break ;
                 case 3 :
                     if(CurrState[i] == KEY_UNPRESSED )
                    {
                    S[i] = 0;
                    }
                    break ;
                 case 4 :
                     if(CurrState[i] == KEY_UNPRESSED)
                    {
                        S[i] = 0;
                    }
                    else if(Time[i] == 0)
                    {
                        Time[i] = KEY_TIME_REPEA ;       //设定重复时间 1s
                        KEY1_flay[i] |= REPEA;
                    }
                    break ;
                 default : S[i] = 0; break ;   
                        
             }
            
        }

     }   
}      
        

       
        

/**
 * @brief  定时器周期溢出中断回调函数
 * @param  htim: 定时器外设句柄指针
 * @retval 无
 * @note   TIM4定时周期10ms，周期性执行按键扫描与灯带步进逻辑
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim ->Instance  == TIM4  )
    {
        
        key_Scan_10ms();      //10ms定时按键时序消抖、多模式状态扫描
        ws2812_dir_step();    //灯带渐变流水步数周期更新
    }
}




