#ifndef UAD_COBJECT3D_GL_H
#define UAD_COBJECT3D_GL_H

#include "Config.h"
#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureGL.h"
#elif defined(USING_D3D11)
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif

#include "CMatrix4D.h"
#include "PrimitiveBase.h"
#include "UtilsGL.h"
#include <vector>

class CObject3D : public PrimitiveBase {
public:
	std::vector<CVertex> bufferVertex;
	std::vector<std::vector<unsigned short>*> bufferIndexForText;
	CMatrix4D	transform;
	int		TexId[32];

	void Create(char * path);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();
#ifdef USING_OPENGL_ES
	CObject3D() : shaderID(0) {}
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;
	GLint	diffuseLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	GLuint			VB;
	GLuint			IB[20];

	GLint DirectionGlobalLight;
	GLint ColorGlobalLight;
	GLint PositionPointLight;
	GLint ColorPointLight;
	GLint PosCamera;

#elif defined(USING_D3D11)
	struct CBuffer
	{
		CMatrix4D WVP;
		CMatrix4D World;
	};
	CObject3D() {}
	ComPtr<ID3D11Buffer>		IB[20];
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;

	CObject3D::CBuffer				CnstBuffer;
	Texture *Textures[20];

#endif
};


#endif