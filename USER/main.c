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
	WWDG_Init();      //��ʼ�����ڿ��Ź�
	LED_GPIO_Init();	//��ʼ��LED
	Wifi_GPIO_Init();	//��ʼ��WiFiģ��
	MPU9250_Init();   //��ʼ��MPU9250

	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	
	SPI_FLASH_Init(); //��ʼ��W25Q128
	TIME_Init();     //��ʼ����ʱ��

	USART3_Init();    //��ʼ������3
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
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //�����жϱ�־λ
	  data = (u8)USART_ReceiveData(USART3);
		USART3_Cmd(&data, &tim2send_flag);
	}

}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   //�����жϱ�־λ

		led_time++;
		ack_time++;
		
		if(tim2send_flag == 2)
		{
			Save_MPU9250_6rdData();    //WiFi����������,ֻд��W25Q128
			data_cnt++;
			if(data_cnt == dataBufferSize)
			{
				data_cnt = 0;
				write_flag = 1;
//				printf("\r\n============================\r\n");  //�ָ���
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

