#include "TestXMMatrix.h"

void TestXMMatrix()
{
	//XMVECTOR n1 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//XMVECTOR n2 = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	//XMVECTOR n3 = XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);
	//XMVECTOR n4 = XMVectorSet(1.0f, 2.0f, 3.0f, 1.0f);

	//XMMATRIX M1(n1, n2, n3, n4);

	//XMMATRIX M2 = XMMatrixIdentity();

	//XMMATRIX M3 = M1 * M2;

	//XMMATRIX TM1 = XMMatrixTranspose(M1);

	//XMVECTOR DetM1 = XMMatrixDeterminant(M1);
	//XMMATRIX InvM1 = XMMatrixInverse(&DetM1, M1);

	////试试自己封的简易接口
	//XMMATRIX MyInvM1 = XM4X4MatrixInverseMatrix(M1);

	//XMVECTOR V1 = XMVectorSet(1, 2, 3, 0);
	//const float MMultifloats[16] = { 1,2,3,0,1,2,4,0,1,3,9,0,0,0,0,0 };
	//XMMATRIX MMulti(MMultifloats);

	//XMVECTOR VMulti1 = V1 * MMulti;
	//XMVECTOR VMulti2 = MMulti * V1;

	//cout << "V1 = " << V1 << endl;
	//cout << "MMulti = " << endl << MMulti << endl;
	//cout << "VMulti1 = " << VMulti1 << endl;
	//cout << "VMulti2 = " << VMulti2 << endl;


	//cout << "M1 = " << endl << M1 << endl;
	//cout << "M2 = " << endl << M2 << endl;
	//cout << "M1 * M2 = " << endl << M3 << endl;
	//cout << "TM1 = " << endl << TM1 << endl;
	//cout << "DetM1 = " << endl << DetM1 << endl;
	//cout << "InvM1 = " << endl << InvM1 << endl;
	//cout << "MyInvM1 = " << endl << MyInvM1 << endl;

	//const float Testfloats[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15 };
	//XMMATRIX MTestfloatsInit(Testfloats);

	//cout << "MTestfloatsInit = " << endl << MTestfloatsInit << endl;

	//下面测试矩阵变换函数
	XMVECTOR v1 = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR v2 = XMVectorSet(1.0f, 2.0f, 4.0f, 0.0f);


	XMMATRIX ScaleM = XMMatrixScaling(2.0f, 3.0f, 4.0f);
	XMMATRIX RotateM = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMConvertToRadians(45.0f));
	XMMATRIX MyRotateM = GetRotateMatrix(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), 45.0f, true);


	XMVECTOR v3 = XMVector3TransformCoord(v1, ScaleM);
	XMVECTOR v4 = XMVector3Transform(v1, ScaleM);
	XMVECTOR v5 = XMVector3TransformNormal(v1, ScaleM);
	XMVECTOR Myv3 = v1*ScaleM;

	XMVECTOR v6 = XMVector3Transform(v1, RotateM);
	XMVECTOR Myv6 = XMVector3Transform(v1, MyRotateM);

	cout << "v3 = " << v3 << endl;
	cout << "v4 = " << v4 << endl;
	cout << "v5 = " << v5 << endl;
	cout << "Myv3 = " << Myv3 << endl;
	cout << "v6 = " << v6 << endl;
	cout << "Myv6 = " << Myv6 << endl;

}
