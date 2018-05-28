#include "STM32F4xx.h"
#include "Matrix.h"
#include "Map.h"
#include <math.h>

/* Private  macro -------------------------------------------------------------*/
#define Pai				3.14159265358979323846

#define WARD0 			0		//��ʼ����������������X������ļн�
#define MO_TO_COAngle 	0		//��ȫ�ֶ�λָ��������ĵ��������������X��������ļнǣ�MapOrgin to CenterOrigin Angle��
#define R				0		//��ȫ�ֶ�λ���������ĵľ��룬��mmΪ��λ

/**	˵����Ϊ������ƹ����г������䣬�ֽ��Ƕȱ�Ϊ����ֵ����+2k��	**/

/* Public  variables ---------------------------------------------------------*/
Coord Origin;					//����ԭ������
Coord OriginalCoord;			//��ȫ�ֶ�λ����������һ��ת����ԭʼ����ֵ
Coord CoordBase_O = {0, 0, 0};	//����ȫ�ֶ�λ�������׼(��ȡ׼ȷ����Ļ���) -- Original
Coord CoordBase_R = {0, 0, 0};	//����ʵ������������׼(��ȡ׼ȷ����Ļ���) -- Real

Matrix CorrectMat;				//�������任�Ĺ��ɾ���
int16_t AngleOffset;			//�Ƕ�У׼ֵ��׼ȷ - ʵ�ʣ�

Coord OtherCoordinate;			//����������Դ��ȡ�ľ�������
motion_status motion; 			//����ȫ�ֶ�λ��������ֵ�ı���

int16_t Angle_cnt;				//�ۼ�Ȧ��������
Coord Coordinate;				//ʵ������ָ�������˶��ľ�������

/**
* @brief  �趨�����׼��ʹ�������ֶ�У׼ȫ�ֶ�λʱʹ�ã�
* @param  RealCoord:��ǰ��ȫ�ֶ�λ����ʹ�ø�ʽ�Ļش�ֵ
* @retval None
*/
static void setCoordinateBase(Coord RealCoord)
{
	CoordBase_O = OriginalCoord;
	CoordBase_R = RealCoord;
}

/**
* @brief  ������������Ĺ��ɾ���
* @param  theta:�Ƕ�ƫ������ƣ�׼ȷֵ��ȥȫ�ֶ�λ��ʵ��ֵ
* @introduction
*			��������˳�򣺴��������Ǳ���������Ļ���Ȼ���Ǳ���������Ļ�
*			��ÿ�������������˳�򣺴��ϵ������ǻ��������ռ��еĺ����꣬Ȼ����������
* @retval None
*/
static void CreatCorrectMat(double theta)
{
	Matrix MatAlpha;				//ԭ����ϵ�����������ɵ�����������
	Matrix MatBeta;					//������ϵ�����������ɵ�����������
	
	Matrix::CreatedArreyType aAlpha = Matrix::CreatArray(2, 2);			//����һ������
	Matrix::CreatedArreyType aBeta = Matrix::CreatArray(2, 2);			//����һ������
	
	aAlpha.ArrayName = ArrayName(aAlpha);
	aBeta.ArrayName = ArrayName(aBeta);
	
	aAlpha.Pointer[0][1] = 1;				//ԭ����ϵ�µı�׼������
	aAlpha.Pointer[1][0] = 1;
	
	MatAlpha.fillMatrix(aAlpha);
	Matrix::DeleteArray(aAlpha);										//ɾ��һ������
	
	aBeta.Pointer[0][0] = 1 * cos(theta);	//������ϵ�µı�׼������
	aBeta.Pointer[1][0] = 1 * sin(theta);
	
	aBeta.Pointer[0][1] = 1 * sin(theta + Pai/2.0);
	aBeta.Pointer[1][1] = 1 * cos(theta + Pai/2.0);
	
	MatBeta.fillMatrix(aBeta);
	Matrix::DeleteArray(aBeta);											//ɾ��һ������
	
	CorrectMat = MatBeta.getInverseMatrix() * MatAlpha;		//��ù��ɾ���
}

/**
* @brief  �趨��������Ϊԭ�㣨��ȫ�ֶ�λ�ڵ��̷�����λ�ð�װʱʹ�ã�
* @param  ��
* @retval ��
*/
static void SetOrigin(void)
{
	Angle_cnt = 0;
	Origin.Angle = 0;
	Origin.point.X = R * cos((double)WARD0);
	Origin.point.Y = R * sin((double)WARD0);
}

