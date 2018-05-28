#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include "PlaneAnalyticGeometry.h"


//传感器数据接收结构体（信息量）
typedef struct
{
		float pos_x;
		float pos_y;
		float zangle;
		float xangle;
		float yangle;
		float w_z;
	
}motion_status;

//全局定位相关的坐标（运算量）
typedef struct Coordinate{
	
	Point point;
	double Angle;
	
}Coord;

extern motion_status motion;

extern Coord OtherCoordinate;;			//从其他数据源获取的绝对坐标

extern Coord Coordinate;				//用来指导运动的绝对坐标
extern int16_t Angle_cnt;


void MapParamsInit(void);
void CorrectCoordinate(Coord RealCoordinate);
void UpdateRealCoordinate(void);

#endif

