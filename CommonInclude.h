#pragma once
#include <windows.h> // for XMVerifyCPUSupport
#include<DirectXMath.h>//DX12 SIMD Math数学库 代码都以内联方式实现在头文件里，不需要额外添加库
#include<DirectXPackedVector.h>//DX12 一些相关的数据类型
#include<iostream>

using namespace std;//std::ostream std::cout ...
using namespace DirectX;//DirectX::FXMVECTOR ...




namespace MyXMFuncLib
{

	//没有2X2,自己定义一个
	struct XMFLOAT2X2
	{
		union
		{
			struct
			{
				float _11, _12;
				float _21, _22;
			};
			float m[2][2];
		};

		XMFLOAT2X2() = default;

		XMFLOAT2X2(const XMFLOAT2X2&) = default;
		XMFLOAT2X2& operator=(const XMFLOAT2X2&) = default;

		XMFLOAT2X2(XMFLOAT2X2&&) = default;
		XMFLOAT2X2& operator=(XMFLOAT2X2&&) = default;

		constexpr XMFLOAT2X2(float m00, float m01,
			float m10, float m11) noexcept
			: _11(m00), _12(m01),
			_21(m10), _22(m11) {}
		explicit XMFLOAT2X2(_In_reads_(4) const float* pArray) noexcept
		{
			assert(pArray != nullptr);
			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[1][0] = pArray[2];
			m[1][1] = pArray[3];
		}

		float  operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
	};

	//重载<<运算符号，使能输出XMVECTOR

