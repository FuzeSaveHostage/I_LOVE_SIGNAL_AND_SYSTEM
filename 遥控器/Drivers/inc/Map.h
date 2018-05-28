#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include "PlaneAnalyticGeometry.h"


//���������ݽ��սṹ�壨��Ϣ����
typedef struct
{
		float pos_x;
		float pos_y;
		float zangle;
		float xangle;
		float yangle;
		float w_z;
	
}motion_status;

//ȫ�ֶ�λ��ص����꣨��������
typedef struct Coordinate{
	
	Point point;
	double Angle;
	
}Coord;

extern motion_status motion;

extern Coord OtherCoordinate;;			//����������Դ��ȡ�ľ�������

extern Coord Coordinate;				//����ָ���˶��ľ�������
extern int16_t Angle_cnt;


void MapParamsInit(void);
void CorrectCoordinate(Coord RealCoordinate);
void UpdateRealCoordinate(void);

#endif

