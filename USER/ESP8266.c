#include "ESP8266.h"
#include "usart3.h"
#include "delay.h"
#include "MPU9250.h"
#include "datapro.h"
#include "LED.h"
#include "sysreset.h"
#include "config.h"

StatusFlag statusflag;
extern u8 data_cnt;
extern u8 led_time;

void Wifi_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_12;	    		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA,GPIO_Pin_2); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void ESP8266_Init(void)
{
	printf("\r\n");
	printf("颗粒运动测量系统\r\n");
	printf("固件版本号：%s\r\n", firmware_ID);
	printf("更新时间：%s\r\n", update_Time);
	printf("设备号：%s\r\n", dataHeader);
	printf("FLASH芯片容量：16MB\r\n");
	printf("采样频率：200Hz\r\n");
	printf("加速度：±16G\r\n");
	printf("陀螺仪：±2000dps\r\n");
	printf("上海理工大学 清华大学\r\n");
	printf("设备准备就绪...\r\n");	
}

void USART3_Cmd(u8 *usartdata, u8 *pattern_flag)
{

	switch(*usartdata)
		{
//			case START:			
//				statusflag = SendData;
//			  *pattern_flag = 1;
//		  	
//			  TIM_Cmd(TIM2, ENABLE);
//			  break;
			
			case STOP:
				statusflag = Stop;
			  break;
			
			case STARTANDWRITE:
				statusflag = SendAndWrite;
				data_cnt = 0;
				*pattern_flag = 2;
				MPU9250_Buffer_Reset();
			  printf("%s START!!!\r\n", dataHeader);
				TIM_Cmd(TIM2, ENABLE);
			  TIM_Cmd(TIM3, ENABLE);
			  break;
			
			case READ:
				statusflag = Read; 
				break;
			
			case SENDBYUSART1:
				statusflag = Read_USART1;
				break;
			
			case ERASE:
				statusflag = Erase; 
				break;
			
			case ADJUST:
			  statusflag = Adjust; 
				break;
				
			case SOFTRESET:
				statusflag = SoftReset; 
				break;
						
			case device_ID:
				statusflag = Read_USART1; 
				break;
			
			default:
        break;				
		}
}
void sys_cmd(u8 *write_flag, u8 *pattern_flag)
{
	if(statusflag == Stop)
	{
		TIM_Cmd(TIM2, DISABLE);	
		TIM_Cmd(TIM3, DISABLE);
		printf("%s STOP !!!\r\n", dataHeader);
		MPU9250_Buffer_Reset();
		*pattern_flag = 0;
		LED_ON();
		led_time = 0;
		statusflag = None;
	}	
		
	else if(*write_flag == 1)
	{
		Write_W25Q128();
		*write_flag = 0;		
	}
	
	else if(statusflag == Read)
	{
		TIM_Cmd(TIM2, DISABLE);
		printf("\r\n %s 开始读取W25Q128! \r\n", dataHeader);
		LED_OFF();
		while(Read_W25Q128() == 1);
		printf("\r\n %s 读取结束！！！ \r\n", dataHeader);
		LED_ON();
		statusflag = None;
		delay_ms(500);
		Erase_W25Q128_4K();
	}	
	else if(statusflag == Read_USART1)
	{
		TIM_Cmd(TIM2, DISABLE);
		printf("\r\n %s 通过USART1读取W25Q128! \r\n", dataHeader);
		LED_OFF();
		while(Read_W25Q128_USART1() == 1);
		printf("\r\n %s 通过USART1读取结束！！！ \r\n", dataHeader);
		LED_ON();
		statusflag = None;
		delay_ms(500);
		Erase_W25Q128_4K();
	}	

	else if(statusflag == Erase)
	{
		Erase_W25Q128_Bulk();     //整片擦除W25Q128
		statusflag = None;
	}	

	else if(statusflag == Adjust)
	{
		MPU9250_Adjust_GYRO();  //调整陀螺仪的零漂
		statusflag = None;
	}
	
	else if(statusflag == SoftReset)
	{
		Soft_Reset_System();        //系统软件复位
	}
}
