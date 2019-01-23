#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
//#include <stdio.h>

void USART1_Init(void);
void USART1_GPIOConfig(void);
void UART1_DMACon(void);
void NVIC_Configuration(void);
void USART_printf ( USART_TypeDef *USARTx, char * Data, ... );
//int fputc(int ch, FILE *f);


#endif /* __USART1_H */
