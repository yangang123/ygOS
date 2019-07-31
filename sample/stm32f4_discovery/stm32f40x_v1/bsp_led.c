
#include "bsp_led.h"

void bsp_led_gpio_init(void)
{
	GPIO_InitTypeDef  		GPIO_InitStructure;  
	
	RCC_AHB1PeriphClockCmd(LED1_PIN_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED1_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED1_PIN_PORT, &GPIO_InitStructure);	
	
	RCC_AHB1PeriphClockCmd(LED2_PIN_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED2_PIN ;
	GPIO_Init(LED2_PIN_PORT, &GPIO_InitStructure);		
	
	RCC_AHB1PeriphClockCmd(LED3_PIN_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED3_PIN ;
	GPIO_Init(LED3_PIN_PORT, &GPIO_InitStructure);
			
	RCC_AHB1PeriphClockCmd(LED4_PIN_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED4_PIN ;
	GPIO_Init(LED4_PIN_PORT, &GPIO_InitStructure);
	

	LED1_PIN_OFF();
	LED2_PIN_OFF();	
	LED3_PIN_OFF();
	LED4_PIN_OFF();	
	
	
	
}

