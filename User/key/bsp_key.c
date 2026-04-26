#include "key/bsp_key.h"
#include "tim_pwm/bsp_tim.h"
#include "ws2812b/bsp_ws2812b.h"
#include "ws2812b/app_ws2812b.h"


/*    7     6          5           4            3            2           1             0 */
/*      REPEA(重复) LONG(长按) DOUBLE(双击) SINGLE(单击) UP(松开时刻) DOWN(按下时刻) HOLD(按住不放) */


volatile  uint8_t KEY1_flay[KEY_NUM] = {0};  






void KEY_GPIO_Config(void)
{
   
    MX_TIM4_Init();
    
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIOA端口时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 设置引脚为上拉输入 */
    GPIO_InitStruct.Pin = KEY1_Pin | KEY2_Pin | KEY3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;     // 输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP;             // 上拉输入
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    // 低速输出
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

    
//    GPIO_InitStruct.Pin = KEY1_Pin ;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     // 输入模式
//    GPIO_InitStruct.Pull = GPIO_NOPULL;             // 上拉输入
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    // 低速输出
//    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);
//    HAL_GPIO_WritePin (KEY_GPIO_Port,KEY1_Pin,GPIO_PIN_RESET );
}

//


/**
  * @brief  扫描指定按键状态(按下，松开)
  * @param  GPIOx：其中 x 可以取值为（A 至 G，具体取决于所使用的设备），用于选择 GPIO 外围组件。
  * @note   无
  * @retval KEY_PRESSED 按键按下 KEY_UNPRESSED 按键松开
  */
uint8_t  KEY_GetState(uint8_t n )
{
    if(n == 0)
    {
        if(HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY1_Pin ) == GPIO_PIN_RESET)
        {
            return  KEY_PRESSED;
        }
    }
    else if(n == 1)
    {
       if(HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY2_Pin ) == GPIO_PIN_RESET)
        {
            return  KEY_PRESSED;
        } 
    }
    else if(n == 2)
    {
       if(HAL_GPIO_ReadPin (KEY_GPIO_Port ,KEY3_Pin ) == GPIO_PIN_RESET)
        {
            return  KEY_PRESSED;
        } 
    }
    
    return KEY_UNPRESSED ;
}



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





