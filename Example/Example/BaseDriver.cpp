#include "BaseDriver.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Config.h"
BaseDriver*	g_pBaseDriver = 0;


bool BaseRT::LoadRT(int nrt, int cf, int df, int w, int h) {
	this->number_RT = nrt;
	this->color_format = cf;
	this->depth_format = df;
	this->w = w;
	this->h = h;
	return LoadAPIRT();
}

void BaseRT::DestroyRT() {
	DestroyAPIRT();
}

bool ShaderBase::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {

	std::string Defines = "";

#if defined(USING_GL_COMMON30) || defined(USING_GL_COMMON31)
	Defines += "#version 300 es\n\n";
	Defines += "#define ES_30\n\n";
#endif

#if VDEBUG_NO_LIGHT
	Defines += "#define NO_LIGHT\n\n";
#endif

#if VDEBUG_SIMPLE_COLOR
	Defines += "#define SIMPLE_COLOR\n\n";
#endif
#ifdef USE_GLOBALLIGHT
	Defines += "#define USE_GLOBALLIGHT\n\n";
#endif // USE_GLOBALLIGHT

	if (sig&Signature::HAS_NORMALS)
		Defines += "#define USE_NORMALS\n\n";
	if (sig&Signature::HAS_TEXCOORDS0)
		Defines += "#define USE_TEXCOORD0\n\n";
	if (sig&Signature::HAS_TEXCOORDS1)
		Defines += "#define USE_TEXCOORD1\n\n";
	if (sig&Signature::HAS_TANGENTS)
		Defines += "#define USE_TANGENTS\n\n";
	if (sig&Signature::HAS_BINORMALS)
		Defines += "#define USE_BINORMALS\n\n";
	if (sig&Signature::DIFFUSE_MAP)
		Defines += "#define DIFFUSE_MAP\n\n";
	if (sig&Signature::SPECULAR_MAP)
		Defines += "#define SPECULAR_MAP\n\n";
	if (sig&Signature::GLOSS_MAP)
		Defines += "#define GLOSS_MAP\n\n";
	if (sig&Signature::NORMAL_MAP)
		Defines += "#define NORMAL_MAP\n\n";
	if (sig&Signature::REFLECT_MAP)
		Defines += "#define REFLECT_MAP\n\n";
	if (sig&Signature::LIGTHSHADOWMAP)
		Defines += "#define LIGHT_SHADOW_MAP\n\n";
	if (sig&Signature::NO_LIGHT_AT_ALL)
		Defines += "#define NO_LIGHT\n\n";
	if (sig&Signature::GBUFF_PASS) {
		Defines += "#define G_BUFFER_PASS\n\n";
#if defined(USING_GL_COMMON20)
		Defines += "#define NON_LINEAR_DEPTH\n\n";
#endif
	}
	if (sig&Signature::FSQUAD_1_TEX)
		Defines += "#define FSQUAD_1_TEX\n\n";
	if (sig&Signature::FSQUAD_3_TEX)
		Defines += "#define FSQUAD_3_TEX\n\n";
	if (sig&Signature::DEFERRED_PASS) {
		Defines += "#define DEFERRED_PASS\n\n";
#if defined(USING_GL_COMMON20)
		Defines += "#define NON_LINEAR_DEPTH\n\n";
#endif
	}

#ifdef USING_OPENGL
	Defines += "#define lowp \n\n";
	Defines += "#define mediump \n\n";
	Defines += "#define highp \n\n";
#endif

	src_vs = Defines + src_vs;
	src_fs = Defines + src_fs;
	this->Sig = sig;
	return CreateShaderAPI(src_vs, src_fs, sig);
}
