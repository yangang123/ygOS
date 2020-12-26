
#include "r_device.h"
#include <stdio.h>
#include "led/r_led.h"
#include "system/r_system.h"
#include "intc/r_intc.h"


void bsp_init(void)
{   
    //初始化时钟
    R_SYSTEM_ClockInit();
    
    //初始化GPIO
    R_LED_GpioInit(LED1);
    
    //初始化定时器1ms触发中断,使能中断向量表
    R_SYSTEM_TimerInit();
    R_INTC_SetTableBit((uint16_t*)R_ICOSTM0);
    R_INTC_UnmaskInterrupt((uint16_t*)R_ICOSTM0);
    R_SYSTEM_TimerStart();
}


void INT_OSTM_handler(void)
{
    // 500ms翻转GPIO, 1HZ闪烁LED
    static int st = 0;
    static int cnt = 0;
    if (cnt >= 500){
      if (st) {
        R_LED_GpioOn(LED1);
        st =0;

      } else {
        R_LED_GpioOff(LED1);
        st = 1;
      }
      
      cnt = 0 ;
    } else {
      cnt++;
    }
    
    //定时任务调度
    ygos_timer_sche();
} 