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
}_sensor_data;				  //传感器原始数据类型，使用int

typedef struct 
{
	float x;
	float y;
	float z;
} _gesture_data;		   	//传感器数据处理后的数据类型，使用float

typedef struct
{
	float Yaw;          //航向角
	float Pitch;        //俯仰角
  float Roll;         //横滚角
} _eulerAngle;        //欧拉角数据类型

//四元数数据类型
typedef struct
{
	float Q0;
	float Q1;
	float Q2;
	float Q3;	 
} _quaternion;


#endif
