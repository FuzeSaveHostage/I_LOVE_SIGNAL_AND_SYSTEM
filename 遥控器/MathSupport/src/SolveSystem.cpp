#include "SolveSystem.h"
#include "PlaneAnalyticGeometry.h"
#include "Matrix.h"


	
/**
  * @brief  用Doolittle分解法解一个二阶线性方程组
  * @param  Mat：二阶系数矩阵
  * @param  B:增广阵的增广部分
  * @retval 解得的二维解向量
  */

Vector SolveSecondOrder_System(CoefficientMatrix Co_mat, Vector b)
{
	Vector result = {0};
	
	Matrix::CreatedArreyType aL = Matrix::CreatArray(sizeof(Co_mat.a)/sizeof(Co_mat.a[0]), sizeof(Co_mat.a[0])/sizeof(double));
	Matrix::CreatedArreyType aU = Matrix::CreatArray(sizeof(Co_mat.a)/sizeof(Co_mat.a[0]), sizeof(Co_mat.a[0])/sizeof(double));
	Matrix::CreatedArreyType aX;
	Matrix::CreatedArreyType aY;

	Matrix Mat(Co_mat.a, sizeof(Co_mat.a)/sizeof(Co_mat.a[0]), sizeof(Co_mat.a[0])/sizeof(double));
	Matrix B(b.b, sizeof(b.b)/sizeof(b.b[0]), 1);
	
	Matrix Y;
	Matrix L;
	Matrix U;

#ifdef _DebugMatrix
	Matrix X;
	Matrix A;
	Matrix Temp = Mat;
	Matrix mat_result;
#endif
	
	aL.ArrayName = ArrayName(aL);
	aU.ArrayName = ArrayName(aU);
	
	Y.setMatName(MatName(Y));
	L.setMatName(MatName(L));
	U.setMatName(MatName(U));
	B.setMatName(MatName(B));
	Mat.setMatName(MatName(Mat));
	
#ifdef _DebugMatrix
	A.setMatName(MatName(A));
	X.setMatName(MatName(X));
	Temp.setMatName(MatName(Temp));
	mat_result.setMatName(MatName(result));
#endif

	for (uint32_t i = 0; i < Mat.getxOrder(); i++)			//Doolittle分解
		aL.Pointer[i][i] = 1;

	for (uint32_t i = 0; i < Mat.getxOrder(); i++)
	{
		for (uint32_t j = 0; j < i; j++)					//获取第i行的L参数
		{
			aL.Pointer[i][j] = Mat.getElementValue(i, j);

			for (uint32_t k = 0; k < j; k++)
				aL.Pointer[i][j] -= aL.Pointer[i][k] * aU.Pointer[k][j];

			aL.Pointer[i][j] /=  aU.Pointer[j][j];
		}

		for (uint32_t j = i; j < Mat.getyOrder(); j++)		//获取第i行的U参数
		{
			aU.Pointer[i][j] = Mat.getElementValue(i, j);

			for (uint32_t k = 0; k < i; k++)
				aU.Pointer[i][j] -= aL.Pointer[i][k] * aU.Pointer[k][j];
		}
	}
	
	Mat.~Matrix();				//释放内存空间以节约资源
	
	L.fillMatrix(aL);
	Matrix::DeleteArray(aL);	//释放内存空间以节约资源
	
	U.fillMatrix(aU);
	Matrix::DeleteArray(aU);	//释放内存空间以节约资源
	
#ifdef _DebugMatrix
	A = L * U;
#endif
	
	aX = Matrix::CreatArray(sizeof(b.b)/sizeof(b.b[0]), 1);
	aY = Matrix::CreatArray(sizeof(b.b)/sizeof(b.b[0]), 1);
	
	aX.ArrayName = ArrayName(aX);
	aY.ArrayName = ArrayName(aY);
	
	for (uint32_t i = 0; i < L.getxOrder(); i++)	//第一次回代，获取Y向量
	{
		aY.Pointer[i][0] = B.getElementValue(i, 0);

		for (uint32_t j = 0; j < i; j++)
			aY.Pointer[i][0] -= aY.Pointer[j][0] * L.getElementValue(i, j);
	}
	Y.fillMatrix(aY);

	for (uint32_t i = U.getxOrder(); i > 0; i--)	//第二次回代，获取X向量
	{
		aX.Pointer[i-1][0] = Y.getElementValue(i-1, 0);

		for (uint32_t j = U.getxOrder() - 1; j > i-1; j--)
			aX.Pointer[i-1][0] -= aX.Pointer[j][0] * U.getElementValue(i-1, j);

		aX.Pointer[i - 1][0] /= U.getElementValue(i - 1, i - 1);
	}
	
	for(uint32_t i = 0; i < MatrixOrder; i++)
	{
		result.b[i] = aX.Pointer[i][0];
	}
	
#ifdef _DebugMatrix
	X.fillMatrix(aX);

	mat_result = Temp * X;
#endif

	Matrix::DeleteArray(aX);
	Matrix::DeleteArray(aY);
	
	return result;
}


