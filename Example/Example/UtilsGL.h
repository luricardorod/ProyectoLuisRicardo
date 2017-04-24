#ifndef UAD_UTILS_GL_H
#define UAD_UTILS_GL_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "PrimitiveBase.h"
#include <stdio.h>

bool checkcompilederrors(GLuint shader, GLenum type);
GLuint createShader(GLenum type, char* pSource);
char *file2string(const char *path, long* size);
char *file2string(const char *path);

unsigned char* loadBMPFile(char *path);

#endif