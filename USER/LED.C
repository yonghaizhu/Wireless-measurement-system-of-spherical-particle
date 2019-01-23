#include "LED.h"
#include "delay.h"

void LED_GPIO_Init(void)
{		
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   

	 GPIO_InitStructure.GPIO_Pin = LED_Pin;	    		 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOB,LED_Pin); 
	 delay_ms(50);
	 LED_OFF();
	 delay_ms(50);
	 LED_ON();
	 delay_ms(50);
	 LED_OFF();	
	 delay_ms(50);
	 LED_OFF();
	 delay_ms(50);
	 LED_ON();
	 delay_ms(50);
	 LED_OFF();	
}

