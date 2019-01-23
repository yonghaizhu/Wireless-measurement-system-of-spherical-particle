#include "time.h"

void TIME_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*��ʱ��TIM2ʱ��ʹ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /*��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,������499Ϊ5ms*/
	TIM_TimeBaseStructure.TIM_Period = 499;   
	/*����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/
	TIM_TimeBaseStructure.TIM_Prescaler =719;
  /*����ʱ�ӷָ�:TDTS = Tck_tim*/  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	/*TIM���ϼ���ģʽ*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  /*����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ*/
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
  /*ʹ�ܻ���ʧ��ָ����TIM�ж�*/
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	/*ʹ��TIMx����*/
	TIM_Cmd(TIM2, DISABLE);   
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	/*TIM2�ж�*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
  /*��ռ���ȼ�2��*/	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  /*�����ȼ�3��*/	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  /*IRQͨ����ʹ��*/	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	/*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���*/
	NVIC_Init(&NVIC_InitStructure);  
}
