
#include "led.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

/* 初始化LED */
void led_init(void)
{
    SW_MUX_GPIO5_IO01 = 0x5;        /* 复用为GPIO5——IO03*/
    SW_PAD_GPIO5_IO01 = 0X10B0;     /*设置GPIO5_IO03电气属性 */

    /* GPIO初始化 */
    GPIO5_GDIR  |= 1<<1;             /* 设置为输出 */
    GPIO5_DR    &= ~(1<<1);            /* 打开LED灯 */
}

/* 短延时 */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* 延时，一次循环大概是1ms，在主频396MHz
 * n ：延时ms数
  */
void delay(volatile unsigned int n)
{
    while(n--) {
        delay_short(0x7ff);
    }
}

/* 打开LED灯 */
void led_on(void)
{
    GPIO5_DR &= ~(1<<1); /* bit3清零 */
}

/* 关闭LED灯 */
void led_off(void)
{
    GPIO5_DR |= (1<<1); /* bit3置1 */
}
