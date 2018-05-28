#ifndef _MAT_H_
#define _MAT_H_

#include <stdint.h>
#include <string>

#define VariableName(x)		#x					//将变量转换为同义字符串
#define MatName(x)			VariableName(x)
#define ArrayName(x)		VariableName(x)

#define _INARM

class Matrix 
{
	public:
		typedef struct
		{
			double** Pointer;
			std::string ArrayName;
			uint32_t xOrder;
			uint32_t yOrder;
			bool isDeleted;

		}CreatedArreyType;

	public:
		Matrix(void);												//默认构造
		Matrix(const Matrix &mat);									//复制构造
		Matrix(void* pArray, uint32_t _xOrder, uint32_t _yOrder);	//普通构造
		~Matrix(void);
		
		void setMatName(const char* str);
		uint32_t getxOrder(void) const;
		uint32_t getyOrder(void) const;
		Matrix getE(uint32_t Order);
		Matrix getInverseMatrix(void);
		uint32_t findAbsoluteMaxinColumn(uint32_t Suby, uint32_t xStart);
		double getElementValue(uint32_t Subx, uint32_t Suby) const;
		void fillMatrix(void* pArray, uint32_t _xOrder, uint32_t _yOrder);
		void fillMatrix(CreatedArreyType cArray);
		
		void rowExchange(uint32_t x0, uint32_t x1);
		void columnExchange(uint32_t y0, uint32_t y1);
		void rowOperation(uint32_t xSrc, uint32_t xTarget, double coefficient);
		void columnOperation(uint32_t ySrc, uint32_t yTarget, double coefficient);
		Matrix elicitTranspose(void);
		Matrix operator+ (const Matrix &mat); 
		Matrix operator* (const Matrix &mat);
		friend Matrix operator* (const double &Num, const Matrix &mat);
		Matrix operator-(void);
		void operator= (const Matrix &mat);
		void operator= (const CreatedArreyType &cArray);

		static CreatedArreyType CreatArray(uint32_t _xOrder, uint32_t _yOrder);
		static void DeleteArray(CreatedArreyType cArray);

#ifndef _INARM
		void PrintMatrix(void);
		static void PrintArray(const CreatedArreyType cArray);
#endif

	private:
		Matrix(CreatedArreyType cArray);
		void CreatMatrixFrame(void* pArray, uint32_t _xOrder, uint32_t _yOrder);
		void CreatMatrixFrame(CreatedArreyType cArray);

		void fillMatrix(double* pArray);

#ifndef _INARM
		static void PrintArray(double** pArray, uint32_t _xOrder, uint32_t _yOrder);
#endif
	
	private:
		std::string MatName;
		uint32_t xOrder;
		uint32_t yOrder;
		double** pMatrix;
		bool isInitialized;			//标识该矩阵框架是否被初始化
		bool isInvertible ;			//标识该矩阵是否可逆
};


#endif

