#include "watchdog.h"

void WWDG_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(80);
	WWDG_Enable(127);
	WWDG_ClearFlag();
	WWDG_EnableIT();
}
