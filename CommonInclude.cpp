#include"CommonInclude.h"



ostream& XM_CALLCONV MyXMFuncLib::operator<<(ostream& os, FXMVECTOR v)
{

	//XMVECTOR存粹用来实现高速向量运算的
	//局部或全局变量用XMVECTOR类型
	//对于类中数据成员，或者需要用v.x v.y v.z取值的数据用XMFLOAT2/3/4类型
	//运算前，用XV = XMLoadFloat2/3/4(XF)转成高速运算向量
	//用XMVECTOR实例进行高速向量运算
	//用XMStore2/3/4(XF,XV)将XMVECTOR类型转成XMFLOAT2/3/4类型，或者用XMVectorGet/SET X/Y/Z/W 直接得到/设置分量值
	XMFLOAT3 Vector;
	XMStoreFloat3(&Vector, v);

	os << '(' << Vector.x << ',' << Vector.y << ',' << Vector.z << ')';
	return os;
}

ostream& XM_CALLCONV MyXMFuncLib::operator<<(ostream& os, FXMMATRIX m)
{
	//-----------------不转成XMStoreFloat4x4的写法----------------------
	//for (int i = 0; i < 4; i++)
	//{
	//	os << XMVectorGetX(m.r[i]) << "\t";
	//	os << XMVectorGetY(m.r[i]) << "\t";
	//	os << XMVectorGetZ(m.r[i]) << "\t";
	//	os << XMVectorGetW(m.r[i]);
	//	os << endl;
	//}
	//-----------------不转成XMStoreFloat4x4的写法----------------------


	//-----------------转成XMStoreFloat4x4的写法----------------------
	//用XM = XMLoadFloatNxN(XF)将存储矩阵转成高速运算矩阵
	//用XMStoreFloat4X4(XF,XM)将高速运算矩阵存储到存储矩阵
	XMFLOAT4X4 StoreMatrix;
	XMStoreFloat4x4(&StoreMatrix, m);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			os << StoreMatrix(i, j) << "\t";
		}
		os << endl;
	}
	//-----------------不转成XMStoreFloat4x4的写法----------------------


	return os;
}

XMVECTOR XM_CALLCONV MyXMFuncLib::XMVector3ProjectionNV(FXMVECTOR V, FXMVECTOR N)
{
	XMVECTOR VNDot = XMVector3Dot(V, N);
	//ProjnV = (Dot(v,n)/|n|)*(n/|n|) = (Dot(v,n)*n/(|n|^2))(投影长度数乘投影方向的单位向量)
	return XMVectorDivide(XMVectorMultiply(VNDot, N), XMVector3LengthSq(N));
}

XMVECTOR XM_CALLCONV MyXMFuncLib::XMVector3PerPendicularNV(FXMVECTOR V, FXMVECTOR N)
{
	//PerpnV = V - ProjnV
	return V - XMVector3ProjectionNV(V, N);
}

void XM_CALLCONV MyXMFuncLib::XMStoreFloat2x2(XMFLOAT2X2* pDestination, FXMMATRIX M)
{
	assert(pDestination);

	//不想去定义多平台的，用自带的中转一下
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	pDestination->m[0][0] = TempStoreF4X4.m[0][0];
	pDestination->m[0][1] = TempStoreF4X4.m[0][1];
	pDestination->m[1][0] = TempStoreF4X4.m[1][0];
	pDestination->m[1][1] = TempStoreF4X4.m[1][1];
}

XMMATRIX XM_CALLCONV MyXMFuncLib::XMLoadFloat2x2(const XMFLOAT2X2* pSource)
{
	assert(pSource);

	//不想去定义多平台的，用自带的中转一下
	XMFLOAT4X4 TempStoreF4X4;

	TempStoreF4X4.m[0][0] = pSource->m[0][0];
	TempStoreF4X4.m[0][1] = pSource->m[0][1];
	TempStoreF4X4.m[1][0] = pSource->m[1][0];
	TempStoreF4X4.m[1][1] = pSource->m[1][1];

	return XMLoadFloat4x4(&TempStoreF4X4);
}


