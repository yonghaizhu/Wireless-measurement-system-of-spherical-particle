#include "datapro.h"
#include "mpu9250.h"
#include "spi_flash.h"
#include "delay.h"
#include "usart3.h"
#include "basemath.h"
#include "LED.h"
#include "config.h"
#include "usart1.h"
//#define datasize 9

#define PerWritePageSize 256

static u32 write_addr = 0x000000;
static u32 read_addr = 0x000000;
static u16 page_cnt_write = 0;
static u16 page_cnt_read = 0;

void Write_W25Q128(void)      //向flash芯片(W25Q128)写入252字节补码数据，共14组数据，每组18字节
{
	extern u8 caches[PerWritePageSize];   //定义在"mpu9250.c", 存储MPU9250的补码数据

	write_addr = PerWritePageSize * page_cnt_write;
	SPI_FLASH_PageWrite(caches, write_addr, PerWritePageSize);
	page_cnt_write++;
	if(page_cnt_write == W25Qxx_Max_Page)
	{
		TIM_Cmd(TIM2, DISABLE);
		LED_ON();
		printf(" %s W25Q128存储空间已满！！！\r\n", dataHeader);
	}
}


u8 Read_Flash_Buffer(u8* FlashReadBuffer)  //读取flash芯片指定地址起的256字节补码数据
{	
	u16 check_data = 0;
	read_addr = PerWritePageSize * page_cnt_read;
	SPI_FLASH_BufferRead(FlashReadBuffer, read_addr, PerWritePageSize);
	check_data = FlashReadBuffer[255] + FlashReadBuffer[254] + FlashReadBuffer[253] + FlashReadBuffer[252];
//	printf("read_addr = %d\r\n", read_addr);
//	printf("check_data = %d\r\n", check_data);
	if(check_data == 0)
	{
		page_cnt_read++;
		return OK;		
	}
	else
	{
	  return ERROR;
	}
}

int Com_Tran_Trf(u16 comdata)    //补码变原码
{
	if(comdata & 0x8000)
		return -(65536 - comdata);
	
	else
		return comdata;
}

void Get_Com_Data(u8 *pBuff, u16 *comBuff)  //获取补码数据
{
	u8 i = 0;
	
	for(i = 0; i < 126; i++)
	{
		*comBuff = (*pBuff << 8) | *(pBuff + 1);
		pBuff += 2;
		comBuff++;
	}
}

void Get_Trf_Data(u16 *comBuff, int *trfBuff) //获取原码数据
{
	u8 i = 0;
	for(i = 0; i < 126 ;i++)
	{
		*trfBuff = Com_Tran_Trf(*comBuff);
		comBuff++;
		trfBuff++;
	}
}

u8 ReadLast4BytesOfPage_Erase(u32 addr) //判断每个扇区第一页最后四字节之和是否为0，为0则擦除该扇区
{
	u8 temp[4] = {0};
	u8 count = 0;
	SPI_FLASH_BufferRead(temp, addr + 252, 4);
	count = temp[0] + temp[1] + temp[2] + temp[3];
	if(count == 0)
	{
		return OK;
	}
	else 
		return ERROR;
}

void Send_Flash_WiFi(u8* FlashReadBuffer)   //向串口发送原码数据
{
	u16 comBuff[126] = {0};
	int trfBuff[126] = {0};
	u8 i = 0;
	extern _sensor_data gyro_offset;
	extern _sensor_data accel_offset;
	
	Get_Com_Data(FlashReadBuffer, comBuff);
	Get_Trf_Data(comBuff, trfBuff);
	
	for(i = 0; i < 21; i++)
	{
		trfBuff[i * 6 + 0] = trfBuff[i * 6 + 0] - accel_offset.x;
		trfBuff[i * 6 + 1] = trfBuff[i * 6 + 1] - accel_offset.y;
		trfBuff[i * 6 + 2] = trfBuff[i * 6 + 2] - accel_offset.z;
		trfBuff[i * 6 + 3] = trfBuff[i * 6 + 3] - gyro_offset.x;
		trfBuff[i * 6 + 4] = trfBuff[i * 6 + 4] - gyro_offset.y;
		trfBuff[i * 6 + 5] = trfBuff[i * 6 + 5] - gyro_offset.z;
	}
	
	for(i = 0; i < 21; i++)
	{
		printf("%s %d %d %d %d %d %d\r\n",
		dataHeader,
		trfBuff[i * 6], trfBuff[i * 6 + 1], trfBuff[i * 6 + 2], 
		trfBuff[i * 6 + 3], trfBuff[i * 6 + 4], trfBuff[i * 6 + 5]);
	}
	printf("\r\n============================\r\n");
}

