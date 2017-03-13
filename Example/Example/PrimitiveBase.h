#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif

#include "CVector4D.h"

class PrimitiveBase
{
public:
	virtual void Create(char *path) = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t, float *vp) = 0;
	virtual void Destroy() = 0;
	CVector4D *lightPosBase;
};

struct CVertex
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
};

#endif