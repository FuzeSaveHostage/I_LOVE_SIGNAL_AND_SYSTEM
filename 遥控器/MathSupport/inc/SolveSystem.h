#ifndef _SOVESYSTEM_H_
#define _SOVESYSTEM_H_

#define MatrixOrder 2

typedef union
{	
	struct{	double a11;	double a12;
			double a21;	double a22;
			}Mat;
	
	double a[MatrixOrder][MatrixOrder];
	
}CoefficientMatrix;

typedef struct
{
	double b[MatrixOrder];
	
}Vector;


extern "C"
{
	Vector SolveSecondOrder_System(CoefficientMatrix Co_mat, Vector b);
}

#endif
