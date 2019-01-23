#ifndef _USART3_H
#define _USART3_H

#include "stm32f10x.h"
#include <stdio.h>

void USART3_Init(void);
int fputc(int ch, FILE *f);

#endif
