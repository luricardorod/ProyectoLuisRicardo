#include "PrimitiveInstance.h"

void PrimitiveInst::TranslateAbsolute(float x, float y, float z){
	position = Translation(x, y, z);
}

void PrimitiveInst::RotateXAbsolute(float ang) {
	rotationX = RotationX(ang*3.1416 / 180);
}

void PrimitiveInst::RotateYAbsolute(float ang) {
	rotationY = RotationY(ang*3.1416 / 180);
}

void PrimitiveInst::RotateZAbsolute(float ang) {
	rotationZ = RotationZ(ang*3.1416 / 180);
}

void PrimitiveInst::ScaleAbsolute(float sc) {
	scale = Scaling(sc, sc, sc);
}

void PrimitiveInst::TranslateRelative(float x, float y, float z) {
	CMatrix4D tmp;
	tmp = Translation(x, y, z);
	position = position * tmp;
}

void PrimitiveInst::RotateXRelative(float ang) {
	CMatrix4D tmp;
	tmp = RotationX(ang*3.1416 / 180);
	rotationX = rotationX * tmp;
}

void PrimitiveInst::RotateYRelative(float ang) {
	CMatrix4D tmp;
	tmp = RotationY(ang*3.1416 / 180);
	rotationY = rotationY * tmp;
}

void PrimitiveInst::RotateZRelative(float ang) {
	CMatrix4D tmp;
	tmp = RotationZ(ang*3.1416 / 180);
	rotationZ = rotationZ * tmp;
}

void PrimitiveInst::ScaleRelative(float sc) {
	CMatrix4D tmp;
	tmp = Scaling(sc, sc, sc);
	scale = scale * tmp;
}

void PrimitiveInst::Update() {
	final = scale*rotationX*rotationY*rotationZ*position;
}

void PrimitiveInst::Draw()
{
	pBase->dirGlobal = dirGlobal;
	pBase->colorGlobal = colorGlobal;
	pBase->posPoint = posPoint;
	pBase->colorPoint = colorPoint;
	pBase->Draw(&final.m[0][0],&(*pViewProj).m[0][0]);
}
