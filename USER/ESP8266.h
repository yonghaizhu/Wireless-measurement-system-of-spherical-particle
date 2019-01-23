#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"



typedef enum 
{
	None = 0x00,            //��־λ��0
	SendData = 0x01,        //ͨ��WiFi��������
	Stop = 0x02,            //ֹͣ
	SendAndWrite = 0x03,    //ͨ��WiFi�������ݲ������ݱ�����W25Q128
	Read = 0x04,	          //��W25Q128�ж�ȡ����
	Erase = 0x05,           //��Ƭ����W25Q128
	Adjust = 0x06,          //���������ǵ���Ư
	SoftReset = 0x07,       //�����λϵͳ
	Read_USART1 = 0x08     //ͨ������1��������
}StatusFlag;

void ESP8266_Init(void);
void Wifi_GPIO_Init(void);
void USART3_Cmd(u8 *usartdata, u8 *pattern_flag);
void sys_cmd(u8 *write_flag, u8 *pattern_flag);

#endif
