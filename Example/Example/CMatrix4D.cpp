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
CMatrix4D::CMatrix4D(float * vp)
{
	m00 = (*vp);
	m01 = (*(vp+1));
	m02 = (*(vp + 2));
	m03 = (*(vp + 3));

	m10 = (*(vp + 4));
	m11 = (*(vp + 5));
	m12 = (*(vp + 6));
	m13 = (*(vp + 7));

	m20 = (*(vp + 8));
	m21 = (*(vp + 9));
	m22 = (*(vp + 10));
	m23 = (*(vp + 11));

	m30 = (*(vp + 12));
	m31 = (*(vp + 13));
	m32 = (*(vp + 14));
	m33 = (*(vp + 15));
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
CMatrix4D LookAtRH(CVector4D &EyePos, CVector4D &Target, CVector4D &Up)
{
	CVector4D zaxis = Normalize(EyePos - Target);
	CVector4D xaxis = Normalize(Cross3(Up, zaxis));
	CVector4D yaxis = Cross3(zaxis, xaxis);

	CMatrix4D Result;
	Result.m[0][0] = xaxis.x;	Result.m[0][1] = yaxis.x;	Result.m[0][2] = zaxis.x;	Result.m[0][3] = 0;
	Result.m[1][0] = xaxis.y;	Result.m[1][1] = yaxis.y;	Result.m[1][2] = zaxis.y;	Result.m[1][3] = 0;
	Result.m[2][0] = xaxis.z;	Result.m[2][1] = yaxis.z;	Result.m[2][2] = zaxis.z;	Result.m[2][3] = 0;
	Result.m[3][0] = Dot(xaxis, EyePos); Result.m[3][1] = Dot(yaxis, EyePos); Result.m[3][2] = Dot(zaxis, EyePos); Result.m[3][3] = 1;

	return Result;
}
CMatrix4D PerspectiveFovRH(float fovY, float Aspect, float zn, float zf)
{
	float yScale = 1 / tanf(fovY / 2);
	float xScale = yScale / Aspect;
	CMatrix4D result = Identity();
	result.m00 = xScale;
	result.m11 = yScale;
	result.m22 = zf / (zn - zf);
	result.m23 = -1;
	result.m32 = zn*zf / (zn - zf);
	result.m33 = 0;

	return result;
}
#ifdef MANODER
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
CMatrix4D CMatrix4D::Inverse()
{
	CMatrix4D tmp64 = *this;
	float det =
		tmp64.m00*tmp64.m11*tmp64.m22*tmp64.m33 + tmp64.m00*tmp64.m12*tmp64.m23*tmp64.m31 + tmp64.m00*tmp64.m13*tmp64.m21*tmp64.m32
		+ tmp64.m01*tmp64.m10*tmp64.m23*tmp64.m32 + tmp64.m01*tmp64.m12*tmp64.m20*tmp64.m33 + tmp64.m01*tmp64.m13*tmp64.m22*tmp64.m30
		+ tmp64.m02*tmp64.m10*tmp64.m21*tmp64.m33 + tmp64.m02*tmp64.m11*tmp64.m23*tmp64.m30 + tmp64.m02*tmp64.m13*tmp64.m20*tmp64.m31
		+ tmp64.m03*tmp64.m10*tmp64.m22*tmp64.m31 + tmp64.m03*tmp64.m11*tmp64.m20*tmp64.m32 + tmp64.m03*tmp64.m12*tmp64.m21*tmp64.m30
		- tmp64.m00*tmp64.m11*tmp64.m23*tmp64.m32 - tmp64.m00*tmp64.m12*tmp64.m21*tmp64.m33 - tmp64.m00*tmp64.m13*tmp64.m22*tmp64.m31
		- tmp64.m01*tmp64.m10*tmp64.m22*tmp64.m33 - tmp64.m01*tmp64.m12*tmp64.m23*tmp64.m30 - tmp64.m01*tmp64.m13*tmp64.m20*tmp64.m32
		- tmp64.m02*tmp64.m10*tmp64.m23*tmp64.m31 - tmp64.m02*tmp64.m11*tmp64.m20*tmp64.m33 - tmp64.m02*tmp64.m13*tmp64.m21*tmp64.m30
		- tmp64.m03*tmp64.m10*tmp64.m21*tmp64.m32 - tmp64.m03*tmp64.m11*tmp64.m22*tmp64.m30 - tmp64.m03*tmp64.m12*tmp64.m20*tmp64.m31;

	if (det == 0.0f)
		exit(666);

	det = 1.0f / det;
	CMatrix4D out;

	out.m00 = tmp64.m11*tmp64.m22*tmp64.m33 + tmp64.m12*tmp64.m23*tmp64.m31 + tmp64.m13*tmp64.m21*tmp64.m32 - tmp64.m11*tmp64.m23*tmp64.m32 - tmp64.m12*tmp64.m21*tmp64.m33 - tmp64.m13*tmp64.m22*tmp64.m31;
	out.m01 = tmp64.m01*tmp64.m23*tmp64.m32 + tmp64.m02*tmp64.m21*tmp64.m33 + tmp64.m03*tmp64.m22*tmp64.m31 - tmp64.m01*tmp64.m22*tmp64.m33 - tmp64.m02*tmp64.m23*tmp64.m31 - tmp64.m03*tmp64.m21*tmp64.m32;
	out.m02 = tmp64.m01*tmp64.m12*tmp64.m33 + tmp64.m02*tmp64.m13*tmp64.m31 + tmp64.m03*tmp64.m11*tmp64.m32 - tmp64.m01*tmp64.m13*tmp64.m32 - tmp64.m02*tmp64.m11*tmp64.m33 - tmp64.m03*tmp64.m12*tmp64.m31;
	out.m03 = tmp64.m01*tmp64.m13*tmp64.m22 + tmp64.m02*tmp64.m11*tmp64.m23 + tmp64.m03*tmp64.m12*tmp64.m21 - tmp64.m01*tmp64.m12*tmp64.m23 - tmp64.m02*tmp64.m13*tmp64.m21 - tmp64.m03*tmp64.m11*tmp64.m22;
	out.m10 = tmp64.m10*tmp64.m23*tmp64.m32 + tmp64.m12*tmp64.m20*tmp64.m33 + tmp64.m13*tmp64.m22*tmp64.m30 - tmp64.m10*tmp64.m22*tmp64.m33 - tmp64.m12*tmp64.m23*tmp64.m30 - tmp64.m13*tmp64.m20*tmp64.m32;
	out.m11 = tmp64.m00*tmp64.m22*tmp64.m33 + tmp64.m02*tmp64.m23*tmp64.m30 + tmp64.m03*tmp64.m20*tmp64.m32 - tmp64.m00*tmp64.m23*tmp64.m32 - tmp64.m02*tmp64.m20*tmp64.m33 - tmp64.m03*tmp64.m22*tmp64.m30;
	out.m12 = tmp64.m00*tmp64.m13*tmp64.m32 + tmp64.m02*tmp64.m10*tmp64.m33 + tmp64.m03*tmp64.m12*tmp64.m30 - tmp64.m00*tmp64.m12*tmp64.m33 - tmp64.m02*tmp64.m13*tmp64.m30 - tmp64.m03*tmp64.m10*tmp64.m32;
	out.m13 = tmp64.m00*tmp64.m12*tmp64.m23 + tmp64.m02*tmp64.m13*tmp64.m20 + tmp64.m03*tmp64.m10*tmp64.m22 - tmp64.m00*tmp64.m13*tmp64.m22 - tmp64.m02*tmp64.m10*tmp64.m23 - tmp64.m03*tmp64.m12*tmp64.m20;
	out.m20 = tmp64.m10*tmp64.m21*tmp64.m33 + tmp64.m11*tmp64.m23*tmp64.m30 + tmp64.m13*tmp64.m20*tmp64.m31 - tmp64.m10*tmp64.m23*tmp64.m31 - tmp64.m11*tmp64.m20*tmp64.m33 - tmp64.m13*tmp64.m21*tmp64.m30;
	out.m21 = tmp64.m00*tmp64.m23*tmp64.m31 + tmp64.m01*tmp64.m20*tmp64.m33 + tmp64.m03*tmp64.m21*tmp64.m30 - tmp64.m00*tmp64.m21*tmp64.m33 - tmp64.m01*tmp64.m23*tmp64.m30 - tmp64.m03*tmp64.m20*tmp64.m31;
	out.m22 = tmp64.m00*tmp64.m11*tmp64.m33 + tmp64.m01*tmp64.m13*tmp64.m30 + tmp64.m03*tmp64.m10*tmp64.m31 - tmp64.m00*tmp64.m13*tmp64.m31 - tmp64.m01*tmp64.m10*tmp64.m33 - tmp64.m03*tmp64.m11*tmp64.m30;
	out.m23 = tmp64.m00*tmp64.m13*tmp64.m21 + tmp64.m01*tmp64.m10*tmp64.m23 + tmp64.m03*tmp64.m11*tmp64.m20 - tmp64.m00*tmp64.m11*tmp64.m23 - tmp64.m01*tmp64.m13*tmp64.m20 - tmp64.m03*tmp64.m10*tmp64.m21;
	out.m30 = tmp64.m10*tmp64.m22*tmp64.m31 + tmp64.m11*tmp64.m20*tmp64.m32 + tmp64.m12*tmp64.m21*tmp64.m30 - tmp64.m10*tmp64.m21*tmp64.m32 - tmp64.m11*tmp64.m22*tmp64.m30 - tmp64.m12*tmp64.m20*tmp64.m31;
	out.m31 = tmp64.m00*tmp64.m21*tmp64.m32 + tmp64.m01*tmp64.m22*tmp64.m30 + tmp64.m02*tmp64.m20*tmp64.m31 - tmp64.m00*tmp64.m22*tmp64.m31 - tmp64.m01*tmp64.m20*tmp64.m32 - tmp64.m02*tmp64.m21*tmp64.m30;
	out.m32 = tmp64.m00*tmp64.m12*tmp64.m31 + tmp64.m01*tmp64.m10*tmp64.m32 + tmp64.m02*tmp64.m11*tmp64.m30 - tmp64.m00*tmp64.m11*tmp64.m32 - tmp64.m01*tmp64.m12*tmp64.m30 - tmp64.m02*tmp64.m10*tmp64.m31;
	out.m33 = tmp64.m00*tmp64.m11*tmp64.m22 + tmp64.m01*tmp64.m12*tmp64.m20 + tmp64.m02*tmp64.m10*tmp64.m21 - tmp64.m00*tmp64.m12*tmp64.m21 - tmp64.m01*tmp64.m10*tmp64.m22 - tmp64.m02*tmp64.m11*tmp64.m20;

	CMatrix4D tmpO;

	for (int i = 0; i < 16; i++)
		tmpO.v[i] = det*out.v[i];

	return tmpO;
}