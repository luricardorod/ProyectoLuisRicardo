
#include "PrimitiveManager.h"
#include "TriangleGL.h"
#include "CubeGL.h"
#include "CObject3D.h"

PrimitiveBase*	PrimitiveManager::GetPrimitive(unsigned int index) {
	if (index >= primitives.size())
		return 0;

	return primitives[index];
}

int  PrimitiveManager::CreateTriangle() {
	PrimitiveBase *primitive = new Trangle();
	primitive->Create("NULL");
	primitives.push_back(primitive);
	return (int)(primitives.size()-1);
}

int	 PrimitiveManager::CreateCube(){
	PrimitiveBase *primitive = new CubeGL();
	primitive->Create("NULL");
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}


int PrimitiveManager::CreateObject3D(char * path)
{
	PrimitiveBase *primitive = new CObject3D();
	primitive->Create(path);
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}

void PrimitiveManager::DrawPrimitives() {
	for(unsigned int i=0;i<primitives.size();i++){
		primitives[i]->Draw(0,&(*pVP).m[0][0]);
	}
}

void PrimitiveManager::DestroyPrimitives() {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->Destroy();
		delete primitives[i];
	}
	primitives.clear();
}