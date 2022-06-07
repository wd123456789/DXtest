#include"TestXMVector.h"

void TestXMVector()
{
	XMVECTOR a = XMVectorSet(1.0f, 0.0f, 0.0f, 4.0f);
	XMVECTOR b = XMVectorSet(0.0f, 1.0f, 0.0f, 3.0f);
	XMVECTOR c = XMVectorSet(4.0f, 3.0f, 1.0f, 2.0f);
	XMVECTOR d = XMVectorSet(3.0f, 0.1f, -1.0f, -2.0f);

	XMVECTOR e = a * b;
	XMVECTOR f = a / b;

	XMVECTOR Lengtha = XMVector3Length(a);

	XMVECTOR g = XMVector3Normalize(a);

	XMVECTOR h = XMVector3Dot(a, b);

	XMVECTOR i = XMVector3Cross(a, b);


	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "c = " << c << endl;
	cout << "d = " << d << endl;

	cout << "a * b = " << e << endl;
	cout << "a / b = " << f << endl;
	cout << "Lengtha = " << Lengtha << endl;

	cout << "Norm a = " << g << endl;

	cout << "a dot b = " << h << endl;

	cout << "a cross b = " << i << endl;

	//返回a的第0/1/2/3位，参数不能超过3
	cout << "XMVectorSwizzle(a, 1, 2, 3, 3) = "
		<< XMVectorSwizzle(a, 1, 2, 3, 3) << endl;

	//将矢量的每个分量饱和到 0.0f 到 1.0f 的范围。
	cout << "XMVectorSaturate(d) = " << XMVectorSaturate(d) << endl;

	//用a的Z分量来初始化向量w
	XMVECTOR w = XMVectorSplatZ(a);
	cout << "w = " << w << endl;

	//测试一组向量的施密特正交化的DX写法
	XMVECTOR v1 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR v2 = XMVectorSet(1.0f, 5.0f, 0.0f, 0.0f);
	XMVECTOR v3 = XMVectorSet(2.0f, 1.0f, -4.0f, 0.0f);

	//正交
	//w1 = v1
	//wi = vi - ∑(j = 1 : i-1)(Proj(vi,wj))
	XMVECTOR w1 = v1;
	XMVECTOR w2 = v2 - XMVector3ProjectionNV(v2, w1);
	XMVECTOR w3 = v3 - XMVector3ProjectionNV(v3, w1) - XMVector3ProjectionNV(v3, w2);

	//规范化
	XMVECTOR n1 = XMVector3Normalize(w1);
	XMVECTOR n2 = XMVector3Normalize(w2);
	XMVECTOR n3 = XMVector3Normalize(w3);

	cout << "w1 = " << w1 << endl;
	cout << "w2 = " << w2 << endl;
	cout << "w3 = " << w3 << endl;

	cout << "n1 = " << n1 << endl;
	cout << "n2 = " << n2 << endl;
	cout << "n3 = " << n3 << endl;
}