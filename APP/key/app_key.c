#include "key/app_key.h"
#include "key/bsp_key.h"
#include "ws2812b/app_ws2812b.h"


void KEY_WS2812BMode(void )
{
    
    
    if( key_check (0,SINGLE))
    {
        static uint8_t key_count = 0;
        key_count ++;
        
        if(ws2812b_basic_mode != WS2812_Breathe_MODE)
        {
            switch(key_count)
            {
            case 1 :
                ws2812b_basic_mode = WS2812_BASIC_MODE_ON;
                break ;
            
            case 2 :
                ws2812b_basic_mode = WS2812_MODE_WARM_TONE;
                break ;
            
            case 3 :
                ws2812b_basic_mode = WS2812_MODE_COOL_TONE;
                key_count = 0;
                break ;
            } 
        }
        
              
     }   
     else if(key_check (0,DOUBLE)) 
     {
        static uint8_t key_count = 0;
        key_count ++;
        switch (key_count)
        {
            case 1 :
                ws2812b_basic_mode = WS2812_Breathe_MODE;
                break ;
            
            case 2 :
                ws2812b_basic_mode = WS2812_BASIC_MODE_ON;
                key_count = 0;
                break ;
        }
     }
     
     else if(key_check (0,LONG ))
     {
        ws2812b_basic_mode = WS2812_MODE_OFF;
     }
     
     //°´¼ü2
     if( key_check (1,SINGLE))
    {
        static uint8_t key_count = 0;
        key_count ++;
        
       
            switch(key_count)
            {
            case 1 :
                ws2812b_basic_mode = WS2812_MODE_L;               
                break ;
            case 2 :
                ws2812b_basic_mode = WS2812_MODE_MIOD;               
                break ;
            case 3 :
                ws2812b_basic_mode = WS2812_MODE_HIGH; 
                key_count = 0 ;
                break ;
            }         
              
     }   
     else if(key_check (1,DOUBLE)) 
     {
//        static uint8_t key_count = 0;
//        key_count ++;
//        switch (key_count)
//        {
//            case 1 :
//                ws2812b_basic_mode = WS2812_MODE_LINGHTMAX;
//                break ;
//            
//            case 2 :
//                ws2812b_basic_mode = WS2812_MODE_LINGHTMIN;
//                key_count = 0;
//                break ;
//        }
     }
     
     else if(key_check (1,LONG ))
     {
//        ws2812b_basic_mode = WS2812_MODE_LINGHTSUB;
     }
}