void key_Scan_10ms(void)
{
    static uint8_t key_cnt = 0;           //按键消抖计数器
    static uint8_t i = 0;
    static uint8_t CurrState[KEY_NUM],PrevState[KEY_NUM];   //当前按键状态，上一次按键状态
    static uint8_t S[KEY_NUM];                     //按键状态，单击 双击 长按 重复
    static uint16_t Time[KEY_NUM];                 //长按时间判断
    
    for(i = 0;i < KEY_NUM ;i++)
    {
        if(Time[i] > 0)
        {
            Time[i] --;
        }
    }
    
    
    key_cnt ++;
    if(key_cnt >=  3)
    {
        key_cnt = 0;
        
        for(i = 0 ; i < KEY_NUM ; i++)
        {
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
        
        if(CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
        {
           KEY1_flay[i] |= KEY_DOWN;
        }
        
        if(CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
        {
           KEY1_flay[i] |= KEY_UP ;
        }
        
        
        
        if(S[i] == 0)
        {
            if(CurrState[i] == KEY_PRESSED )
            {
                Time[i] = KEY_TIME_LONG ;            //设定长按时间
                S[i] = 1;
            }
        }
        else if(S[i] == 1)
        {
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
        }
        else if(S[i] == 2)
        {
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
            
        }
        else if(S[i] == 3)
        {
            if(CurrState[i] == KEY_UNPRESSED )
            {
                S[i] = 0;
            }
        }
        else if(S[i] == 4)
        {
            if(CurrState[i] == KEY_UNPRESSED)
            {
                S[i] = 0;
            }
            else if(Time[i] == 0)
            {
                Time[i] = KEY_TIME_REPEA ;       //设定重复时间 1s
                KEY1_flay[i] |= REPEA;
            }
        }
        
     
       }
    }    
}




//10ms定时器中断中调用
//void key_Scan_10ms(void )
//{
//    //判断按键几被按下
//    uint8_t KEY1 = HAL_GPIO_ReadPin (KEY_GPIO_Port,KEY1_Pin);
//    uint8_t KEY2 = HAL_GPIO_ReadPin (KEY_GPIO_Port,KEY2_Pin);
//    uint8_t KEY3 = HAL_GPIO_ReadPin (KEY_GPIO_Port,KEY3_Pin);
//    
//    //按键1状态机（消抖 + 长按判断）
//    static uint8_t key1_cnt = 0;
//    static KeyStateTypedef key1_state = KEY_STATE_RELEASE;
//    
//    if (KEY1 == 0)//
//    {
//       
//        
//       if(/*key1_cnt >= 3*/key1_cnt < 255)
//       {
//           key1_cnt ++;
//            if(key1_cnt >= 3 && key1_state == KEY_STATE_RELEASE)
//            {
//                key1_state = KEY_STATE_PRESS;   //切换按键为按下状态
//                /* 单击按键发生事件 */
////               
//                KEY1_flay = 1;
//                
//            }
//            //长按判断 >=50*10ms=500ms
//            if (key1_state == KEY_STATE_PRESS && key1_cnt >= 50)
//            {
//                key1_state = KEY_STATE_LONG_PRESS;    //切换按键为长按状态
//                /* 长按按键发生事件 */
//                KEY1_flay = 2;
//                
//            }   
//       }
//       
//    }
//    else 
//       {
//           
//           key1_cnt = 0;      //清零计数器
//           key1_state = KEY_STATE_RELEASE;   //复位按键1的状态为松开
//       }
//    
//    //按键2状态机（消抖 + 长按判断）
//    static uint8_t key2_cnt = 0;
//    static KeyStateTypedef key2_state = KEY_STATE_RELEASE;
//    
//    if (KEY2 == 0)//
//    {
//       key2_cnt ++;
//       if(key2_cnt >= 3)
//       {
//            if(key2_state == KEY_STATE_RELEASE)
//            {
//                key2_state = KEY_STATE_PRESS;   //切换按键2为按下状态
//                /* 单击按键2发生事件 */
////                WS2812B_Flow3group(64,20);
//                
//            }
//            //长按判断 >=50*10ms=500ms
//            else if (key2_state == KEY_STATE_PRESS && key2_cnt >= 50)
//            {
//                key2_state = KEY_STATE_LONG_PRESS;    //切换按键2为长按状态
//                /* 长按按键2发生事件 */
//                
//                
//            }   
//       }
//       else 
//       {
//           key2_cnt = 0;      //清零计数器
//           key2_state = KEY_STATE_RELEASE;   //复位按键2的状态为松开
//       }
//    }
//    
//   
//    
//    //按键3状态机（消抖 + 长按判断）
//    static uint8_t key3_cnt = 0;
//    static KeyStateTypedef key3_state = KEY_STATE_RELEASE;
//    
//    if (KEY3 == 0)//
//    {
//       key3_cnt ++;
//       if(key3_cnt >= 3)
//       {
//            if(key3_state == KEY_STATE_RELEASE)
//            {
//                key3_state = KEY_STATE_PRESS;   //切换按键3为按下状态
//                /* 单击按键3发生事件 */
//                
//                
//            }
//            //长按判断 >=50*10ms=500ms
//            else if (key3_state == KEY_STATE_PRESS && key3_cnt >= 50)
//            {
//                key3_state = KEY_STATE_LONG_PRESS;    //切换按键3为长按状态
//                /* 长按按键3发生事件 */
//                
//                
//            }   
//       }
//       else 
//       {
//           key3_cnt = 0;      //清零计数器
//           key3_state = KEY_STATE_RELEASE;   //复位按键3的状态为松开
//       }
//    }
//    
//    
//}


//定时器中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim ->Instance  == TIM4  )
    {
        //10ms到，调用按键扫描函数
        key_Scan_10ms();
        ws2812_dir_step();
    }
}
