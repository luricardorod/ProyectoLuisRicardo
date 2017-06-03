#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H
#include "CVector4D.h"
#include "Texture.h"
struct Lights
{
	CVector4D dirGlobal;
	CVector4D colorGlobal;
	CVector4D posPoint;
	CVector4D colorPoint;
	CVector4D *posCamera;
	bool flagWireFrame;
};
class PrimitiveBase {
public:
	virtual void Create(char *path) = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t,float *vp) = 0;
	virtual void Destroy() = 0;
	Lights* lights;
	Texture*				 Textures[8];
	void SetTexture(Texture* tex, int index) {
		Textures[index] = tex;
	}
};

struct CVertex {

	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
	float bix, biy, biz, biw;
	float tanx, tany, tanz, tanw;
};

#endif