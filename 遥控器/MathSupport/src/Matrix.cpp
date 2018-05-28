#include "Matrix.h"
#include <math.h>


#ifndef _INARM

#include <iomanip>
#include <iostream>

#define Debug	0

#endif

/**
  * @brief  ���������(Ĭ�Ϲ��캯��)
  * @param  None
  * @retval None 
  */
Matrix::Matrix(void)
{
	this->isInitialized = false;
	this->MatName = "This Mat";
	this->pMatrix = NULL;
	this->xOrder = 0;
	this->yOrder = 0;
#if Debug == 1
	std::cout << "\nCreat a NULL matrix frame!" << std::endl;
#endif
		
}

/**
  * @brief  ���������(���ƹ��캯��)
  * @param  mat:��һ���������
  * @retval None 
  */
Matrix::Matrix(const Matrix & mat)
{
	this->isInitialized = false;
	this->CreatMatrixFrame(NULL, mat.getxOrder(), mat.getyOrder());

	for(uint32_t i =0; i < this->xOrder; i++)
    	for(uint32_t j = 0; j < this->yOrder; j++)
    		this->pMatrix[i][j] = mat.getElementValue(i,j);

	this->MatName = "This Mat";
}

/**
  * @brief  �������
  * @param  ָ����������ָ��,��ΪNULL���򹹽����� 
  * @param  xOrder:������н��� 
  * @param  yOrder:������н���
  * @retval None 
  */
Matrix::Matrix(void* pArray, uint32_t _xOrder, uint32_t _yOrder)
{		
	this->isInitialized = false;
	this->CreatMatrixFrame(pArray, _xOrder, _yOrder);
	this->MatName = "This Mat";
}

/**
  * @brief  �������(˽��)
  * @param  cArray:���������
  * @retval None 
  */
Matrix::Matrix(CreatedArreyType cArray)
{	
	this->isInitialized = false;
	this->CreatMatrixFrame(cArray);
	this->MatName = "This Mat";

	this->DeleteArray(cArray);
}

/**
  * @brief  ���������� 
  * @param  None 
  * @retval None 
  */
Matrix::~Matrix(void)
{
	if (true == this->isInitialized && NULL != this->pMatrix)
	{
		for(uint32_t i = 0; i < this->xOrder; i++)
			delete [] this->pMatrix[i];
		
		delete [] this->pMatrix;
	}

	this->isInitialized = false;
		
#if Debug == 1
	std::cout << "\nDelete " << (this->MatName) << " matrix frame successfully!" << std::endl;
#endif
}

/**
  * @brief  ��������� 
  * @param  ָ����������ָ��,��ΪNULL���򹹽����� 
  * @param  xOrder:������н��� 
  * @param  yOrder:������н���
  * @retval None 
  */
void Matrix::CreatMatrixFrame(void* pArray, uint32_t _xOrder, uint32_t _yOrder)
{
	if (false == this->isInitialized)
	{
		this->isInitialized = true;
		this->xOrder = _xOrder;
		this->yOrder = _yOrder;

		this->pMatrix = new double*[this->xOrder];

		for (uint32_t i = 0; i < xOrder; i++)
			this->pMatrix[i] = new double[this->yOrder];

#if Debug == 1
		std::cout << "\nCreat matrix frame successfully!" << std::endl;
#endif

		if (NULL != pArray)
		{
			this->fillMatrix((double*)pArray);
		}
		else
		{
			CreatedArreyType pZero = this->CreatArray(_xOrder, _yOrder);

#if Debug == 1
			std::cout << "\nPassed the NULL point! Have created a zero matrix!" << std::endl;
#endif
			this->fillMatrix(pZero);

			this->DeleteArray(pZero);
		}
	}
#ifndef _INARM

	else
		std::cout << "\n<error>:Have initialized this Matrix!" << std::endl;
#endif	
		
}

/**
  * @brief  ��������� 
  * @param  ���������
  * @retval None 
  */
