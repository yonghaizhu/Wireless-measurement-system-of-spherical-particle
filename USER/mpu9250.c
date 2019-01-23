#include "mpu9250.h"
#include "i2c.h"
#include "delay.h"
#include "basemath.h"
#include "usart3.h"
#include "LED.h"
#include "config.h"

_sensor_data gyro_offset = {GYRO_X_Offset, GYRO_Y_Offset, GYRO_Z_Offset};
_sensor_data accel_offset = {ACCEL_X_Offset, ACCEL_Y_Offset, ACCEL_Z_Offset};
u8 BufferSize = 0;
u8 FlashWriteBuffer[12] = {0}; //每组数据占18字节
u8 caches[256] = {0};   //数据编码缓存 256字节

void I2C_WriteByte_MPU9250(u8 SlaveAddr, u8 RegAddr, u8 value) //向MPU9250写入一字节
{
	I2C_Start();
	Send_Byte(SlaveAddr);
	I2C_Wait_ACK();
	Send_Byte(RegAddr);
	I2C_Wait_ACK();
	Send_Byte(value);
	I2C_Wait_ACK();
	I2C_Stop();
}

u8 I2C_ReadByte_MPU9250(u8 SlaveAddr, u8 RegAddr) //向MPU9250读取一字节
{
	u8 RegData;
	
	I2C_Start();
	Send_Byte(SlaveAddr);
	I2C_Wait_ACK();
	Send_Byte(RegAddr);
	I2C_Wait_ACK();
	I2C_Start();
	Send_Byte(SlaveAddr + 1);
	I2C_Wait_ACK();
	RegData = Receive_Byte(0);
	I2C_Stop();
	
	return RegData;
}

u8 I2C_ReadByte_AK8963(u8 RegAddr, u8 ack)  //读取磁强计一个字节数据
{
	u8 RegData;
	
	I2C_Start();
	Send_Byte(MAG_ADDRESS);
	I2C_Wait_ACK();
	Send_Byte(RegAddr);
	I2C_Wait_ACK();
	I2C_Start();
	Send_Byte(MAG_ADDRESS + 1);
	I2C_Wait_ACK();
	RegData = Receive_Byte(ack);
	I2C_Stop();
	
	return RegData;
}

void MPU9250_Init(void)  //初始化MPU9250
{
	I2C_GPIO_Init();
	I2C_WriteByte_MPU9250(SlaveAddress, PWR_MGMT_1, 0x00);//解除休眠状态
	delay_ms(1000);
	I2C_WriteByte_MPU9250(SlaveAddress, SMPLRT_DIV, 0x01);  //陀螺仪采样频率 1000/(1 + 1) = 500 Hz
	I2C_WriteByte_MPU9250(SlaveAddress, CONFIG, 0x06);      
	I2C_WriteByte_MPU9250(SlaveAddress, GYRO_CONFIG, 0x18); // 2000
	I2C_WriteByte_MPU9250(SlaveAddress, ACCEL_CONFIG, 0x18);// +-16g
//	I2C_WriteByte_MPU9250(SlaveAddress, ACCEL_CONFIG2, 0x06);
	I2C_WriteByte_MPU9250(SlaveAddress, 0x6A, 0x00);
	I2C_WriteByte_MPU9250(SlaveAddress, 0x37, 0x02);
//	I2C_WriteByte_MPU9250(SlaveAddress, 0x68, 0x06);
//	delay_ms(10);
//	I2C_WriteByte_MPU9250(SlaveAddress, 0x7D, 0x00);
//	I2C_WriteByte_MPU9250(SlaveAddress, 0x7E, 0xFE);

//	delay_ms(50);
//	I2C_WriteByte_MPU9250(MAG_ADDRESS, 0x0A, 0x11);
}

u16 Get_ACCEL_Com(u8 RegAddr)  //获取加速度计数据补码
{
	u8 H, L;
	
	H = I2C_ReadByte_MPU9250(SlaveAddress, RegAddr);
	L = I2C_ReadByte_MPU9250(SlaveAddress, RegAddr + 1);
	
	if(BufferSize >= 12)
		BufferSize = 0;
	
	FlashWriteBuffer[BufferSize] = H;
	FlashWriteBuffer[BufferSize + 1] = L;
	BufferSize += 2;
	
	return (H << 8) + L;
}

u16 Get_GYRO_Com(u8 RegAddr)  //获取陀螺仪数据补码
{
	u8 H, L;
	H = I2C_ReadByte_MPU9250(SlaveAddress, RegAddr);
	L = I2C_ReadByte_MPU9250(SlaveAddress, RegAddr + 1);
	
	FlashWriteBuffer[BufferSize] = H;
	FlashWriteBuffer[BufferSize + 1] = L;
	BufferSize += 2;
	
	return (H << 8) + L;
}

//u16 Get_MAG_Com(u8 RegAddr, u8 ack)  //获取陀指南针数据补码
//{
//	u8 H, L;
//	H = I2C_ReadByte_AK8963(RegAddr + 1, ack);
//	L = I2C_ReadByte_AK8963(RegAddr, ack);
//	
//	FlashWriteBuffer[BufferSize] = H;
//	FlashWriteBuffer[BufferSize + 1] = L;	
//	BufferSize += 2;
//	
//	return (H << 8) + L;
//}

   
void Get_ACCEL_Trf(_sensor_data *acc)  //加速度计数据补码变原码 
{
	_sensor_data accel;
	accel.x = Get_ACCEL_Com(ACCEL_XOUT_H);
	if(accel.x&0x8000)
	{
		accel.x = -(65536 - accel.x);
	}
//	delay_ms(1);
	accel.y = Get_ACCEL_Com(ACCEL_YOUT_H);
	if(accel.y&0x8000)
	{
		accel.y = -(65536 - accel.y);
	}
	
	accel.z = Get_ACCEL_Com(ACCEL_ZOUT_H);
	if(accel.z&0x8000)
	{
		accel.z = -(65536 - accel.z);
	}
	
	acc->x = accel.x - accel_offset.x;
	acc->y = accel.y - accel_offset.y;
	acc->z = accel.z - accel_offset.z;
}

