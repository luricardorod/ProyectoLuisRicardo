#ifndef UAD_GLDRIVER_H
#define UAD_GLDRIVER_H

#include "BaseDriver.h"
#include "Config.h"

#if defined(USING_OPENGL_ES20)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#include <SDL/SDL.h>
#else
#include <GL/glew.h>
#include <SDL/SDL.h>
#endif


class GLDriver : public BaseDriver {
public:
	GLDriver() {  }
	void	InitDriver();
	void	CreateSurfaces();
	void	DestroySurfaces();
	void	Update();
	void	DestroyDriver();
	void	SetWindow(void *window) ;

	void	Clear();
	void	SwapBuffers();
	void	SetDimensions(int, int);

	int 	CreateRT(int nrt, int cf, int df, int w, int h);
	void	PushRT(int id);
	void	PopRT();
	void	DestroyRT(int id);
	void	DestroyRTs();

#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30))

	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

	EGLNativeWindowType	eglWindow;
#endif
	GLint				CurrentFBO;
#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30)
	GLenum				DrawBuffers[16];
#endif
	int	width, height;

};

#endif