void Matrix::CreatMatrixFrame(CreatedArreyType cArray)
{
	if (false == this->isInitialized)
	{
		this->isInitialized = true;
		this->xOrder = cArray.xOrder;
		this->yOrder = cArray.yOrder;

		this->pMatrix = new double*[cArray.xOrder];

		for (uint32_t i = 0; i < xOrder; i++)
			this->pMatrix[i] = new double[cArray.yOrder];

#if Debug == 1
		std::cout << "\nCreat matrix frame successfully!" << std::endl;
#endif

		this->fillMatrix(cArray);
		
	}
#ifndef _INARM

	else
		std::cout << "\n<error>:Have initialized this Matrix!" << std::endl;
#endif
		
}

/**
  * @brief  ����������(˽��)
  * @param  ָ�����������Ԫ�ص�һ��ָ�� 
  * @retval None 
  */
void Matrix::fillMatrix(double* pArray)
{
#if Debug == 1
	std::cout << std::endl;
#endif
	this->isInitialized = true;

	for(uint32_t i =0; i < this->xOrder; i++)
    	for(uint32_t j = 0; j < this->yOrder; j++)
    	{
    		this->pMatrix[i][j] = (pArray + i*this->yOrder)[j];
#if Debug == 1
			std::cout << "\n<Private_pArray>:Filled in the pMatrix[" << i << "][" << j << "] = " << this->pMatrix[i][j] << std::endl;
#endif
		}

#if Debug == 1
	std::cout << std::endl;
#endif
    		
}

/**
  * @brief  ����������(˽��)
  * @param  ���뵽������ṹ�� 
  * @retval None 
  */
void Matrix::fillMatrix(CreatedArreyType cArray)
{
#if Debug == 1
	std::cout << std::endl;
#endif

	if (false == this->isInitialized)
	{
		this->CreatMatrixFrame(cArray);
	}
	else
	{
		for(uint32_t i =0; i < cArray.xOrder; i++)
    		for(uint32_t j = 0; j < cArray.yOrder; j++)
    		{
    			this->pMatrix[i][j] = cArray.Pointer[i][j];
	#if Debug == 1
		std::cout << "\n<Private_cArray>:Filled in the pMatrix[" << i << "][" << j << "] = " << this->pMatrix[i][j] << std::endl;
	#endif
			}
	}
	

#if Debug == 1
	std::cout << std::endl;
#endif
    		
}

/**
  * @brief  ���������ݣ�������
  * @param  ָ����������ָ�� 
  * @param  xOrder:������н��� 
  * @param  yOrder:������н���
  * @retval None 
  */
void Matrix::fillMatrix(void* pArray, uint32_t _xOrder, uint32_t _yOrder)
{
	double* Array = (double*)pArray;

	if (false == this->isInitialized)
	{
		this->CreatMatrixFrame(pArray, _xOrder, _yOrder);
	}
	else
	{
		if(_xOrder == this->xOrder && _yOrder == this->yOrder)
		{

	#if Debug == 1
		std::cout << std::endl;
	#endif

			for(uint32_t i =0; i < _xOrder; i++)
	    		for(uint32_t j = 0; j < _yOrder; j++)
	    		{
	#if Debug == 1
		std::cout << "\n<Public>:Filled in the pMatrix[" << i << "][" << j << "] = " << (Array + i*_yOrder)[j] << std::endl;
	#endif
	    			pMatrix[i][j] = (Array + i*_yOrder)[j];
				}

	#if Debug == 1
		std::cout << std::endl;
	#endif
		}
#ifndef _INARM

		else
			std::cout << "\nerror >> Invalid matrix operation: The Order mismatched!"  << std::endl;
#endif
	}

}

/**
  * @brief  ���þ��������(ȱʡΪ"This Mat")
  * @param  str:��������
  * @retval None
  */
void Matrix::setMatName(const char* str)
{
	this->MatName = str;
}

#ifndef _INARM

/**
  * @brief  ��ӡ����
  * @param  None
  * @retval None
  */
void Matrix::PrintMatrix(void)
{
	std::cout << std::endl << this->MatName << " = " << std::endl;
	this->PrintArray((this->pMatrix), this->xOrder, this->yOrder);
	std::cout << std::endl;
}

/**
  * @brief  ��ӡһ����ά���������(˽��)
  * @param  ָ�����������Ԫ�صĶ���ָ��
  * @param  xOrder:������н���
  * @param  yOrder:������н���
  * @retval None
  */
