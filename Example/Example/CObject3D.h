#ifndef UAD_COBJECT3D_GL_H
#define UAD_COBJECT3D_GL_H

#include "Config.h"
#ifdef USING_GL_COMMON
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

struct textureByMesh
{
	bool specular;
	bool gloss;
	bool normal;
	char* specularName;
	char* glossName;
	char* normalName;
	char *diffuseName;
	int  idSpecular;
	int  idNormal;
	int  idDiffuse;
	int	 idGloss;
};

struct infotex {
public:
	unsigned short numberOfTextures;
	std::vector<unsigned short> indicesTextures;
	std::vector<textureByMesh> textures;
};

class CObject3D : public PrimitiveBase {
	
public:

	CMatrix4D	transform;

	void Create(char * path);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();
	bool viewWareFrame = true;
#ifdef USING_GL_COMMON
	struct mesh {
		GLuint			VB;
		GLuint			IBMesh;

		GLuint			IB[20];
		std::vector<CVertex> bufferVertex;
		std::vector<unsigned short> meshbufferIndex;
		std::vector<unsigned short> bufferIndex;
		infotex infoTexture;
		std::vector<std::vector<unsigned short>*> bufferIndexForTextures;
	};

	CObject3D() : shaderID(0), shaderWireFrame(0) {}

	GLuint	shaderID;
	GLuint	shaderWireFrame;
	GLint	vertexAttribLocSimple;
	GLint   matWorldViewProjUniformLocSimple;

	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	binormalAttribLoc;
	GLint	tangenteAttribLoc;
	GLint	uvAttribLoc;
	GLint	normalLoc;
	GLint	diffuseLoc;
	GLint	specularLoc;
	GLint	glossLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;


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
		CMatrix4D LigthView;
		CVector4D DirectionGlobalLight;
		CVector4D ColorGlobalLight;
		CVector4D PositionPointLight;
		CVector4D ColorPointLight;
		CVector4D PosCamera;
	};

	struct CBufferRes
	{
		CMatrix4D WVP;
	};
	struct mesh {
		ComPtr<ID3D11Buffer>		VB;
		ComPtr<ID3D11Buffer>		IBMesh;
		ComPtr<ID3D11Buffer>		IB[20];
		std::vector<CVertex> bufferVertex;
		std::vector<unsigned short> meshbufferIndex;

		std::vector<unsigned short> bufferIndex;
		infotex infoTexture;
		std::vector<std::vector<unsigned short>*> bufferIndexForTextures;
	};
	bool flagShader = true;
	CObject3D() {}

	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	ComPtr<ID3D11VertexShader>  pVSRes;
	ComPtr<ID3D11PixelShader>   pFSRes;
	ComPtr<ID3DBlob>            VS_blobRes;
	ComPtr<ID3DBlob>            FS_blobRes;
	ComPtr<ID3D11InputLayout>   LayoutRes;
	ComPtr<ID3D11Buffer>        pd3dConstantBufferRes;

	CObject3D::CBufferRes			CnstBufferRes;
	CObject3D::CBuffer				CnstBuffer;
	Texture *TexturesObj[20];
	int signature;
	TextureD3D*	d3dxTextures[8];

#endif
	std::vector<mesh> meshes;

};


#endif