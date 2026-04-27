#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__


#include "main.h"


#define KEY_NUM     3

#define KEY1_Pin         GPIO_PIN_0
#define KEY_GPIO_Port   GPIOB

#define KEY2_Pin         GPIO_PIN_1
#define KEY_GPIO_Port   GPIOB

#define KEY3_Pin         GPIO_PIN_2
#define KEY_GPIO_Port   GPIOB




#define KEY_PRESSED         1    //按下为1
#define KEY_UNPRESSED       0    //未按下是0


#define KEY_TIME_DOUBLE    20
#define KEY_TIME_LONG      200
#define KEY_TIME_REPEA     100


#define KEY_HOLD     0x01
#define KEY_DOWN     0x02
#define KEY_UP       0x04
#define LONG         0x20
#define DOUBLE       0x10
#define SINGLE       0x08
#define REPEA        0x40

extern volatile uint8_t KEY1_flay[3];






/* 按键状态机定义 */
typedef enum {
    KEY_STATE_RELEASE,        //松开
    KEY_STATE_PRESS,         //按下（消抖）
    KEY_STATE_LONG_PRESS     //长按
}KeyStateTypedef;


/* 按键端口结构体 */
typedef struct {
    GPIO_TypeDef GPIOx;
    uint16_t GPIO_Pin;
}GPIOx_PinTypedef;




/* 函数声明 */
void KEY_GPIO_Config(void);
uint8_t  KEY_GetState(uint8_t n );
uint8_t key_check(uint8_t n ,uint8_t Flag);
void key_Scan_10ms(void );
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif /* __BSP_KEY_H__ */

