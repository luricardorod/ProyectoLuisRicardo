#ifndef UAD_COBJECT3D_GL_H
#define UAD_COBJECT3D_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "CMatrix4D.h"

#include "PrimitiveBase.h"
#include "UtilsGL.h"

class CObject3D : public PrimitiveBase {
public:
	CObject3D() : shaderID(0) {}
	void Create(char * path);

	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	CVertex*		vertices;
	unsigned short*	indices;
	GLuint			VB;
	GLuint			IB;
	int sizeIndex;

	CMatrix4D	transform;
};


#endif