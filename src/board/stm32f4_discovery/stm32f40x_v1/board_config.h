
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H
 
#include "stm32f4xx.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define LED1_PIN            	 GPIO_Pin_12
#define LED1_PIN_PORT       	 GPIOD
#define LED1_PIN_RCC         	 RCC_AHB1Periph_GPIOD
#define LED1_PIN_RESET()         LED1_PIN_PORT->BSRRH = LED1_PIN
#define LED1_PIN_SET()           LED1_PIN_PORT->BSRRL = LED1_PIN

#define LED2_PIN            	 GPIO_Pin_13
#define LED2_PIN_PORT       	 GPIOD
#define LED2_PIN_RCC         	 RCC_AHB1Periph_GPIOD
#define LED2_PIN_RESET()         LED2_PIN_PORT->BSRRH = LED2_PIN
#define LED2_PIN_SET()           LED2_PIN_PORT->BSRRL = LED2_PIN

#define LED3_PIN            	 GPIO_Pin_14
#define LED3_PIN_PORT       	 GPIOD
#define LED3_PIN_RCC         	 RCC_AHB1Periph_GPIOD
#define LED3_PIN_RESET()         LED3_PIN_PORT->BSRRH = LED3_PIN
#define LED3_PIN_SET()           LED3_PIN_PORT->BSRRL = LED3_PIN
                                    
#define LED4_PIN            	 GPIO_Pin_15
#define LED4_PIN_PORT       	 GPIOD
#define LED4_PIN_RCC         	 RCC_AHB1Periph_GPIOD
#define LED4_PIN_RESET()         LED4_PIN_PORT->BSRRH = LED4_PIN
#define LED4_PIN_SET()           LED4_PIN_PORT->BSRRL = LED4_PIN

#endif

