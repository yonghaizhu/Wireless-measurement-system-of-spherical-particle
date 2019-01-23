#ifndef _BASEMATH_H
#define _BASEMATH_H
#include <math.h>
#include <stdint.h>

#define PI 							3.1415926f
typedef struct 
{
	int x;
	int y;
	int z;
}_sensor_data;				  //������ԭʼ�������ͣ�ʹ��int

typedef struct 
{
	float x;
	float y;
	float z;
} _gesture_data;		   	//���������ݴ������������ͣ�ʹ��float

typedef struct
{
	float Yaw;          //�����
	float Pitch;        //������
  float Roll;         //�����
} _eulerAngle;        //ŷ������������

//��Ԫ����������
typedef struct
{
	float Q0;
	float Q1;
	float Q2;
	float Q3;	 
} _quaternion;


#endif
