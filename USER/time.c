#include "time.h"

void TIME_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*定时器TIM2时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到499为5ms*/
	TIM_TimeBaseStructure.TIM_Period = 499;   
	/*设置用来作为TIMx时钟频率除数的预分频值*/
	TIM_TimeBaseStructure.TIM_Prescaler =719;
  /*设置时钟分割:TDTS = Tck_tim*/  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	/*TIM向上计数模式*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
  /*使能或者失能指定的TIM中断*/
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	/*使能TIMx外设*/
	TIM_Cmd(TIM2, DISABLE);   
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	/*TIM2中断*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
  /*先占优先级2级*/	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  /*从优先级3级*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  /*IRQ通道被使能*/	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/
	NVIC_Init(&NVIC_InitStructure);  
}
