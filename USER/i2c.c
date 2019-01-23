#include "i2c.h"

void I2C_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB,&GPIO_InitStructure);
}  

//void I2C_OUT(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
//	GPIO_Init(GPIOB,&GPIO_InitStructure);

////	I2C_SCL_H();
////	I2C_SDA_H();   //把两条线都变成高电平
//}

//void I2C_IN(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //输入上拉模式
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
//}

//在SCL高电平，SDA由高到低，在此之前，SDA的高电平必须保持>4.7us，起始信号变成低电平之后，还要延时>4us
void I2C_Start(void) //I2C开始信号
{
	SDA_OUT();     //sda线输出 
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void I2C_Stop(void) //I2C停止信号
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);	
}

void I2C_ACK(void)  //主机产生应答信号
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void I2C_NACK(void)  //主机不产生应答信号
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

u8 I2C_Wait_ACK(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

void Send_Byte(u8 txd)  //发送一个字节的数据
{
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC_SDA = (txd & 0x80) >> 7;
			txd<<=1; 	  
			delay_us(2);   
			IIC_SCL=1;
			delay_us(2); 
			IIC_SCL=0;	
			delay_us(2);
    }	 
}

u8 Receive_Byte(u8 ack) //接收一个字节的数据
{
	u8 i,receive=0;
	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
     IIC_SCL=0; 
     delay_us(2);
		 IIC_SCL=1;
     receive <<= 1;
     if(READ_SDA)
			 receive++;   
		 delay_us(1); 
  }					 
  if(ack)
     I2C_ACK(); //发送ACK 
  else
     I2C_NACK();//发送nACK  
  return receive;
}
