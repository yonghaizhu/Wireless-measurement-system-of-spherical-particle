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
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽ
//	GPIO_Init(GPIOB,&GPIO_InitStructure);

////	I2C_SCL_H();
////	I2C_SDA_H();   //�������߶���ɸߵ�ƽ
//}

//void I2C_IN(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������ģʽ
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
//}

//��SCL�ߵ�ƽ��SDA�ɸߵ��ͣ��ڴ�֮ǰ��SDA�ĸߵ�ƽ���뱣��>4.7us����ʼ�źű�ɵ͵�ƽ֮�󣬻�Ҫ��ʱ>4us
void I2C_Start(void) //I2C��ʼ�ź�
{
	SDA_OUT();     //sda����� 
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

void I2C_Stop(void) //I2Cֹͣ�ź�
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);	
}

void I2C_ACK(void)  //��������Ӧ���ź�
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void I2C_NACK(void)  //����������Ӧ���ź�
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
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}

void Send_Byte(u8 txd)  //����һ���ֽڵ�����
{
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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

u8 Receive_Byte(u8 ack) //����һ���ֽڵ�����
{
	u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
     I2C_ACK(); //����ACK 
  else
     I2C_NACK();//����nACK  
  return receive;
}
