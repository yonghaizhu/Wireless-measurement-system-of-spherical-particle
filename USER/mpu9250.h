#ifndef MPU9250_H_
#define MPU9250_H_

#include "stm32f10x.h"

/* MPU9250 Register Address ------------------------------------------------------------*/
#define	SMPLRT_DIV		0x19	//陀螺仪采样率
#define	CONFIG				0x1A	//低通滤波频率
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
														//(0x00 +-2g;)  ( 0x08 +-4g;)  (0x10 +-8g;)  (0x18 +-16g)	
#define ACCEL_CONFIG2 0X1D														
#define MAG_CONFIG    0x0A

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据

#define	GYRO_ADDRESS   0xD0	  //陀螺地址
#define MAG_ADDRESS    0x18   //磁场地址
#define ACCEL_ADDRESS  0xD0   //加速度计地址

/* ---- AK8963 Reg In MPU9250 ----------------------------------------------- */
#define AK8963_I2C_ADDR             ((u8)0x0C)
#define AK8963_DEV_ID               ((u8)0x48) 
//Read-only Reg
#define AK8963_WIA                  ((u8)0x00)  
#define AK8963_INFO                 ((u8)0x01)
#define AK8963_ST1                  ((u8)0x02)
#define AK8963_HXL                  ((u8)0x03)
#define AK8963_HXH                  ((u8)0x04)
#define AK8963_HYL                  ((u8)0x05)
#define AK8963_HYH                  ((u8)0x06)
#define AK8963_HZL                  ((u8)0x07)
#define AK8963_HZH                  ((u8)0x08)
#define AK8963_ST2                  ((u8)0x09)
//Write/Read Reg
#define AK8963_CNTL1                ((u8)0x0A)
#define AK8963_CNTL2                ((u8)0x0B)
#define AK8963_ASTC                 ((u8)0x0C)
#define AK8963_TS1                  ((u8)0x0D)
#define AK8963_TS2                  ((u8)0x0E)
#define AK8963_I2CDIS               ((u8)0x0F)
//Read-only Reg ( ROM )
#define AK8963_ASAX                 ((u8)0x10)
#define AK8963_ASAY                 ((u8)0x11)
#define AK8963_ASAZ                 ((u8)0x12)


typedef enum {
    MPU_GYRO_LPS_250HZ   = 0x00,
    MPU_GYRO_LPS_184HZ   = 0x01,
    MPU_GYRO_LPS_92HZ    = 0x02,
    MPU_GYRO_LPS_41HZ    = 0x03,
    MPU_GYRO_LPS_20HZ    = 0x04,
    MPU_GYRO_LPS_10HZ    = 0x05,
    MPU_GYRO_LPS_5HZ     = 0x06,
    MPU_GYRO_LPS_DISABLE = 0x07,
} MPU_GYRO_LPF_TypeDef;

typedef enum {
    MPU_ACCE_LPS_460HZ   = 0x00,
    MPU_ACCE_LPS_184HZ   = 0x01,
    MPU_ACCE_LPS_92HZ    = 0x02,
    MPU_ACCE_LPS_41HZ    = 0x03,
    MPU_ACCE_LPS_20HZ    = 0x04,
    MPU_ACCE_LPS_10HZ    = 0x05,
    MPU_ACCE_LPS_5HZ     = 0x06,
    MPU_ACCE_LPS_DISABLE = 0x08,
} MPU_ACCE_LPF_TypeDef;

typedef enum {
    MPU_GYRO_FS_250  = 0x00,
    MPU_GYRO_FS_500  = 0x08,
    MPU_GYRO_FS_1000 = 0x10,
    MPU_GYRO_FS_2000 = 0x18,
} MPU_GYRO_FS_TypeDef;

typedef enum {
    MPU_ACCE_FS_2G  = 0x00,
    MPU_ACCE_FS_4G  = 0x08,
    MPU_ACCE_FS_8G  = 0x10,
    MPU_ACCE_FS_16G = 0x18,
} MPU_ACCE_FS_TypeDef;

typedef enum {
    MPU_READ_ACCE = 1 << 0,
    MPU_READ_TEMP = 1 << 1,
    MPU_READ_GYRO = 1 << 2,
    MPU_READ_MAGN = 1 << 3,
    MPU_READ_ALL  = 0x0F,
} MPU_READ_TypeDef;

typedef enum {
    MPU_CORRECTION_PX = 0x01,
    MPU_CORRECTION_NX = 0x02,
      
    MPU_CORRECTION_PY = 0x03,
    MPU_CORRECTION_NY = 0x04,
    MPU_CORRECTION_PZ = 0x05,
    MPU_CORRECTION_NZ = 0x06,
    MPU_CORRECTION_GYRO = 0x07,
    MPU_CORRECTION_CALCX = 0x08,
    MPU_CORRECTION_CALCY = 0x09,
    MPU_CORRECTION_CALCZ = 0x0A,
    MPU_CORRECTION_SAVE = 0x0B,
    MPU_CORRECTION_CIRCLE = 0x0C,
    MPU_CORRECTION_CIRCLEZ = 0x0D,
} MPU_CORRECTION_TypeDef;

void MPU9250_Init(void);
void Send_MPU9250_6rdData(void);
void Save_MPU9250_6rdData(void);
void MPU9250_Adjust_GYRO(void);
void MPU9250_Buffer_Reset(void);


#endif
