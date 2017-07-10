#ifndef UAD_PRIMITIVE_INSTANCE_H
#define UAD_PRIMITIVE_INSTANCE_H

#include "PrimitiveBase.h"
#include "CMatrix4D.h"


class PrimitiveInst {
	public:
		void	CreateInstance(PrimitiveBase *pPrim, CMatrix4D *pVP) {
			pBase = pPrim;
			pViewProj = pVP;
			position = Identity();
			scale = Identity();
			rotationX = Identity();
			rotationY = Identity();
			rotationZ = Identity();
			final = Identity();
			flagLigths = true;
		}

		void	TranslateAbsolute(float x, float y, float z);
		void	RotateXAbsolute(float ang);
		void	RotateYAbsolute(float ang);
		void	RotateZAbsolute(float ang);
		void	ScaleAbsolute(float sc);

		void	TranslateRelative(float x, float y, float z);
		void	RotateXRelative(float ang);
		void	RotateYRelative(float ang);
		void	RotateZRelative(float ang);
		void	ScaleRelative(float sc);

		void	SetSignature(unsigned int f);
		void	SetTexture(Texture* tex, int index);

		void	Update();
		void	Draw();
		bool flagLigths;

		CMatrix4D		position;
		CMatrix4D		scale;
		CMatrix4D		rotationX;
		CMatrix4D		rotationY;
		CMatrix4D		rotationZ;
		CMatrix4D		final;

		CMatrix4D		*pViewProj;
		Lights			*lights;
		PrimitiveBase	*pBase;
};


#endif