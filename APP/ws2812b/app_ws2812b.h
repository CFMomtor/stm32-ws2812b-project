#ifndef __APP_WS2812B_H__
#define __APP_WS2812B_H__


#include "main.h"
#include "ws2812b/bsp_ws2812b.h"



/* 静态颜色定义 ------------------------------ */
/* 定义常用 RGB 颜色值结构体，便于 LED 控制或显示使用 */
extern  const RGB_Color_TypeDef RED ;
extern  const RGB_Color_TypeDef ORANGE; 
extern  const RGB_Color_TypeDef YELLOW;
extern  const RGB_Color_TypeDef GREEN;
extern  const RGB_Color_TypeDef CYAN;
extern  const RGB_Color_TypeDef BLUE;
extern  const RGB_Color_TypeDef PURPLE;
extern  const RGB_Color_TypeDef BLACK;
extern  const RGB_Color_TypeDef WHITE;
extern  const RGB_Color_TypeDef MAGENTA;






/* 定义WS2812B的几种模式 */
typedef enum 
{
    WS2812_BASIC_MODE_ON,
    WS2812_MODE_OFF,
    WS2812_MODE_WARM_TONE,
    WS2812_MODE_COOL_TONE,
    WS2812_Breathe_MODE,
    WS2812_MODE_L,
    WS2812_MODE_MIOD,
    WS2812_MODE_HIGH,
    WS2812_MODE_RUNHORSE    
    
}WS2812B_BASIC_MODETypedef;


extern volatile WS2812B_BASIC_MODETypedef ws2812b_basic_mode;
extern WS2812B_BASIC_MODETypedef ws2812b_new_mode;
extern uint8_t breathe_color_num;
extern uint8_t breathe_color_falg ;



void WS2812B_Task(void);
void WS2812B_Basic_Mode(void);
void show_2812B_linght(WS2812B_BASIC_MODETypedef mode);



#endif /* __BSP_KEY_H__ */

