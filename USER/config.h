#ifndef _CONFIG_H
#define _CONFIG_H

/*==============================================================
*    功能       |           系统配置文件
*    作者       |           Zhu Yonghai
*    修改时间   |           2018/12/03
================================================================*/

#include "stm32f10x.h"

#define device_ID 0x06
//设备ID
#define dataHeader "A06"                 //数据头帧
#define firmware_ID "1.6"
#define update_Time "2018/12"

#define sFLASH_ID 0xEF4018               //W25Q128芯片ID
#define dataBufferSize 21
#define LED_TimeControl 200             //LED闪烁时间控制   

#define LED_Port GPIOB
#define LED_Pin GPIO_Pin_12

#define GYRO_X_Offset 0
#define GYRO_Y_Offset 0
#define GYRO_Z_Offset 0

#define ACCEL_X_Offset 0
#define ACCEL_Y_Offset 0
#define ACCEL_Z_Offset 0

#define W25Qxx_Max_Page 65536


/*----------------串口WiFi配置命令------------*/
#define CWMODE_DEF    "AT+CWMODE_DEF=1\r\n"    //工作在单station模式，设置参数保存在flash
#define CWJAP_DEF     "AT+CWJAP_DEF=\"Lab842\",\"842842842\"\r\n"  //连接路由器，保存到flash
//#define CIPSTART      "AT+CIPSTART=\"TCP\",\"192.168.1.125\",8090\r\n"  //连接TCP服务器,不保存到flash
#define SAVETRANSLINK "AT+SAVETRANSLINK=1,\"192.168.1.125\",8090,\"TCP\"\r\n"  //连接TCP服务器,保存到flash
#define CIPMODE       "AT+CIPMODE=1\r\n"
#define CIPSEND       "AT+CIPSEND\r\n"     //发送数据
#define EXIT_CIPSEND  "+++"      //退出透传模式
#define AT_OK         "AT\r\n"
/*--------------------------------------------*/

/*----------------------------------------------
AT
AT+CWMODE_DEF=1
AT+CWJAP_DEF="YH_Lab","842842842"
AT+SAVETRANSLINK=1,"192.168.1.112",8090,"TCP"
AT+CIPMODE=1
AT+RST
------------------------------------------------*/

/*----------------通信协议--------------------*/
#define START         's'
#define STOP				  'p'
#define STARTANDWRITE 'w'
#define READ          'r'
#define ERASE         'e'
#define ADJUST        'a'
#define SOFTRESET     '0'
#define SENDBYUSART1  'u'
/*-------------------------------------------*/

#endif

