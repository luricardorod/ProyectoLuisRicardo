#include "CMatrix4D.h"
#include <cmath>
#include <string.h>
#define MANODER

CMatrix4D::CMatrix4D()
{
}
CMatrix4D::CMatrix4D(const CMatrix4D& A)
{
	*this = A;
}
CMatrix4D::CMatrix4D(
	float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	m00 = a00; m01 = a01; m02 = a02; m03 = a03;
	m10 = a10; m11 = a11; m12 = a12; m13 = a13;
	m20 = a20; m21 = a21; m22 = a22; m23 = a23;
	m30 = a30; m31 = a31; m32 = a32; m33 = a33;
}
CMatrix4D::CMatrix4D(
	CVector4D &row0,
	CVector4D &row1,
	CVector4D &row2,
	CVector4D &row3)
{
	*(CVector4D*)&m00 = row0;
	*(CVector4D*)&m10 = row1;
	*(CVector4D*)&m20 = row2;
	*(CVector4D*)&m30 = row3;
}
CMatrix4D::~CMatrix4D()
{
}
CMatrix4D operator*(CMatrix4D& A, CMatrix4D& B)
{
	CMatrix4D R = Zero();
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < 4; k++)
				R.m[j][i] += A.m[j][k] * B.m[k][i];
	return R;
}
CVector4D operator*(CVector4D& V, CMatrix4D& M)
{
	CVector4D R(0, 0, 0, 0);
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			R.v[j] += V.v[i] * M.m[i][j];
	return R;

}
CVector4D operator*(CMatrix4D& M, CVector4D& V)
{
	return CVector4D(
		Dot(*(CVector4D*)&M.m00, V),
		Dot(*(CVector4D*)&M.m10, V),
		Dot(*(CVector4D*)&M.m20, V),
		Dot(*(CVector4D*)&M.m30, V)
	);
}
CMatrix4D Zero()
{
	CMatrix4D Z;
	memset(&Z, 0, sizeof(CMatrix4D));
	return Z;
}
CMatrix4D Identity()
{
	CMatrix4D I = Zero();
	for (int i = 0; i < 4; i++) I.m[i][i] = 1.0f;
	return I;
}

CMatrix4D Transpose(CMatrix4D& M) {
	CMatrix4D T;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) T.m[j][i] = M.m[i][j];
	return T;
}

CMatrix4D Scaling(float sx, float sy, float sz)
{
	CMatrix4D S = Identity();
	S.m00 = sx;
	S.m11 = sy;
	S.m22 = sz;
	return S;
}
#ifdef MANODER
CMatrix4D Translation(float dx, float dy, float dz)
{
	CMatrix4D T = Identity();
	T.m03 = dx;
	T.m13 = dy;
	T.m23 = dz;
	return T;
}
CMatrix4D RotationX(float theta)
{
	CMatrix4D R = Identity();
	R.m11 = std::cos(theta);
	R.m22 = R.m11;
	R.m21 = std::sin(theta);
	R.m12 = -R.m21;
	return R;
}
CMatrix4D RotationY(float theta)
{
	CMatrix4D R = Identity();
	R.m00 = std::cos(theta);
	R.m22 = R.m00;
	R.m02 = std::sin(theta);
	R.m20 = -R.m02;
	return R;
}
CMatrix4D RotationZ(float theta)
{
	CMatrix4D R = Identity();
	R.m00 = std::cos(theta);
	R.m11 = R.m00;
	R.m10 = std::sin(theta);
	R.m01 = -R.m10;
	return R;
}
#else
CMatrix4D Translation(float dx, float dy, float dz)
{
	CMatrix4D T = Identity();
	T.m30 = dx;
	T.m31 = dy;
	T.m32 = dz;
	return T;
}
CMatrix4D RotationX(float theta)
{
	CMatrix4D R = Identity();
	R.m11 = std::cos(theta);
	R.m22 = R.m11;
	R.m12 = std::sin(theta);
	R.m21 = -R.m12;
	return R;
}
CMatrix4D RotationY(float theta)
{
	CMatrix4D R = Identity();
	R.m00 = std::cos(theta);
	R.m22 = R.m00;
	R.m20 = std::sin(theta);
	R.m02 = -R.m20;
	return R;
}
CMatrix4D RotationZ(float theta)
{
	CMatrix4D R = Identity();
	R.m00 = std::cos(theta);
	R.m11 = R.m00;
	R.m01 = std::sin(theta);
	R.m10 = -R.m01;
	return R;
}
#endif
