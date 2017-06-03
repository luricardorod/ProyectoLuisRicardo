#ifndef UAD_BASEDRIVER_H
#define UAD_BASEDRIVER_H
#include <vector>
#include "Texture.h"

struct BaseRT {
	enum ATTACHMENTS {
		COLOR0_ATTACHMENT = 1,
		COLOR1_ATTACHMENT = 2,
		COLOR2_ATTACHMENT = 4,
		COLOR3_ATTACHMENT = 8,
		DEPTH_ATTACHMENT = 16
	};

	enum FORMAT {
		FD16 = 0,
		F32,
		RGB8,
		RGBA8,
		RGBA16F,
		RGBA32F,
		BGR8,
		BGRA8,
		BGRA32
	};

	bool			LoadRT(int nrt, int cf, int df, int w, int h) {
		this->number_RT = nrt;
		this->color_format = cf;
		this->depth_format = df;
		this->w = w;
		this->h = h;
		return LoadAPIRT();
	}
	virtual bool	LoadAPIRT() = 0;

	void			DestroyRT() {
		DestroyAPIRT();
	}
	virtual void	DestroyAPIRT() = 0;

	int w;
	int h;
	int number_RT;
	int color_format;
	int depth_format;

	std::vector<Texture*>							vColorTextures;
	Texture*										pDepthTexture;
};
class BaseDriver {
public:
	BaseDriver() {  }
	virtual	void	InitDriver() = 0;
	virtual void	CreateSurfaces() = 0;
	virtual void	DestroySurfaces() = 0;
	virtual void	Update() = 0;
	virtual void	DestroyDriver() = 0;
	virtual void	SetWindow(void *window) = 0;
	virtual void	SetDimensions(int, int) = 0;
	virtual void	Clear() = 0;	
	virtual void	SwapBuffers() = 0;

	virtual int 	 CreateRT(int nrt, int cf, int df, int w, int h) = 0;
	virtual void	 PushRT(int id) = 0;
	virtual void	 PopRT() = 0;
	virtual void	 DestroyRT(int id) = 0;
	virtual void	 DestroyRTs() = 0;

	std::vector<BaseRT*>		RTs;
};

#endif