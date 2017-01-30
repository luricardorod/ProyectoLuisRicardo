#ifndef CMATRIX4D_H
#define CMATRIX4D_H

#include "CVector4D.h"
class CMatrix4D
{
public:
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		float m[4][4];
		float v[16];
	};
public:
	CMatrix4D();
	CMatrix4D(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);
	CMatrix4D(const CMatrix4D& M);
	CMatrix4D(CVector4D &row0,
		CVector4D &row1,
		CVector4D &row2,
		CVector4D &row3);
	~CMatrix4D();
};

CMatrix4D operator*(CMatrix4D& A, CMatrix4D& B);
CVector4D operator*(CVector4D& V, CMatrix4D& M);
CVector4D operator*(CMatrix4D& M, CVector4D& V);
CMatrix4D Zero();
CMatrix4D Identity();
CMatrix4D Transpose(CMatrix4D& M);

CMatrix4D Translation(float dx, float dy, float dz);
CMatrix4D RotationX(float theta);
CMatrix4D RotationY(float theta);
CMatrix4D RotationZ(float theta);
CMatrix4D Scaling(float sx, float sy, float sz);

#endif