void Matrix::PrintArray(double ** pArray, uint32_t _xOrder, uint32_t _yOrder)
{
	std::cout << std::setprecision(4);
	for (uint32_t i = 0; i < _xOrder; i++)
	{
		for (uint32_t j = 0; j < _yOrder; j++)
			std::cout << pArray[i][j] << "\t";
		std::cout << std::endl;
	}
		
}

/**
  * @brief  ��ӡһ����ά���������(����)
  * @param  cArray:���������
  * @retval None
  */
void Matrix::PrintArray(const CreatedArreyType cArray)
{
	std::cout << std::setprecision(4);
	std::cout << "\n" << cArray.ArrayName << " = " << std::endl;
	for (uint32_t i = 0; i < cArray.xOrder; i++)
	{
		for (uint32_t j = 0; j < cArray.yOrder; j++)
			std::cout << cArray.Pointer[i][j] << "\t";
		std::cout << std::endl;
	}
		
}

#endif

/**
  * @brief  ����һ������,����ʼ������Ԫ��Ϊ��
  * @param  _xOrder:�н��� 
  * @param  _yOrder:�н���
  * @retval None 
  */
Matrix::CreatedArreyType Matrix::CreatArray(uint32_t _xOrder, uint32_t _yOrder)
{
	Matrix::CreatedArreyType Temp;
	Temp.Pointer = new double*[_xOrder];

	for (uint32_t i = 0; i < _xOrder; i++)
	{
		Temp.Pointer[i] = new double[_yOrder];

		for (uint32_t j = 0; j < _yOrder; j++)
		{
			Temp.Pointer[i][j] = 0.0;
		}
	}

	Temp.ArrayName = "This Array";
	Temp.xOrder = _xOrder;
	Temp.yOrder = _yOrder;
	Temp.isDeleted = false;

	return Temp;
}

/**
  * @brief  ɾ��һ�����������
  * @param  _xOrder:�н��� 
  * @param  _yOrder:�н���
  * @retval None 
  */
void Matrix::DeleteArray(CreatedArreyType cArray)
{
	if (false == cArray.isDeleted)
	{
		cArray.isDeleted = false;

		for (uint32_t i = 0; i < cArray.xOrder; i++)
			delete[] cArray.Pointer[i];

		delete[] cArray.Pointer;
	}
#ifndef _INARM

	else
		std::cout << "\nThis Array has been deleted!" << std::endl;
#endif
}

/**
  * @brief  ��ȡ������ĳ��Ԫ��ֵ 
  * @param  Subx:Ԫ�����±� 
  * @param  Suby:Ԫ�����±� 
  * @retval ���ظ�ָ��Ԫ�� 
  */
