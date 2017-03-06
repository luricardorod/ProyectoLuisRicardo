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
#include <map>

class CObject3D : public PrimitiveBase {
public:
#ifdef USING_D3D11
	struct CBuffer {
		CMatrix4D WVP;
		CMatrix4D World;
	};
#endif
	void Create(char * path);

	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();
	CMatrix4D	transform;
	CVertex*		vertices;
	unsigned short*	indices;
	int sizeIndex;
#ifdef USING_OPENGL_ES
	CObject3D() : shaderID(0) {}
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;
	GLint	diffuseLoc;
	int		TexId[32];

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	GLuint			VB;
	GLuint			IB[20];
	std::vector<unsigned short> bufferIndex[20];
	std::map <std::string, unsigned short> list;
#elif defined(USING_D3D11)
	CObject3D() {}
	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;

	CObject3D::CBuffer				CnstBuffer;

#endif
};


#endif