MyXMFuncLib::XMFLOAT2X2 XM_CALLCONV MyXMFuncLib::XM3X3MatrixMinorMatrix(XMFLOAT3X3 M, int _i, int _j)
{
	XMFLOAT2X2 ResultF2X2 = XMFLOAT2X2(0.0f, 0.0f, 0.0f, 0.0f);

	//容错
	if (_i < 0 || _i >= 3 || _j < 0 || _j >= 3)
		return ResultF2X2;

	int ValueNum = 0;//0 ~ 3，共4个

	//忽略第_i行第_j列，得到余子阵
	for (int k = 0; k < 3; k++)
	{
		if (k == _i)
			continue;

		for (int l = 0; l < 3; l++)
		{
			if (l == _j)
				continue;

			ResultF2X2.m[ValueNum / 2][ValueNum % 2] = M.m[k][l];
			ValueNum++;//相信代码没问题，不用容错退出，错了再说^^
		}
	}

	return ResultF2X2;
}

MyXMFuncLib::XMFLOAT2X2 XM_CALLCONV MyXMFuncLib::XM3X3MatrixMinorMatrix(FXMMATRIX M, int _i, int _j)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	XMFLOAT2X2 ResultF2X2 = XM3X3MatrixMinorMatrix(TempStoreF3X3, _i, _j);
	return ResultF2X2;
}

float XM_CALLCONV MyXMFuncLib::XM3X3MatrixCofactor(XMFLOAT3X3 M, int _i, int _j)
{
	//容错
	if (_i < 0 || _i >= 3 || _j < 0 || _j >= 3)
		return 0.0f;
	//C(i,j) = (-1)^(i + j)Det(MinorA(i, j))
	float p = float(pow(-1, _i + _j));
	float Cofactor = p * XM2X2MatrixDeterminant(XM3X3MatrixMinorMatrix(M, _i, _j));
	return Cofactor;
}

float XM_CALLCONV MyXMFuncLib::XM3X3MatrixCofactor(FXMMATRIX M, int _i, int _j)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	float Cofactor = XM3X3MatrixCofactor(TempStoreF3X3, _i, _j);
	return Cofactor;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM4X4MatrixMinorMatrix(XMFLOAT4X4 M, int _i, int _j)
{
	XMFLOAT3X3 ResultF3X3 = XMFLOAT3X3(0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f);

	//容错
	if (_i < 0 || _i >= 4 || _j < 0 || _j >= 4)
		return ResultF3X3;

	int ValueNum = 0;//0 ~ 8,共9个

	//忽略第_i行第_j列，得到余子阵
	for (int k = 0; k < 4; k++)
	{
		if (k == _i)
			continue;

		for (int l = 0; l < 4; l++)
		{
			if (l == _j)
				continue;

			ResultF3X3.m[ValueNum / 3][ValueNum % 3] = M.m[k][l];
			ValueNum++;//相信代码没问题，不用容错退出，错了再说^^
		}
	}

	return ResultF3X3;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM4X4MatrixMinorMatrix(FXMMATRIX M, int _i, int _j)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	XMFLOAT3X3 ResultF3X3 = XM4X4MatrixMinorMatrix(TempStoreF4X4, _i, _j);
	return ResultF3X3;
}

float XM_CALLCONV MyXMFuncLib::XM4X4MatrixCofactor(XMFLOAT4X4 M, int _i, int _j)
{
	//容错
	if (_i < 0 || _i >= 4 || _j < 0 || _j >= 4)
		return 0.0f;
	//C(i,j) = (-1)^(i + j)Det(MinorA(i, j))
	float p = float(pow(-1, _i + _j));
	float Cofactor = p * XM3X3MatrixDeterminant(XM4X4MatrixMinorMatrix(M, _i, _j));
	return Cofactor;
}

