#include "stm32f10x.h"
#include "usart3.h"
#include "usart1.h"
#include "LED.h"
#include "delay.h"
#include "mpu9250.h"
#include "ESP8266.h"
#include "time.h"
#include "spi_flash.h"
#include "datapro.h"
#include "watchdog.h"
#include "config.h"


//u32 FlashID = 0;
u16 ack_time = 0;
u8 tim2send_flag = 0;
u8 led_time = 0;
u8 write_flag = 0;
u8 data_cnt = 0;


int main(void)
{	
	delay_init(72);
	WWDG_Init();      //初始化窗口看门狗
	LED_GPIO_Init();	//初始化LED
	Wifi_GPIO_Init();	//初始化WiFi模块
	MPU9250_Init();   //初始化MPU9250

	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	
	SPI_FLASH_Init(); //初始化W25Q128
	TIME_Init();     //初始化定时器

	USART3_Init();    //初始化串口3
	USART1_Init();
	ESP8266_Init();
	LED_ON();

	while(1)
	{
		 sys_cmd(&write_flag, &tim2send_flag);
	}	
}


void USART3_IRQHandler(void)
{
	u8 data = 0;
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);  
		USART_ReceiveData(USART3);	
	}
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //更新中断标志位
	  data = (u8)USART_ReceiveData(USART3);
		USART3_Cmd(&data, &tim2send_flag);
	}

}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   //更新中断标志位

		led_time++;
		ack_time++;
		
		if(tim2send_flag == 2)
		{
			Save_MPU9250_6rdData();    //WiFi不发送数据,只写入W25Q128
			data_cnt++;
			if(data_cnt == dataBufferSize)
			{
				data_cnt = 0;
				write_flag = 1;
//				printf("\r\n============================\r\n");  //分隔符
			}
		}
		LED_OFF();
		if(led_time == LED_TimeControl)
		{
			led_time = 0;
			LED_ON();
		}
		
		if(ack_time == 1000)
		{
			ack_time = 0;
			printf("%s:OK\r\n", dataHeader);
		}
	}
}

