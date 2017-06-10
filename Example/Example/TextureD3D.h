#ifndef UAD_TEXTURE_D3D_H
#define UAD_TEXTURE_D3D_H

#include "Config.h"

#include "BaseDriver.h"


#include <d3d11.h>
#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;


class TextureD3D : public Texture {
public:
	TextureD3D() {}

	void	SetTextureParams();
	void	GetFormatBpp(unsigned int &props, unsigned int &Format, unsigned int &bpp);

	void	LoadAPITexture(unsigned char* buffer);
	void	LoadAPITextureCompressed(unsigned char* buffer);
	void	DestroyAPITexture();


	ComPtr<ID3D11Texture2D>				Tex;
	ComPtr<ID3D11ShaderResourceView>    pSRVTex;
	ComPtr<ID3D11SamplerState>          pSampler;

};

#endif
