#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f10x.h"



typedef enum 
{
	None = 0x00,            //标志位致0
	SendData = 0x01,        //通过WiFi发送数据
	Stop = 0x02,            //停止
	SendAndWrite = 0x03,    //通过WiFi发送数据并将数据保存于W25Q128
	Read = 0x04,	          //从W25Q128中读取数据
	Erase = 0x05,           //整片擦除W25Q128
	Adjust = 0x06,          //调整陀螺仪的零漂
	SoftReset = 0x07,       //软件复位系统
	Read_USART1 = 0x08     //通过串口1发送数据
}StatusFlag;

void ESP8266_Init(void);
void Wifi_GPIO_Init(void);
void USART3_Cmd(u8 *usartdata, u8 *pattern_flag);
void sys_cmd(u8 *write_flag, u8 *pattern_flag);

#endif
