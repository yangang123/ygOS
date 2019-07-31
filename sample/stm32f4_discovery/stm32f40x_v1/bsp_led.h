#ifndef  _BSP_LED_H
#define  _BSP_LED_H

#include "board_config.h"

#define LED1_PIN_ON()        	 LED1_PIN_SET()
#define LED1_PIN_OFF()           LED1_PIN_RESET()

#define LED2_PIN_ON()        	 LED2_PIN_SET()
#define LED2_PIN_OFF()           LED2_PIN_RESET()

#define LED3_PIN_ON()        	 LED3_PIN_SET()
#define LED3_PIN_OFF()           LED3_PIN_RESET()

#define LED4_PIN_ON()        	 LED4_PIN_SET()
#define LED4_PIN_OFF()           LED4_PIN_RESET()

#define LED_ACT_PIN_ON()        	LED_ACT_PIN_SET()
#define LED_ACT_PIN_OFF()           LED_ACT_PIN_RESET()

#define LOW_VOLTAGE_PIN_ON()        LOW_VOLTAGE_PIN_SET()
#define LOW_VOLTAGE_PIN_OFF()       LOW_VOLTAGE_PIN_RESET()

#define POWER_LED_PIN_ON()        	POWER_LED_PIN_SET()
#define POWER_LED_PIN_OFF()         POWER_LED_PIN_RESET()

/* 定义RTK的状态灯  */
#define GPS_RTK_LED_BLUE_ON()	  LED5_PIN_SET()
#define GPS_RTK_LED_BLUE_OFF()    LED5_PIN_RESET()  
#define GPS_FIXED_LED_GREEN_ON()  LED6_PIN_SET()	
#define GPS_FIXED_LED_GREEN_OFF() LED6_PIN_RESET()
#define GPS_NO_FIX_LED_RED_ON()   LED7_PIN_SET()	
#define GPS_NO_FIX_LED_RED_OFF()  LED7_PIN_RESET()

extern void bsp_led_gpio_init(void);
extern void reg_led_gpio_init(void);
extern void bsp_reg_led_gpio_init(void);

#endif /* _BSP_LED_H */