float XM_CALLCONV MyXMFuncLib::XM4X4MatrixCofactor(FXMMATRIX M, int _i, int _j)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	float Cofactor = XM4X4MatrixCofactor(TempStoreF4X4, _i, _j);
	return Cofactor;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM3X3MatrixCofactorMatrix(XMFLOAT3X3 M)
{
	XMFLOAT3X3 ResultF3X3;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ResultF3X3.m[i][j] = XM3X3MatrixCofactor(M, i, j);
		}
	}

	return ResultF3X3;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM3X3MatrixCofactorMatrix(FXMMATRIX M)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	XMFLOAT3X3 ResultF3X3 = XM3X3MatrixCofactorMatrix(TempStoreF3X3);
	return ResultF3X3;
}

XMFLOAT4X4 XM_CALLCONV MyXMFuncLib::XM4X4MatrixCofactorMatrix(XMFLOAT4X4 M)
{
	XMFLOAT4X4 ResultF4X4;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ResultF4X4.m[i][j] = XM4X4MatrixCofactor(M, i, j);
		}
	}

	return ResultF4X4;
}

XMFLOAT4X4 XM_CALLCONV MyXMFuncLib::XM4X4MatrixCofactorMatrix(FXMMATRIX M)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	XMFLOAT4X4 ResultF4X4 = XM4X4MatrixCofactorMatrix(TempStoreF4X4);
	return ResultF4X4;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM3X3MatrixAdjointMatrix(XMFLOAT3X3 M)
{
	//先求代数余子式矩阵
	XMFLOAT3X3 CM = XM3X3MatrixCofactorMatrix(M);

	XMFLOAT3X3 ResultStoreF3X3;
	//代数余子式矩阵的转置
	XMStoreFloat3x3(&ResultStoreF3X3, XMMatrixTranspose(XMLoadFloat3x3(&CM)));

	return ResultStoreF3X3;
}

XMFLOAT3X3 XM_CALLCONV MyXMFuncLib::XM3X3MatrixAdjointMatrix(FXMMATRIX M)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	XMFLOAT3X3 ResultF3X3 = XM3X3MatrixAdjointMatrix(TempStoreF3X3);
	return ResultF3X3;
}

XMFLOAT4X4 XM_CALLCONV MyXMFuncLib::XM4X4MatrixAdjointMatrix(XMFLOAT4X4 M)
{
	//先求代数余子式矩阵
	XMFLOAT4X4 CM = XM4X4MatrixCofactorMatrix(M);

	XMFLOAT4X4 ResultStoreF4X4;
	//代数余子式矩阵的转置
	XMStoreFloat4x4(&ResultStoreF4X4, XMMatrixTranspose(XMLoadFloat4x4(&CM)));

	return ResultStoreF4X4;
}

XMFLOAT4X4 XM_CALLCONV MyXMFuncLib::XM4X4MatrixAdjointMatrix(FXMMATRIX M)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	XMFLOAT4X4 ResultF4X4 = XM4X4MatrixAdjointMatrix(TempStoreF4X4);
	return ResultF4X4;
}

XMMATRIX  XM_CALLCONV MyXMFuncLib::XM3X3MatrixInverseMatrix(XMFLOAT3X3 M)
{
	XMMATRIX ResultMatrix = XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	//行列式为0，不可逆
	float DetM = XM3X3MatrixDeterminant(M);
	if (abs(DetM) < 1e-6)
		return ResultMatrix;

	//A^-1 = A*/|A|
	XMFLOAT3X3 AdjointF3X3 = XM3X3MatrixAdjointMatrix(M);
	XMMATRIX AdjointM = XMLoadFloat3x3(&AdjointF3X3);
	ResultMatrix = AdjointM / DetM;

	return ResultMatrix;
}

XMMATRIX  XM_CALLCONV MyXMFuncLib::XM3X3MatrixInverseMatrix(FXMMATRIX M)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	XMMATRIX ResultM3X3 = XM3X3MatrixInverseMatrix(TempStoreF3X3);
	return ResultM3X3;
}

XMMATRIX XM_CALLCONV MyXMFuncLib::XM4X4MatrixInverseMatrix(XMFLOAT4X4 M)
{
	XMMATRIX ResultMatrix = XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	//行列式为0，不可逆
	float DetM = XM4X4MatrixDeterminant(M);
	if (abs(DetM) < 1e-6)
		return ResultMatrix;

	//A^-1 = A*/|A|
	XMFLOAT4X4 AdjointF4X4 = XM4X4MatrixAdjointMatrix(M);
	XMMATRIX AdjointM = XMLoadFloat4x4(&AdjointF4X4);
	ResultMatrix = AdjointM / DetM;

	return ResultMatrix;
}

