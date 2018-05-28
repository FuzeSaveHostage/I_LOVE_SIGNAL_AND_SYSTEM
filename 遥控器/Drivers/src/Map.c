#include "STM32F4xx.h"
#include "Matrix.h"
#include "Map.h"
#include <math.h>

/* Private  macro -------------------------------------------------------------*/
#define Pai				3.14159265358979323846

#define WARD0 			0		//初始底盘正方向与坐标X轴正向的夹角
#define MO_TO_COAngle 	0		//从全局定位指向底盘中心的向量与绝对坐标X轴正方向的夹角（MapOrgin to CenterOrigin Angle）
#define R				0		//从全局定位到底盘中心的距离，以mm为单位

/**	说明：为避免控制过程中出现跳变，现将角度变为连续值，θ+2kπ	**/

/* Public  variables ---------------------------------------------------------*/
Coord Origin;					//绝对原点坐标
Coord OriginalCoord;			//由全局定位回来经过第一次转化的原始坐标值
Coord CoordBase_O = {0, 0, 0};	//基于全局定位的坐标基准(获取准确坐标的基础) -- Original
Coord CoordBase_R = {0, 0, 0};	//基于实际坐标的坐标基准(获取准确坐标的基础) -- Real

Matrix CorrectMat;				//针对坐标变换的过渡矩阵
int16_t AngleOffset;			//角度校准值（准确 - 实际）

Coord OtherCoordinate;			//从其他数据源获取的绝对坐标
motion_status motion; 			//监测从全局定位传回来的值的变量

int16_t Angle_cnt;				//累加圈数计数器
Coord Coordinate;				//实际用来指导底盘运动的绝对坐标

/**
* @brief  设定坐标基准（使用其他手段校准全局定位时使用）
* @param  RealCoord:当前的全局定位符合使用格式的回传值
* @retval None
*/
static void setCoordinateBase(Coord RealCoord)
{
	CoordBase_O = OriginalCoord;
	CoordBase_R = RealCoord;
}

/**
* @brief  生成坐标矫正的过渡矩阵
* @param  theta:角度偏差，弧度制，准确值减去全局定位的实测值
* @introduction
*			·向量的顺序：从左到右先是表征横坐标的基，然后是表征纵坐标的基
*			·每个向量中坐标的顺序：从上到下先是基在向量空间中的横坐标，然后是纵坐标
* @retval None
*/
static void CreatCorrectMat(double theta)
{
	Matrix MatAlpha;				//原坐标系下列向量构成的正交基矩阵
	Matrix MatBeta;					//新坐标系下列向量构成的正交基矩阵
	
	Matrix::CreatedArreyType aAlpha = Matrix::CreatArray(2, 2);			//申请一个数表
	Matrix::CreatedArreyType aBeta = Matrix::CreatArray(2, 2);			//申请一个数表
	
	aAlpha.ArrayName = ArrayName(aAlpha);
	aBeta.ArrayName = ArrayName(aBeta);
	
	aAlpha.Pointer[0][1] = 1;				//原坐标系下的标准正交基
	aAlpha.Pointer[1][0] = 1;
	
	MatAlpha.fillMatrix(aAlpha);
	Matrix::DeleteArray(aAlpha);										//删除一个数表
	
	aBeta.Pointer[0][0] = 1 * cos(theta);	//新坐标系下的标准正交基
	aBeta.Pointer[1][0] = 1 * sin(theta);
	
	aBeta.Pointer[0][1] = 1 * sin(theta + Pai/2.0);
	aBeta.Pointer[1][1] = 1 * cos(theta + Pai/2.0);
	
	MatBeta.fillMatrix(aBeta);
	Matrix::DeleteArray(aBeta);											//删除一个数表
	
	CorrectMat = MatBeta.getInverseMatrix() * MatAlpha;		//获得过渡矩阵
}