void Send_Flash_USART1(u8* FlashReadBuffer)   //向串口发送原码数据
{
	u16 comBuff[126] = {0};
	int trfBuff[126] = {0};
	u8 i = 0;
	u8 j = 0;
	extern _sensor_data gyro_offset;
	extern _sensor_data accel_offset;
	
	Get_Com_Data(FlashReadBuffer, comBuff);
	Get_Trf_Data(comBuff, trfBuff);
	
	for(i = 0; i < 21; i++)
	{
		trfBuff[i * 6 + 0] = trfBuff[i * 6 + 0] - accel_offset.x;
		trfBuff[i * 6 + 1] = trfBuff[i * 6 + 1] - accel_offset.y;
		trfBuff[i * 6 + 2] = trfBuff[i * 6 + 2] - accel_offset.z;
		trfBuff[i * 6 + 3] = trfBuff[i * 6 + 3] - gyro_offset.x;
		trfBuff[i * 6 + 4] = trfBuff[i * 6 + 4] - gyro_offset.y;
		trfBuff[i * 6 + 5] = trfBuff[i * 6 + 5] - gyro_offset.z;
	}
	
	for(i = 0; i < 21; i++)
	{
		USART_printf(USART1,dataHeader);
		USART_printf(USART1," ");
		for(j = 0; j < 6; j++)
		{
			USART_printf(USART1, "%d ", trfBuff[i * 6 + j]);
		}
		USART_printf(USART1,"\r\n");
	}
	USART_printf(USART1,"\r\n============================\r\n");
//	delay_ms(10);
}


u8 Read_W25Q128(void)
{
	u8 FlashReadBuffer[PerWritePageSize] = {0};
	if(Read_Flash_Buffer(FlashReadBuffer))
	{
		Send_Flash_WiFi(FlashReadBuffer);
		return OK;
	}
	
	else
		return ERROR;
}

u8 Read_W25Q128_USART1(void)
{
	u8 FlashReadBuffer[PerWritePageSize] = {0};
	if(Read_Flash_Buffer(FlashReadBuffer))
	{
		Send_Flash_USART1(FlashReadBuffer);
		return OK;
	}
	
	else
		return ERROR;
}

void Erase_W25Q128_4K(void)
{
	u32 temp = 0;
	u32 addr = 0x000000;
	printf("\r\n %s :开始擦除W25Q128！ \r\n", dataHeader);
	LED_OFF();
	USART_Cmd(USART3, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	while(ReadLast4BytesOfPage_Erase(addr))
	{
		SPI_FLASH_SectorErase(addr);
		temp += 16;
		addr = PerWritePageSize * temp;
	}
	write_addr = 0x000000;
	read_addr = 0x000000;
	page_cnt_write = 0;
	page_cnt_read = 0;
	LED_ON();
	USART_Cmd(USART3, ENABLE);
	printf("\r\n %s :W25Q128擦除结束！ \r\n", dataHeader);
}

void Erase_W25Q128_Bulk(void)
{
	printf("\r\n %s :开始整片擦除W25Q128！ \r\n", dataHeader);
	LED_OFF();
	USART_Cmd(USART3, DISABLE);
	SPI_FLASH_BulkErase();
	write_addr = 0x000000;
	read_addr = 0x000000;
	page_cnt_write = 0;
	page_cnt_read = 0;
	LED_ON();
	USART_Cmd(USART3, ENABLE);
	printf("\r\n %s :W25Q128整片擦除结束！ \r\n", dataHeader);
}
