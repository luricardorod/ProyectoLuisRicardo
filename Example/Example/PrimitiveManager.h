#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include "PrimitiveBase.h"
#include "CMatrix4D.h"

class PrimitiveManager {
public:
	void SetVP(CMatrix4D *vp) {
		pVP = vp;
	}
	int CreateQuad();
	int  CreateTriangle();
	int	 CreateCube();
	int  CreateObject3D(char * path);
	int	 CreateSprite(char * path, float width, float height, float positionX, float positionY, float numberFrames, float uvWidth, float uvHeight);
	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	CMatrix4D *pVP;
};

#endif