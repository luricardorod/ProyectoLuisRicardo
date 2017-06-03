#ifndef UAD_QUAD
#define UAD_QUAD
#include "Config.h"
#ifdef USING_GL_COMMON
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureGL.h"
#elif defined(USING_D3D11)
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif

#include "UtilsGL.h"
#include "PrimitiveBase.h"
#include "CMatrix4D.h"
class CQuad : public PrimitiveBase
{
public:
	struct quadVertex {
		float x, y, z, w;
		float s, t;
	};

	void Create(char *path);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	quadVertex	vertices[4];
	unsigned short	indices[6];
	CMatrix4D	transform;
	~CQuad();

#ifdef USING_GL_COMMON
	CQuad() : shaderID(0) {}
	GLuint	shaderID;
	GLuint	vertexAttribLoc;
	GLuint	uvAttribLoc;
	GLint	diffuseLoc;
	GLint	offsetX;
	GLint	offsetY;
	GLint   numFrameX;
	GLint   numFrameY;
	GLint   matTransform;
	int idTexture;

	GLuint			VB;
	GLuint			IB;
#elif defined(USING_D3D11)
	struct CQuadBuffer
	{
		CMatrix4D matTransform;
	};
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	CQuad::CQuadBuffer	CnstBuffer;
	Texture* texture;

	CQuad() {};
#endif
};


#endif

