#ifndef UAD_SPRITE
#define UAD_SPRITE

#include "Config.h"
#ifdef USING_OPENGL_ES
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
#include "Timer.h"
//#define USE_ARRAY_OF_STRUCTS

struct spriteVertex {
	float x, y, z;
	float s, t;
};

struct animation {
	int ix, iy;
	float ftime;
	int iframes;
	float initTime;
};

class Sprite : public PrimitiveBase {
public:
	Timer			DtTimer;

	void Create(char *path);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();
	void Init(float width, float height, float positionX, float positionY, float numberFrames, float uvWidth, float uvHeight);
	void setAnimation(int x,int y, float time, int frames);
	void updateAnimation();

	animation unica;

	float fwidth;
	float fheight;
	float fpositionX;
	float fpositionY;
	unsigned short inumberFrames;
	float fuvWidth;
	float fuvHeight;
	float uvx, uvy;
	int   numberFrameX = 0;
	int   numberFrameY = 8;

	spriteVertex	vertices[4];
	unsigned short	indices[6];
	CMatrix4D	transform;
	~Sprite();
#ifdef USING_OPENGL_ES
	Sprite() : shaderID(0) {}
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
	struct CSpriteBuffer
	{
		CMatrix4D matTransform;
		float offsetX;
		float offsetY;
		int numFrameX;
		int numFrameY;
	};
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	Sprite::CSpriteBuffer	CnstBuffer;
	Texture* texture;

	Sprite() {};
#endif
};

#endif