/**
* @brief  ��ȡ��ǰ����ȫ�ֶ�λ��ԭʼ���꣨����ԭʼ���ݵĸ�ʽ��
* @introduction ȫ�ֶ�λ���ص��ǣ�-180��180����ֵ������ʱ��Ϊ��̬�����ӵķ���
* @param  motion:����������
* @param  OriginData����ǰ��ȫ�ֶ�λ����ʹ�ø�ʽ�Ļش�ֵ
* @retval ��
*/
static void GetOriginalCoordinate(const motion_status motion, Coord &OriginData)
{
	double Temp_Plus = (Coordinate.Angle - (motion.zangle + Angle_cnt * 360));
	double Temp_Minus = -Temp_Plus;
	
	if(Temp_Plus > 340 && Temp_Plus < 380)		//�������Ƕȴ�180������-180����˵��������������������ת����һȦ
	{
		Angle_cnt++;
	}
	else if(Temp_Minus > 340 && Temp_Minus < 380)	//�������Ƕȴ�-180������180����˵��������������������ת����һȦ
	{
		Angle_cnt--;
	}
	else if(fabs(Temp_Plus) > 381)		//��Ч����,������˴δ���
	{
		return;
	}
	
	OriginData.Angle = motion.zangle + (double)(Angle_cnt * 360);	//�ѽǶȴ�[-180,180�����[0,360��,�ټ�������Ȧ
	OriginData.point.X = motion.pos_x + R * sin(OriginData.Angle);
	OriginData.point.Y = motion.pos_y + R * (1.0 - cos(OriginData.Angle));
	
	if(fabs(OriginData.Angle) > 32535.0)							//�����������ݵĴ��ݷ�Χ��16λ���ͣ�
	{
		OriginData.Angle -= Angle_cnt * 360;
		Angle_cnt = 0;
	}
}

/**
* @brief  ��ȡ��ǰ����ȫ�ֶ�λ�����׼����������
* @param  Current_O:��ǰ��ȫ�ֶ�λ����ʹ�ø�ʽ�Ļش�ֵ
* @param  Base_O:�ϴ�У׼ʱ�����Ļ���ȫ�ֶ�λ�������׼
* @retval None
*/
static Point getCurrentCoordIncrement(Coord Current_O, Coord Base_O)
{
	Point Increment;
	
	Increment.X = Current_O.point.X - Base_O.point.X;
	Increment.Y = Current_O.point.Y - Base_O.point.Y;
	
	return Increment;
}

/**
* @brief  Ť��ȫ�ֶ�λ����ֵ
* @param  Increment����ǰ�������ȫ�ֶ�λ��׼���������
* @retval None
*/
static Point CorrectCoordinateIncrement(Point Increment)
{
	Matrix CorrectedIncrement;
	Point Result;
	Matrix::CreatedArreyType aPoint = Matrix::CreatArray(2, 1);		//����һ������һ�е�����
	
	aPoint.Pointer[0][0] = Increment.X;
	aPoint.Pointer[1][0] = Increment.Y;
	
	CorrectedIncrement.fillMatrix(aPoint);
	Matrix::DeleteArray(aPoint);									//�ͷ��ڴ�ռ��Խ�Լ��Դ
	
	CorrectedIncrement = CorrectedIncrement * CorrectMat;
	
	Result.X = CorrectedIncrement.getElementValue(0, 0);
	Result.Y = CorrectedIncrement.getElementValue(1, 0);
	
	return Result;
}

/**
* @brief  ȫ�ֶ�λ������ʼ��
* @param  None
* @retval ��
*/
void MapParamsInit(void)
{
	CorrectMat = CorrectMat.getE(2);
	AngleOffset = 0;
	
	SetOrigin();
}

/**
* @brief  У�����꣨���׼ȷ��ʵ������ֵ֮����ã�
* @param  None
* @retval None
*/
void CorrectCoordinate(Coord RealCoordinate)
{
	setCoordinateBase(RealCoordinate);
	
	AngleOffset = RealCoordinate.Angle - Coordinate.Angle;
	
	CreatCorrectMat(AngleOffset);
}

/**
* @brief  ����ʵ������ֵ
* @param  None
* @retval None
*/
void UpdateRealCoordinate(void)
{
	Point IncreamentPoint;
	
	GetOriginalCoordinate(motion, OriginalCoord);	//��ȡԭʼ����
	
	IncreamentPoint = CorrectCoordinateIncrement(getCurrentCoordIncrement(OriginalCoord, CoordBase_O));
	
	Coordinate.point.X = IncreamentPoint.X;
	Coordinate.point.Y = IncreamentPoint.Y;
	
	Coordinate.Angle = OriginalCoord.Angle + AngleOffset;
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
