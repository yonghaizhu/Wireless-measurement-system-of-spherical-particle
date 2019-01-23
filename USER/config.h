#ifndef _CONFIG_H
#define _CONFIG_H

/*==============================================================
*    ����       |           ϵͳ�����ļ�
*    ����       |           Zhu Yonghai
*    �޸�ʱ��   |           2018/12/03
================================================================*/

#include "stm32f10x.h"

#define device_ID 0x06
//�豸ID
#define dataHeader "A06"                 //����ͷ֡
#define firmware_ID "1.6"
#define update_Time "2018/12"

#define sFLASH_ID 0xEF4018               //W25Q128оƬID
#define dataBufferSize 21
#define LED_TimeControl 200             //LED��˸ʱ�����   

#define LED_Port GPIOB
#define LED_Pin GPIO_Pin_12

#define GYRO_X_Offset 0
#define GYRO_Y_Offset 0
#define GYRO_Z_Offset 0

#define ACCEL_X_Offset 0
#define ACCEL_Y_Offset 0
#define ACCEL_Z_Offset 0

#define W25Qxx_Max_Page 65536


/*----------------����WiFi��������------------*/
#define CWMODE_DEF    "AT+CWMODE_DEF=1\r\n"    //�����ڵ�stationģʽ�����ò���������flash
#define CWJAP_DEF     "AT+CWJAP_DEF=\"Lab842\",\"842842842\"\r\n"  //����·���������浽flash
//#define CIPSTART      "AT+CIPSTART=\"TCP\",\"192.168.1.125\",8090\r\n"  //����TCP������,�����浽flash
#define SAVETRANSLINK "AT+SAVETRANSLINK=1,\"192.168.1.125\",8090,\"TCP\"\r\n"  //����TCP������,���浽flash
#define CIPMODE       "AT+CIPMODE=1\r\n"
#define CIPSEND       "AT+CIPSEND\r\n"     //��������
#define EXIT_CIPSEND  "+++"      //�˳�͸��ģʽ
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

/*----------------ͨ��Э��--------------------*/
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

