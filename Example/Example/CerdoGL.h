#ifndef UAD_CERDO_GL_H
#define UAD_CERDO_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <d3dx9math.h>

#include "PrimitiveBase.h"
#include "UtilsGL.h"

class CerdoGL : public PrimitiveBase {
public:
	CerdoGL() : shaderID(0) {}
	void Create();
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

	D3DXMATRIX	transform;
};


#endif