double Matrix::getElementValue(uint32_t Subx, uint32_t Suby) const
{
	double Temp = 0;
	if (Subx < this->xOrder && Suby < this->yOrder)
	{
		Temp = this->pMatrix[Subx][Suby];
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid subscript called!" << std::endl;
#endif

	return Temp; 
}

/**
  * @brief  ��ȡ�þ�����н���  
  * @param  None
  * @retval ���ظþ�����н��� 
  */
uint32_t Matrix::getxOrder(void) const
{
	return this->xOrder; 
}

/**
  * @brief  ��ȡ�þ�����н���  
  * @param  None
  * @retval ���ظþ�����н��� 
  */
uint32_t Matrix::getyOrder(void) const
{
	return this->yOrder; 
}

/**
  * @brief  ��ȡһ����λ�� 
  * @param  Order����λ��Ľ�������Ϊ0�����ñ�����ͬ�׵ĵ�λ��
  * @retval ���ظþ����Ӧ�ĵ�λ�������Ƿ��󣬷�������
  */
Matrix Matrix::getE(uint32_t Order = 0)
{
	CreatedArreyType Temp;
	Matrix Result;
	
	if(0 == Order)	//���������������������ͬ�ĵ�λ��
	{
		Temp = this->CreatArray(this->xOrder, this->yOrder);

		if (this->yOrder == this->xOrder)
		{
			for (uint32_t i = 0; i < this->xOrder; i++)
				Temp.Pointer[i][i] = 1;
		}
#ifndef _INARM

		else
			std::cout << "\n<error>: This Matrix is not a square matrix!" << std::endl;
#endif
	}
	else
	{
		Temp = this->CreatArray(Order, Order);

		for (uint32_t i = 0; i < Order; i++)
			Temp.Pointer[i][i] = 1;
	}
	
	Result.CreatMatrixFrame(Temp);
	
	this->DeleteArray(Temp);
	
	return Matrix(Result);
}

/**
  * @brief  ��ȡ������������
  * @param  None
  * @retval ���ظþ����Ӧ��������������棬��������
  */
Matrix Matrix::getInverseMatrix(void)
{
	double Temp;
	Matrix Mat = *this;
	Matrix result = this->getE();

//	result.PrintMatrix();

	if (this->yOrder == this->xOrder)// && true == this->isInvertible)
	{
		for (uint32_t i = 0; i < Mat.getyOrder(); i++)				//�������Ϊ�������н��ݲ���λ�����Խ�Ԫ
		{
			uint32_t AimRow = Mat.findAbsoluteMaxinColumn(i, i);

			Mat.rowExchange(AimRow, i);
			result.rowExchange(AimRow, i);

			Temp = Mat.getElementValue(i, i);
			Mat.rowOperation(i, i, 1.0 / Temp);
			result.rowOperation(i, i, 1.0 / Temp);

//			Mat.PrintMatrix();
//			result.PrintMatrix();

			for (uint32_t j = i + 1; j < Mat.getxOrder(); j++)
			{
				Temp = -(Mat.getElementValue(j, i));

				Mat.rowOperation(i, j, Temp);
				result.rowOperation(i, j, Temp);

//				Mat.PrintMatrix();
			}
		}

		for (uint32_t i = Mat.getyOrder(); i > 0; i--)					//����ԭ����Ϊ��λ�󣬽�����ȡ�����
		{
			for (int32_t j = i - 2; j >= 0; j--)
			{
				Temp = -(Mat.getElementValue(j, i - 1));

				Mat.rowOperation(i - 1, j, Temp);
				result.rowOperation(i - 1, j, Temp);

//				Mat.PrintMatrix();
			}
//			Mat.PrintMatrix();
		}
	}
#ifndef _INARM

	else
		std::cout << "\n<error>: This Matrix is not a invertible matrix!" << std::endl;
#endif

	return Matrix(result);
}

/**
  * @brief  Ѱ��ָ������ָ����Χ�ڵ����Ԫ��������
  * @param  Suby:ָ�����±�
  * @param  xStart:ָ����ʼ���±꣬�Ӵ����±���
  * @retval ���ظþ�����н���
  */
uint32_t Matrix::findAbsoluteMaxinColumn(uint32_t Suby, uint32_t xStart)
{
	uint32_t Temp = xStart;

	if (Suby < this->yOrder)
	{
		if (xStart < this->xOrder)
		{
			for (uint32_t i = xStart; i < this->xOrder; i++)
			{
				if (fabs(this->pMatrix[i][Suby]) > fabs(this->pMatrix[Temp][Suby]))
					Temp = i;
			}
		}
#ifndef _INARM

		else
			std::cout << "\n<Overflow>: Invalid xStart Value!" << std::endl;
#endif
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid column Value!" << std::endl;
#endif

	return Temp;
}

/**
  * @brief  �����жԻ�
  * @param  x0:�����������±� 
  * @param  x1:�����������±� 
  * @retval None
  */
void Matrix::rowExchange(uint32_t x0, uint32_t x1)
{
	double Temp = 0.0;

	if (x0 < this->xOrder && x1 < this->xOrder)
	{
		for (uint32_t i = 0; i < this->yOrder; i++)
		{
			Temp = this->pMatrix[x0][i];
			this->pMatrix[x0][i] = this->pMatrix[x1][i];
			this->pMatrix[x1][i] = Temp;
		}
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid row subscript!" << std::endl;
#endif
}

/**
  * @brief  �����жԻ�
  * @param  y0:�����������±�
  * @param  y1:�����������±�
  * @retval None
  */
void Matrix::columnExchange(uint32_t y0, uint32_t y1)
{
	double Temp = 0.0;

	if (y0 == y1)
		return;

	if (y0 < this->yOrder && y1 < this->yOrder)
	{
		for (uint32_t i = 0; i < this->xOrder; i++)
		{
			Temp = this->pMatrix[i][y0];
			this->pMatrix[i][y0] = this->pMatrix[i][y1];
			this->pMatrix[i][y1] = Temp;
		}
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid column subscript!" << std::endl;
#endif

	return;
}


/**
  * @brief  �����б任(�����±�һ�£��򽫻��ϵ���˵�Ŀ������)
  * @param  xSrc:�������任���е����±�
  * @param  xTarget:���任���е����±�
  * @param  coefficient:ϵ��
  * @retval None
  */
void Matrix::rowOperation(uint32_t xSrc, uint32_t xTarget, double coefficient)
{
	if (xSrc < this->xOrder && xTarget < this->xOrder)
	{
		if (xSrc == xTarget)
		{
			for (uint32_t i = 0; i < this->yOrder; i++)
			{
				this->pMatrix[xTarget][i] *=  coefficient;
			}
		}
		else
		{
			for (uint32_t i = 0; i < this->yOrder; i++)
			{
				this->pMatrix[xTarget][i] += this->pMatrix[xSrc][i] * coefficient;
			}
		}
		
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid row subscript!" << std::endl;
#endif
}


/**
  * @brief  �����б任(�����±�һ�£��򽫻��ϵ���˵�Ŀ������)
  * @param  ySrc:�������任���е����±�
  * @param  yTarget:���任���е����±�
  * @param  coefficient:ϵ��
  * @retval None
  */
void Matrix::columnOperation(uint32_t ySrc, uint32_t yTarget, double coefficient)
{
	if (ySrc < this->yOrder && yTarget < this->yOrder)
	{
		if (ySrc == yTarget)
		{
			for (uint32_t i = 0; i < this->xOrder; i++)
			{
				this->pMatrix[i][yTarget] *= coefficient;
			}
		}
		else
		{
			for (uint32_t i = 0; i < this->xOrder; i++)
			{
				this->pMatrix[i][yTarget] += this->pMatrix[i][ySrc] * coefficient;
			}
		}
	}
#ifndef _INARM

	else
		std::cout << "\n<Overflow>:Invalid column subscript!" << std::endl;
#endif
}

/* brief  ��ȡת�þ���
 * param  None
 * retval ת�þ���
 */
Matrix Matrix::elicitTranspose(void)
{
	CreatedArreyType Temp = this->CreatArray(this->yOrder, this->xOrder);
	Matrix result;

	if (true == this->isInitialized)
	{
		for (uint32_t i = 0; i < Temp.xOrder; i++)
			for (uint32_t j = 0; j < Temp.yOrder; j++)
				Temp.Pointer[i][j] = this->pMatrix[j][i];
	}
#ifndef _INARM

	else
		std::cout << "\nThis Mat haven't been initialized!" << std::endl;
#endif

	result = Temp;

	this->DeleteArray(Temp);

	return Matrix(result);
}

/* brief  ����"-"���������Ԫ��ȡ��
 * param  mat:�������
 * retval None
 */
Matrix Matrix::operator-(void)
{
	CreatedArreyType Temp = this->CreatArray(this->yOrder, this->xOrder);
	Matrix result;

	if (true == this->isInitialized)
	{
		for (uint32_t i = 0; i < Temp.xOrder; i++)
			for (uint32_t j = 0; j < Temp.yOrder; j++)
				Temp.Pointer[i][j] = -this->pMatrix[i][j];
	}
#ifndef _INARM

	else
		std::cout << "\nThis Mat haven't been initialized!" << std::endl;
#endif

	result = Temp;

	this->DeleteArray(Temp);

	return Matrix(result);
}

/* brief  ����"="��������Զ���ֵ
 * param  mat:�������
 * retval None
 */
void Matrix::operator= (const Matrix &mat)
{
	if (true == this->isInitialized)
	{
		if(mat.getxOrder() == this->xOrder && mat.getyOrder() == this->yOrder)
		{
    		for(uint32_t i =0; i < this->xOrder; i++)
    			for(uint32_t j = 0; j < this->yOrder; j++)
    				this->pMatrix[i][j] = mat.getElementValue(i,j);
		}
#ifndef _INARM

		else
			std::cout << "\nerror >> Invalid matrix operation: The Order mismatched!"  << std::endl;
#endif
	}
	else
	{
		this->CreatMatrixFrame(NULL, mat.getxOrder(), mat.getyOrder());

		for(uint32_t i =0; i < this->xOrder; i++)
    		for(uint32_t j = 0; j < this->yOrder; j++)
    			this->pMatrix[i][j] = mat.getElementValue(i,j);

	}
		
}
/* brief  ����"="��������Զ���ֵ
 * param  cArray:��Ӧ����
 * retval None
 */

void Matrix::operator=(const CreatedArreyType & cArray)
{
	if (false == this->isInitialized)
	{
		this->CreatMatrixFrame(cArray);
	}
	else
	{
		if(cArray.xOrder == this->xOrder && cArray.yOrder == this->yOrder)
		{
    		for(uint32_t i =0; i < cArray.xOrder; i++)
    			for(uint32_t j = 0; j < cArray.yOrder; j++)
    			{
    				this->pMatrix[i][j] = cArray.Pointer[i][j];
		#if Debug == 1
			std::cout << "\n<Public_cArray>:Filled in the pMatrix[" << i << "][" << j << "] = " << this->pMatrix[i][j] << std::endl;
		#endif
				}
		}
#ifndef _INARM

		else
			std::cout << "\nerror >> Invalid matrix operation: The Order mismatched!"  << std::endl;
#endif
	}
}

/* brief  ����"+"��������������
 * param  mat:�������
 * retval һ���������
 */
Matrix Matrix::operator+ (const Matrix &mat)
{
	CreatedArreyType Temp = this->CreatArray(this->xOrder, this->yOrder);
	Matrix result;

	if(mat.getxOrder() == this->xOrder && mat.getyOrder() == this->yOrder)
    {
    	for(uint32_t i =0; i < this->xOrder; i++)
    		for(uint32_t j = 0; j < this->yOrder; j++)
    			Temp.Pointer[i][j] = this->pMatrix[i][j] + mat.getElementValue(i,j);
	}
#ifndef _INARM

	else
		std::cout << "\nerror >> Invalid matrix operation: The Order mismatched!"  << std::endl;
#endif

	result = Temp;

	this->DeleteArray(Temp);

    return Matrix(result);
}

/* brief  ����"*"��������������
 * param  mat:�������
 * retval None
 */
Matrix Matrix::operator* (const Matrix &mat)
{
	CreatedArreyType Temp = this->CreatArray(this->xOrder, mat.getyOrder());
	Matrix result;

	if(mat.getxOrder() == this->yOrder)
    {
    	for(uint32_t i =0; i < Temp.xOrder; i++)
			for (uint32_t j = 0; j < Temp.yOrder; j++)
			{
				for(uint32_t k = 0; k < this->yOrder; k++)
					Temp.Pointer[i][j] += this->pMatrix[i][k] * mat.getElementValue(k,j);
#if Debug == 1
	std::cout << "\n<*>:Filled in the Temp.Pointer[" << i << "][" << j << "] = " << Temp.Pointer[i][j] << std::endl;
#endif
			}
	}
#ifndef _INARM

	else
		std::cout << "\nerror >> Invalid matrix operation: The Order mismatched!"  << std::endl;
#endif

	result = Temp;

	this->DeleteArray(Temp);

    return Matrix(result);
}

/* brief  ����"*"�������ĳ����������
 * param  mat:�������
 * param  Num:ĳ����
 * retval ������
 */
Matrix operator*(const double & Num, const Matrix & mat)
{
	Matrix::CreatedArreyType Temp = Matrix::CreatArray(mat.getxOrder(), mat.getyOrder());
	Matrix result;

    for(uint32_t i =0; i < Temp.xOrder; i++)
		for (uint32_t j = 0; j < Temp.yOrder; j++)
		{
			Temp.Pointer[i][j] *= Num;
#if Debug == 1
	std::cout << "\n<*>:Filled in the Temp.Pointer[" << i << "][" << j << "] = " << Temp.Pointer[i][j] << std::endl;
#endif
		}

	result = Temp;

	Matrix::DeleteArray(Temp);

    return Matrix(result);
}

#undef Debug