	//XM_CALLCONV:为调用约定注解(构造函数不要使用);
	//函数形参中:
	//前三个XMVECTOR应当用FXMVECTOR
	//第四个XMVECTOR应当用GXMVECTOR
	//第五、六个XMVECTOR应当用HXMVECTOR
	//其余用CXMVECTOR
	//函数传参会把至多前六个XMVECTOR传进寄存器中加快速度，多了或者不用推荐类型则压栈
	ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v);

	//重载<<运算符号，使能输出XMMATRIX0

	//一个XMMATRI相当于四个XMVECTOR，定义一个FXMMATRIX相当于定义了四个XMVECTOR
	ostream& XM_CALLCONV operator<<(ostream& os, FXMMATRIX m);

	//返回V向量在N向量方向上的投影向量(V关于N的平行分量)
	XMVECTOR XM_CALLCONV XMVector3ProjectionNV(FXMVECTOR V, FXMVECTOR N);

	//返回V向量关于N向量的垂直分量(V关于N的正交分量)
	XMVECTOR XM_CALLCONV XMVector3PerPendicularNV(FXMVECTOR V, FXMVECTOR N);

	//XMMATRIX存到XMFLOAT2X2
	void XM_CALLCONV XMStoreFloat2x2(XMFLOAT2X2* pDestination,FXMMATRIX M);

	//XMFLOAT2X2加载到XMMATRIX
	XMMATRIX XM_CALLCONV XMLoadFloat2x2(const XMFLOAT2X2* pSource);


	//余子阵：(minor matrix) MinorA(i,j)即为A去除第i行和第j列之后剩下的i - 1行 j - 1列矩阵
	//代数余子式：(cofactor of Aij) C(i,j) = (-1)^(i+j)Det(MinorA(i,j))
	//代数余子式矩阵：(cofactor matrix of A):将矩阵A的所有第i行 第j列元素替换为其代数余子式矩阵后得到的矩阵
	//伴随矩阵：(adjoint matrix of A,A*)代数余子式矩阵的转置矩阵
	//AA* = |A|E ,故A^-1 = A*/|A|

	//3X3矩阵的余子阵MinorA(i,j)
	XMFLOAT2X2 XM_CALLCONV XM3X3MatrixMinorMatrix(XMFLOAT3X3 M, int _i, int _j);
	XMFLOAT2X2 XM_CALLCONV XM3X3MatrixMinorMatrix(FXMMATRIX M, int _i, int _j);

	//3X3矩阵的代数余子式C(i,j)
	float XM_CALLCONV XM3X3MatrixCofactor(XMFLOAT3X3 M, int _i, int _j);
	float XM_CALLCONV XM3X3MatrixCofactor(FXMMATRIX M, int _i, int _j);

	//4X4矩阵的余子阵MinorA(i,j)
	XMFLOAT3X3 XM_CALLCONV XM4X4MatrixMinorMatrix(XMFLOAT4X4 M, int _i, int _j);
	XMFLOAT3X3 XM_CALLCONV XM4X4MatrixMinorMatrix(FXMMATRIX M, int _i, int _j);

	//4X4矩阵的代数余子式C(i,j)
	float XM_CALLCONV XM4X4MatrixCofactor(XMFLOAT4X4 M, int _i, int _j);
	float XM_CALLCONV XM4X4MatrixCofactor(FXMMATRIX M, int _i, int _j);

	//3X3矩阵的代数余子式矩阵
	XMFLOAT3X3 XM_CALLCONV XM3X3MatrixCofactorMatrix(XMFLOAT3X3 M);
	XMFLOAT3X3 XM_CALLCONV XM3X3MatrixCofactorMatrix(FXMMATRIX M);

	//4X4矩阵的代数余子式矩阵
	XMFLOAT4X4 XM_CALLCONV XM4X4MatrixCofactorMatrix(XMFLOAT4X4 M);
	XMFLOAT4X4 XM_CALLCONV XM4X4MatrixCofactorMatrix(FXMMATRIX M);

	//3X3矩阵的伴随矩阵
	XMFLOAT3X3 XM_CALLCONV XM3X3MatrixAdjointMatrix(XMFLOAT3X3 M);
	XMFLOAT3X3 XM_CALLCONV XM3X3MatrixAdjointMatrix(FXMMATRIX M);

	//4X4矩阵的伴随矩阵
	XMFLOAT4X4 XM_CALLCONV XM4X4MatrixAdjointMatrix(XMFLOAT4X4 M);
	XMFLOAT4X4 XM_CALLCONV XM4X4MatrixAdjointMatrix(FXMMATRIX M);

	//3X3矩阵的逆矩阵
	XMMATRIX  XM_CALLCONV XM3X3MatrixInverseMatrix(XMFLOAT3X3 M);
	XMMATRIX  XM_CALLCONV XM3X3MatrixInverseMatrix(FXMMATRIX M);

	//4X4矩阵的逆矩阵
	XMMATRIX  XM_CALLCONV XM4X4MatrixInverseMatrix(XMFLOAT4X4 M);
	XMMATRIX  XM_CALLCONV XM4X4MatrixInverseMatrix(FXMMATRIX M);

	//行列式：(Determinant)，方阵才有行列式，n阶方阵的行列式 Dn = ∑(-1)^k(a(1,k1)*a(2,k2)...*a(n,kn)),其中k1k2...kn为1~n的某一排列,
	//k为该排列的逆序数，∑表示对所有排列情况求和
	//2X2矩阵的行列式
	float XM_CALLCONV XM2X2MatrixDeterminant(XMFLOAT2X2 M);
	float XM_CALLCONV XM2X2MatrixDeterminant(FXMMATRIX M);
	//3X3矩阵的行列式
	float XM_CALLCONV XM3X3MatrixDeterminant(XMFLOAT3X3 M);
	float XM_CALLCONV XM3X3MatrixDeterminant(FXMMATRIX M);
	//4X4矩阵的行列式
	float XM_CALLCONV XM4X4MatrixDeterminant(XMFLOAT4X4 M);
	float XM_CALLCONV XM4X4MatrixDeterminant(FXMMATRIX M);

	//取XMVECTOR的第几个分量
	float XM_CALLCONV GetVecWeight(FXMVECTOR V, int WeightNum);
	//取矩阵某一行行向量
	XMVECTOR XM_CALLCONV RowVector(FXMMATRIX M, int RowNum);
	//取矩阵某一列列向量
	XMVECTOR XM_CALLCONV ColumnVector(FXMMATRIX M, int ColNum);
	
	//重载矩阵与向量乘法
	//v*m，视为行向量乘以矩阵
	XMVECTOR XM_CALLCONV operator*(FXMVECTOR v, FXMMATRIX m);
	//m*v，视为矩阵乘以列向量
	XMVECTOR XM_CALLCONV operator*(FXMMATRIX m, FXMVECTOR v);

	/***************************************************************	
		@brief  获得旋转矩阵  	
		@param    
			Axis:旋转轴
			Angle:从Axis的指向方向看，逆时针旋转的角度
			bRightHandCoordination:左手系还是右手系(都是笛卡尔垂直坐标系)
	***************************************************************/
	XMMATRIX XM_CALLCONV GetRotateMatrix(FXMVECTOR Axis, float Angle, bool bRightHandCoordination = true);
};
