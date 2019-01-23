#ifndef _DATAPRO_H
#define _DATAPRO_H

#include "stm32f10x.h"

#define OK    1
#define ERROR 0

void Write_W25Q128(void);
u8 Read_W25Q128(void);
u8 Read_W25Q128_USART1(void);
void Erase_W25Q128_4K(void);
void Erase_W25Q128_Bulk(void);
#endif