/**
* @brief  设定底盘中心为原点（当全局定位在底盘非中心位置安装时使用）
* @param  无
* @retval 无
*/
static void SetOrigin(void)
{
	Angle_cnt = 0;
	Origin.Angle = 0;
	Origin.point.X = R * cos((double)WARD0);
	Origin.point.Y = R * sin((double)WARD0);
}

/**
* @brief  获取当前来自全局定位的原始坐标（修正原始数据的格式）
* @introduction 全局定位返回的是（-180，180）的值，以逆时针为姿态角增加的方向
* @param  motion:传感器参量
* @param  OriginData：当前的全局定位符合使用格式的回传值
* @retval 无
*/
static void GetOriginalCoordinate(const motion_status motion, Coord &OriginData)
{
	double Temp_Plus = (Coordinate.Angle - (motion.zangle + Angle_cnt * 360));
	double Temp_Minus = -Temp_Plus;
	
	if(Temp_Plus > 340 && Temp_Plus < 380)		//若反馈角度从180跳变至-180，则说明底盘向着自旋正方向转过了一圈
	{
		Angle_cnt++;
	}
	else if(Temp_Minus > 340 && Temp_Minus < 380)	//若反馈角度从-180跳变至180，则说明底盘向着自旋负方向转过了一圈
	{
		Angle_cnt--;
	}
	else if(fabs(Temp_Plus) > 381)		//无效跳变,则放弃此次处理
	{
		return;
	}
	
	OriginData.Angle = motion.zangle + (double)(Angle_cnt * 360);	//把角度从[-180,180）变成[0,360）,再加上整数圈
	OriginData.point.X = motion.pos_x + R * sin(OriginData.Angle);
	OriginData.point.Y = motion.pos_y + R * (1.0 - cos(OriginData.Angle));
	
	if(fabs(OriginData.Angle) > 32535.0)							//若超出了数据的传递范围（16位整型）
	{
		OriginData.Angle -= Angle_cnt * 360;
		Angle_cnt = 0;
	}
}

/**
* @brief  获取当前基于全局定位坐标基准的坐标增量
* @param  Current_O:当前的全局定位符合使用格式的回传值
* @param  Base_O:上次校准时产生的基于全局定位的坐标基准
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
* @brief  扭正全局定位坐标值
* @param  Increment：当前坐标基于全局定位基准坐标的增量
* @retval None
*/
static Point CorrectCoordinateIncrement(Point Increment)
{
	Matrix CorrectedIncrement;
	Point Result;
	Matrix::CreatedArreyType aPoint = Matrix::CreatArray(2, 1);		//申请一个两行一列的数表
	
	aPoint.Pointer[0][0] = Increment.X;
	aPoint.Pointer[1][0] = Increment.Y;
	
	CorrectedIncrement.fillMatrix(aPoint);
	Matrix::DeleteArray(aPoint);									//释放内存空间以节约资源
	
	CorrectedIncrement = CorrectedIncrement * CorrectMat;
	
	Result.X = CorrectedIncrement.getElementValue(0, 0);
	Result.Y = CorrectedIncrement.getElementValue(1, 0);
	
	return Result;
}

/**
* @brief  全局定位参量初始化
* @param  None
* @retval 无
*/
void MapParamsInit(void)
{
	CorrectMat = CorrectMat.getE(2);
	AngleOffset = 0;
	
	SetOrigin();
}

/**
* @brief  校正坐标（获得准确的实际坐标值之后调用）
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
* @brief  更新实际坐标值
* @param  None
* @retval None
*/
void UpdateRealCoordinate(void)
{
	Point IncreamentPoint;
	
	GetOriginalCoordinate(motion, OriginalCoord);	//获取原始坐标
	
	IncreamentPoint = CorrectCoordinateIncrement(getCurrentCoordIncrement(OriginalCoord, CoordBase_O));
	
	Coordinate.point.X = IncreamentPoint.X;
	Coordinate.point.Y = IncreamentPoint.Y;
	
	Coordinate.Angle = OriginalCoord.Angle + AngleOffset;
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