void Get_GYRO_Trf(_sensor_data *go)  //MPU9250陀螺仪补码变原码
{
	_sensor_data gyro;

	gyro.x = Get_GYRO_Com(GYRO_XOUT_H);
	if(gyro.x&0x8000)
	{
		gyro.x = -(65536 - gyro.x);
	}

	gyro.y = Get_GYRO_Com(GYRO_YOUT_H);
	if(gyro.y&0x8000)
	{
		gyro.y = -(65536 - gyro.y);
	}

	gyro.z = Get_GYRO_Com(GYRO_ZOUT_H);
	if(gyro.z&0x8000)
	{
		gyro.z = -(65536 - gyro.z);
	}
	
	go->x = gyro.x - gyro_offset.x;
	go->y = gyro.y - gyro_offset.y;
	go->z = gyro.z - gyro_offset.z;
}

void Get_GYRO_Offset(_sensor_data *go)  //获取陀螺仪漂移值
{
	int offset_x = 0, offset_y = 0, offset_z = 0;
	int i, num = 100;

	for(i = 0; i < num; i++)
	{
		_sensor_data gyro;
		gyro.x = Get_GYRO_Com(GYRO_XOUT_H);
		if(gyro.x&0x8000)
		{
			gyro.x = -(65536 - gyro.x);
		}
		offset_x += gyro.x;
//		printf("gyro.x = %d \r\n", gyro.x);
//		printf("offset_x = %d \r\n", offset_x);
		
		gyro.y = Get_GYRO_Com(GYRO_YOUT_H);
		if(gyro.y&0x8000)
		{
			gyro.y = -(65536 - gyro.y);
		}
		
		offset_y += gyro.y;
		
		gyro.z = Get_GYRO_Com(GYRO_ZOUT_H);
		if(gyro.z&0x8000)
		{
			gyro.z = -(65536 - gyro.z);
		}
		offset_z += gyro.z;
		delay_ms(10);
	}
	
	go -> x = offset_x / num;
	go -> y = offset_y / num;
	go -> z = offset_z / num;
}

void MPU9250_Adjust_GYRO(void)
{
	LED_OFF();
	Get_GYRO_Offset(&gyro_offset);
	printf("%s 陀螺仪的零漂为： %d %d %d\r\n", dataHeader, gyro_offset.x, gyro_offset.y, gyro_offset.z);
	LED_ON();
}

//void Get_MAG_Trf(_sensor_data *mag)   //获取磁力计原始数据
//{
//	_sensor_data magl;
//	_sensor_data mag_offset = {0,0,0};
//	I2C_WriteByte_MPU9250(SlaveAddress, 0x37, 0x02);
//	delay_ms(10);
//	I2C_WriteByte_MPU9250(MAG_ADDRESS, 0x0A, 0x11);
//	delay_ms(10);
//	magl.x = Get_MAG_Com(MAG_XOUT_L, 1);
//	if(magl.x&0x8000)
//	{
////		magl.x &= 0x0fff;
//		magl.x = -(65536 - magl.x);
//	}
//		
//	magl.y = Get_MAG_Com(MAG_YOUT_L, 1);
//	if(magl.y&0x8000)
//	{
////		magl.y &= 0x0fff;
//		magl.y = -(65536 - magl.y);
//	}
//		
//	magl.z = Get_MAG_Com(MAG_ZOUT_L, 0);
//	if(magl.z&0x8000)
//	{
////		magl.z &= 0x0fff;
//		magl.z = -(65536 - magl.z);
//	}

//	mag->x = magl.x - mag_offset.x;
//	mag->y = magl.y - mag_offset.y;
//	mag->z = magl.z - mag_offset.z;
//}

void Send_MPU9250_6rdData(void)
{
//	static _sensor_data accel, gyro, mag;
	_sensor_data accel, gyro;
	Get_ACCEL_Trf(&accel);  
	Get_GYRO_Trf(&gyro);

	printf("%s %d %d %d %d %d %d\r\n",
	dataHeader,
	accel.x, accel.y, accel.z, 
	gyro.x, gyro.y, gyro.z);
}

void Save_MPU9250_6rdData(void)
{
	_sensor_data accel, gyro;
	static u8 cnt = 0;    //计数器
	u8 j = 0;
	
	Get_ACCEL_Trf(&accel);  
	Get_GYRO_Trf(&gyro);
	for (j = 0; j < 12; j++)
	{
		caches[cnt + j] = FlashWriteBuffer[j];
			//printf("%d ", caches[cnt + j]);
	}
	cnt += 12;
	if(cnt == 252)
	{
		cnt = 0;
		caches[252] = 0;
		caches[253] = 0;
		caches[254] = 0;
		caches[255] = 0;
	}
}


void MPU9250_Buffer_Reset(void)
{
	BufferSize = 0;
}

