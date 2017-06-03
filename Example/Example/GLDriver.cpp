
#include "GLDriver.h"
#include "TextureGL.h"
#include "GLRT.h"

#include <iostream>
#include <string>

#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30))
void EGLError(const char* c_ptr) {
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS) {
		std::cout << "EGL CALL: " << c_ptr << " Error Code: " << iErr << std::endl;
	}
}

bool OpenNativeDisplay(EGLNativeDisplayType* nativedisp_out)
{
	*nativedisp_out = (EGLNativeDisplayType)NULL;
	return true;
}
#endif

void	GLDriver::InitDriver() {
#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30))

	EGLint numConfigs, w, h;

	EGLNativeDisplayType nativeDisplay;

	if(!OpenNativeDisplay(&nativeDisplay)){
		std::cout << "can't open native display" << std::endl;
	}

	eglDisplay = eglGetDisplay(nativeDisplay);

	EGLError("eglGetDisplay");

	EGLint iMajorVersion, iMinorVersion;

	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
		std::cout << "Failed to initialize egl" << std::endl;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	EGLError("eglBindAPI");

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE,		8,
		EGL_GREEN_SIZE,		8,
		EGL_RED_SIZE,		8,
		EGL_DEPTH_SIZE,		24,
		EGL_NONE
	};

	eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs);

	EGLError("eglBindAPI");

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	EGLError("eglCreateWindowSurface");

	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	EGLError("eglCreateContext");

	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
		std::cout << "Failed to make current" << std::endl;
		return;
	}

	eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
	eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);
	
	
#elif defined(USING_OPENGL)
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
	}
	else {
		printf("GLEW OK\n");
	}
	SDL_Surface *sur = SDL_GetVideoSurface();
	width = sur->w;
	height = sur->h;
#endif
	std::string GL_Version = std::string((const char*)glGetString(GL_VERSION));
	std::string GL_Extensions = std::string((const char*)glGetString(GL_EXTENSIONS));

	std::cout << "GL Version: " << GL_Version << "\n\nExtensions\n\n" << GL_Extensions << std::endl;
	


	glEnable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFBO);
#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
	for (int i = 0; i < 16; i++) {
		DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
#endif
}

void	GLDriver::CreateSurfaces() {

}

void	GLDriver::DestroySurfaces() {

}

void	GLDriver::Update() {

}

void	GLDriver::DestroyDriver() {
#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30))
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
#endif
}

void	GLDriver::SetWindow(void *window) {
#if (defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30))
	eglWindow = GetActiveWindow();
#endif
}

void	GLDriver::Clear() {
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

}

void	GLDriver::SwapBuffers() {
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
	eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(USING_OPENGL)
	SDL_GL_SwapBuffers();
#endif
}

void GLDriver::SetDimensions(int, int)
{
}

int  GLDriver::CreateRT(int nrt, int cf, int df, int w, int h) {
	GLRT	*pRT = new GLRT;
	if (w == 0)
		w = width;
	if (h == 0)
		h = height;
	if (pRT->LoadRT(nrt, cf, df, w, h)) {
		glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
		RTs.push_back(pRT);
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}

void GLDriver::PushRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);

	glBindFramebuffer(GL_FRAMEBUFFER, pRT->vFrameBuffers[0]);

#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
	glDrawBuffers(pRT->number_RT, DrawBuffers);
#endif

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDriver::PopRT() {
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFBO);
}

void GLDriver::DestroyRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	RTs[id]->DestroyRT();
	GLRT *pRT = dynamic_cast<GLRT*>(RTs[id]);
	delete pRT;

}

void GLDriver::DestroyRTs() {
	for (unsigned int i = 0; i < RTs.size(); i++) {
		GLRT *pRT = dynamic_cast<GLRT*>(RTs[i]);
		delete pRT;
	}
}