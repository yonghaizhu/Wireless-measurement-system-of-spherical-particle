#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "config.h"

#define LED_OFF()   GPIO_SetBits(LED_Port,LED_Pin)
#define LED_ON()    GPIO_ResetBits(LED_Port,LED_Pin)   


void LED_GPIO_Init(void);   /* LED ¶Ë¿Ú³õÊ¼»¯ */

#endif /* __LED_H */