XMMATRIX XM_CALLCONV MyXMFuncLib::XM4X4MatrixInverseMatrix(FXMMATRIX M)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	XMMATRIX ResultM4X4 = XM4X4MatrixInverseMatrix(TempStoreF4X4);
	return ResultM4X4;
}

float XM_CALLCONV MyXMFuncLib::XM2X2MatrixDeterminant(XMFLOAT2X2 M)
{
	//|M00 M01|	= M00*M11 - M01*M10
	//|M10 M11|
	float DetValue = M.m[0][0] * M.m[1][1] - M.m[0][1] * M.m[1][0];
	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::XM2X2MatrixDeterminant(FXMMATRIX M)
{
	//转成XMFloat2X2
	XMFLOAT2X2 TempStoreF2X2;
	XMStoreFloat2x2(&TempStoreF2X2, M);

	float DetValue = XM2X2MatrixDeterminant(TempStoreF2X2);
	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::XM3X3MatrixDeterminant(XMFLOAT3X3 M)
{
	float DetValue = 0.0f;
	//行列式递推公式：任取一行或一列，等于对 该行/列每一个元素乘以该元素的代数余子式 求和
	for (int i = 0; i < 3; i++)
	{
		//就取第一行来计算
		DetValue += M.m[0][i] * XM3X3MatrixCofactor(M, 0, i);
	}

	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::XM3X3MatrixDeterminant(FXMMATRIX M)
{
	//转成XMFloat3X3
	XMFLOAT3X3 TempStoreF3X3;
	XMStoreFloat3x3(&TempStoreF3X3, M);

	float DetValue = XM3X3MatrixDeterminant(TempStoreF3X3);
	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::XM4X4MatrixDeterminant(XMFLOAT4X4 M)
{
	float DetValue = 0.0f;
	//行列式递推公式：任取一行或一列，等于对 该行/列每一个元素乘以该元素的代数余子式 求和
	for (int i = 0; i < 4; i++)
	{
		//就取第一行来计算
		DetValue += M.m[0][i] * XM4X4MatrixCofactor(M, 0, i);
	}

	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::XM4X4MatrixDeterminant(FXMMATRIX M)
{
	//转成XMFloat4X4
	XMFLOAT4X4 TempStoreF4X4;
	XMStoreFloat4x4(&TempStoreF4X4, M);

	float DetValue = XM4X4MatrixDeterminant(TempStoreF4X4);
	return DetValue;
}

float XM_CALLCONV MyXMFuncLib::GetVecWeight(FXMVECTOR V, int WeightNum)
{
	if (WeightNum <= 0)
		return XMVectorGetX(V);
	else if (WeightNum == 1)
		return XMVectorGetY(V);
	else if (WeightNum == 2)
		return XMVectorGetZ(V);
	else
		return XMVectorGetW(V);
}

XMVECTOR XM_CALLCONV MyXMFuncLib::RowVector(FXMMATRIX M, int RowNum)
{
	//容错
	if (RowNum < 0 || RowNum >= 4)
	{
		return XMVectorZero();
	}

	XMVECTOR ResultVec;
	ResultVec = XMVectorSet(
		XMVectorGetX(M.r[RowNum]),
		XMVectorGetY(M.r[RowNum]),
		XMVectorGetZ(M.r[RowNum]),
		XMVectorGetW(M.r[RowNum])
	);

	return ResultVec;

}

XMVECTOR XM_CALLCONV MyXMFuncLib::ColumnVector(FXMMATRIX M, int ColNum)
{
	//容错
	if (ColNum < 0 || ColNum >= 4)
	{
		return XMVectorZero();
	}
	
	XMVECTOR ResultVec;
	ResultVec = XMVectorSet(
		GetVecWeight(M.r[0], ColNum),
		GetVecWeight(M.r[1], ColNum),
		GetVecWeight(M.r[2], ColNum),
		GetVecWeight(M.r[3], ColNum)
	);

	return ResultVec;
}


XMVECTOR XM_CALLCONV MyXMFuncLib::operator*(FXMVECTOR v, FXMMATRIX m)
{
	XMVECTOR ResultVec;


	ResultVec = XMVectorSet(
		XMVectorGetX(XMVector4Dot(v, ColumnVector(m, 0))),
		XMVectorGetX(XMVector4Dot(v, ColumnVector(m, 1))),
		XMVectorGetX(XMVector4Dot(v, ColumnVector(m, 2))),
		XMVectorGetX(XMVector4Dot(v, ColumnVector(m, 3)))
	);

	return ResultVec;
}

XMVECTOR XM_CALLCONV MyXMFuncLib::operator*(FXMMATRIX m, FXMVECTOR v)
{
	XMVECTOR ResultVec;

	ResultVec = XMVectorSet(
		XMVectorGetX(XMVector4Dot(RowVector(m, 0), v)),
		XMVectorGetX(XMVector4Dot(RowVector(m, 1), v)),
		XMVectorGetX(XMVector4Dot(RowVector(m, 2), v)),
		XMVectorGetX(XMVector4Dot(RowVector(m, 3), v))
	);

	return ResultVec;
}

XMMATRIX XM_CALLCONV MyXMFuncLib::GetRotateMatrix(FXMVECTOR Axis, float Angle, bool bRightHandCoordination)
{
	FXMVECTOR n = XMVector3Normalize(Axis);

	float x = XMVectorGetX(n);
	float y = XMVectorGetY(n);
	float z = XMVectorGetZ(n);

	float c = cos(XMConvertToRadians(Angle));
	float s = sin(XMConvertToRadians(Angle));

	int i = bRightHandCoordination ? 1 : -1;
	//Rn(v) = Rn(v⊥) + v∥ = v⊥*cosα + Cross(n,v)*sinα + v∥
	//其中v∥ = Dot(n,v)*v, v⊥ = v - v∥
	//若在右手系下Cross(V1,V2)=
	//	| i       j       k |
	//	|V1.X   V1.Y   V1.Z |
	//	|V2.X   V2.Y   V2.Z |
	//若在左手系下Cross(V1,V2)=
	//	| i       j       k |
	// -|V1.X   V1.Y   V1.Z |
	//	|V2.X   V2.Y   V2.Z |
	//（叉乘本源定义为满足右手法则时）
	//T(v) = Rn(v) = xRn(i) + yRn(j) + zRn(k)
	//			  |<- Rn(i)->|
	//	= (x,y,z)*|<- Rn(j)->|
	//			  |<- Rn(k)->|
	//故变换矩阵带入可得:
	//	   |c+(1-c)x^2, (1-c)xy + sz, (1-c)xz - sy|
	//Rn = |(1-c)xy - sz, c+(1-c)y^2, (1-c)yz + sx|（右手坐标系）
	//	   |(1-c)xz + sy, (1-c)yz - sx, c+(1-c)z^2|
	//	   |c+(1-c)x^2, (1-c)xy - sz, (1-c)xz + sy|
	//Rn = |(1-c)xy + sz, c+(1-c)y^2, (1-c)yz - sx|（左手坐标系）
	//	   |(1-c)xz - sy, (1-c)yz + sx, c+(1-c)z^2|
	//其中c = cosα, s = sinα

	XMMATRIX ResultRotationMatrix = XMMatrixSet(
		c + (1 - c) * x * x,		 (1 - c) * x * y + i * s * z, (1 - c) * x * z - i * s * y,  0.0f,
		(1 - c) * x * y - i * s * z, c + (1 - c) * y * y,		  (1 - c) * y * z + i * s * x,  0.0f,
		(1 - c) * x * z + i * s * y, (1 - c) * y * z - i * s * x, c + (1 - c) * z * z,			0.0f,
		0.0f,						 0.0f,						  0.0f,							1.0f
	);


	return ResultRotationMatrix;
}





