#ifndef _LED_H
#define _LED_H

/* 定义要使用的寄存器 */
#define CCM_CCGR0           *((volatile unsigned int *)0X020C4068)
#define CCM_CCGR1 			*((volatile unsigned int *)0X020C406C)
#define CCM_CCGR2 			*((volatile unsigned int *)0X020C4070)
#define CCM_CCGR3 			*((volatile unsigned int *)0X020C4074)
#define CCM_CCGR4 			*((volatile unsigned int *)0X020C4078)
#define CCM_CCGR5 			*((volatile unsigned int *)0X020C407C)
#define CCM_CCGR6 			*((volatile unsigned int *)0X020C4080)

/* 
 * IOMUX相关寄存器地址 
 */
#define SW_MUX_GPIO5_IO01 	*((volatile unsigned int *)0X020E0020)
#define SW_PAD_GPIO5_IO01 	*((volatile unsigned int *)0X020E02AC)

/* 
 * GPIO5相关寄存器地址 
 */
#define GPIO5_DR 			*((volatile unsigned int *)0X020AC000)
#define GPIO5_GDIR 			*((volatile unsigned int *)0X020AC004)
#define GPIO5_PSR 			*((volatile unsigned int *)0X020AC008)
#define GPIO5_ICR1 			*((volatile unsigned int *)0X020AC00C)
#define GPIO5_ICR2 			*((volatile unsigned int *)0X020AC010)
#define GPIO5_IMR 			*((volatile unsigned int *)0X020AC014)
#define GPIO5_ISR 			*((volatile unsigned int *)0X020AC018)
#define GPIO5_EDGE_SEL 		*((volatile unsigned int *)0X020AC01C)


void clk_enable(void);
void led_init(void);
void delay(volatile unsigned int n);
void led_on(void);
void led_off(void);


#endif 