#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H
#include "CVector4D.h"
struct Lights
{
	CVector4D dirGlobal;
	CVector4D colorGlobal;
	CVector4D posPoint;
	CVector4D colorPoint;
	CVector4D *posCamera;
};
class PrimitiveBase {
public:
	virtual void Create(char *path) = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t,float *vp) = 0;
	virtual void Destroy() = 0;
	Lights* lights;
	
};

struct CVertex {

	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
	float bix, biy, biz;
	float tanx, tany, tanz;
};

#endif