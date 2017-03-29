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

#ifdef USING_OPENGL_ES
	struct mesh {
		GLuint			VB;
		GLuint			IB[20];
		std::vector<CVertex> bufferVertex;
		std::vector<unsigned short> bufferIndex;
		infotex infoTexture;
		std::vector<std::vector<unsigned short>*> bufferIndexForTextures;
	};

	CObject3D() : shaderID(0) {}
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	binormalAttribLoc;
	GLint	tangenteAttribLoc;
	GLint	uvAttribLoc;
	GLint	normalLoc;
	GLint	diffuseLoc;
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
		CVector4D DirectionGlobalLight;
		CVector4D ColorGlobalLight;
		CVector4D PositionPointLight;
		CVector4D ColorPointLight;
		CVector4D PosCamera;
	};
	struct mesh {
		ComPtr<ID3D11Buffer>		VB;
		ComPtr<ID3D11Buffer>		IB[20];
		std::vector<CVertex> bufferVertex;
		std::vector<unsigned short> bufferIndex;
		infotex infoTexture;
		std::vector<std::vector<unsigned short>*> bufferIndexForTextures;
	};
	CObject3D() {}
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	CObject3D::CBuffer				CnstBuffer;
	Texture *Textures[20];

#endif
	std::vector<mesh> meshes;

};


#endif