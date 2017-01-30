#ifndef CVECTOR4D_H
#define CVECTOR4D_H

class CVector4D
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float r, g, b, a;
		};
		struct
		{
			unsigned long ulx, uly, ulz, ulw;
		};
		struct
		{
			long lx, ly, lz, lw;
		};
		float v[4];
		unsigned long ul[4];
		long l[4];
	};
	CVector4D();
	CVector4D(const CVector4D& V);
	CVector4D(const float fx, const  float fy, const  float fz, const float fw);
	~CVector4D();
};
CVector4D operator* (const CVector4D  &A, const CVector4D &B);
CVector4D operator* (float s, const CVector4D &A);
CVector4D operator* (const CVector4D &A, float s);
CVector4D operator/ (const CVector4D &A, float s);
CVector4D operator+(const CVector4D& A, const CVector4D& B);
CVector4D operator-(const CVector4D& A, const CVector4D& B);
float Dot(const CVector4D &A, const CVector4D &B);
CVector4D Cross3(const CVector4D& A, const CVector4D& B);
float Magnity(const CVector4D& A);
CVector4D Normalize(const CVector4D& A);

#endif

