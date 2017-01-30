#include "CVector4D.h"
#include <cmath>
CVector4D::CVector4D()
{
}

CVector4D::CVector4D(const CVector4D& V)
{
	x = V.x;
	y = V.y;
	z = V.z;
	w = V.w;
}
CVector4D::CVector4D(const float fx, const float fy, const float fz, const float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

CVector4D::~CVector4D()
{
}
CVector4D operator* (const CVector4D  &A, const CVector4D &B)
{
	return CVector4D(A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w);
}
CVector4D operator* (float s, const CVector4D &A)
{
	return CVector4D(A.x*s, A.y*s, A.z*s, A.w*s);
}
CVector4D operator* (const CVector4D &A, float s)
{
	return s*A;
}
CVector4D operator/ (const CVector4D &A, float s)
{
	return A*(1 / s);
}
CVector4D operator+(const CVector4D& A, const CVector4D& B)
{
	return CVector4D(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w);
}
CVector4D operator-(const CVector4D& A, const CVector4D& B)
{
	return CVector4D(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w);

}
float Dot(const CVector4D &A, const CVector4D &B)
{
	return A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
}
CVector4D Cross3(const CVector4D& A, const CVector4D& B)
{
	return CVector4D(
		A.y*B.z - A.z*B.y,
		A.z*B.x - A.x*B.z,
		A.x*B.y - A.y*B.x, 0);
}
float Magnity(const CVector4D& A)
{
	return std::sqrt(Dot(A, A));
}
CVector4D Normalize(const CVector4D& A)
{
	return A / Magnity(A);
}
