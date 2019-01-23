#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"
#include "delay.h"

#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_7
#define GPIO_I2C GPIOB
//#define I2C_SCL_H() GPIO_SetBits(GPIO_I2C, I2C_SCL)   //拉高时钟引脚
//#define I2C_SCL_L() GPIO_ResetBits(GPIO_I2C, I2C_SCL) //拉低时钟引脚
//#define I2C_SDA_H() GPIO_SetBits(GPIO_I2C, I2C_SDA)   //拉高数据引脚
//#define I2C_SDA_L() GPIO_ResetBits(GPIO_I2C, I2C_SDA) //拉低数据引脚

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 


#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

void I2C_GPIO_Init(void);
//void I2C_OUT(void);
//void I2C_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_ACK(void);
void I2C_NACK(void);
u8 I2C_Wait_ACK(void);
void Send_Byte(u8 data);  //发送一个字节的数据
u8 Receive_Byte(u8 ack); //接收一个字节的数据

#endif
