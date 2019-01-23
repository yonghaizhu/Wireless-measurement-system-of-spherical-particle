#include "usart1.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#define USART1_DR_Base 0x40013804
#define SENDBUFF_SIZE 5000
uint8_t SendBuff[SENDBUFF_SIZE];

/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* 配置串口1 （USART1） 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/*串口GPIO端口配置*/
  /* 配置串口1 （USART1 Tx (PA.09)）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* 配置串口1 USART1 Rx (PA.10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
		/* 串口1工作模式（USART1 mode）配置 */
	USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);//使能串口
}
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Configure and enable ADC interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

void UART1_DMACon(void)
{
	 DMA_InitTypeDef  DMA_InitStructure;
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 NVIC_Configuration();
  /* USART1_DMA1_Channel4 Config */
   DMA_DeInit(DMA1_Channel4);
	 DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
   DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
   DMA_InitStructure.DMA_BufferSize = 10;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel4, &DMA_InitStructure);
   /* Enable USART1_DMA1_Channel4 Transfer complete interrupt */
   DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
   
   /* USART1_DMA1_Channel4 enable */
   DMA_Cmd(DMA1_Channel4, ENABLE);
}
void USART1_Init(void)
{
	 USART1_GPIOConfig();
	 UART1_DMACon();
}
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
//int fputc(int ch, FILE *f)
//{
//	/* 将Printf内容发往串口 */
//	USART_SendData(USART1, (unsigned char) ch);
//	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
//	return (ch);
//}
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void USART_printf ( USART_TypeDef *USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16] = {'\0'};
	
	va_list ap;
	va_start(ap, Data);
 
	while ( * Data != 0 )     
	{				                          
		if ( * Data == 0x5c ) 
		{									  
			switch ( *++Data )
			{
				case 'r':							          
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;
 
				case 'n':							         
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;
 
				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  
			switch ( *++Data )
			{				
				case 's':										 
				s = va_arg(ap, const char *);
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx, *s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}			
				Data++;							
				break;
 
				case 'd':					
				d = va_arg(ap, int);
				itoa(d, buf, 10);	
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}							
				Data++;					
				break;
								
				default:
				Data++;
				break;
				
			}		 
		}	
		else USART_SendData(USARTx, *Data++);
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );	
	}
}
