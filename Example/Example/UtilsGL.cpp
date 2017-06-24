#include "UtilsGL.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string>

bool checkcompilederrors(GLuint shader, GLenum type) {
	bool flag = true;
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER) {
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
			flag = false;
		}
		else if (type == GL_VERTEX_SHADER) {
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
			flag = false;
		}
		else {
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
			flag = false;
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
	return flag;
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	if (!checkcompilederrors(shader, type))
	{
		shader = NULL;
	}
	return shader;
}

char *file2string(const char *path) {
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc((len + 1) * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);
	return str;
}

char *file2string(const char *path, long* size) {
	FILE *fd;
	long len;
	char *str;
	if (!(fd = fopen(path, "r"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc((len + 1) * sizeof(char));
	(*size) = fread(str, sizeof(char), len, fd);
	str[(*size)] = '\0';
	fclose(fd);
	return str;
}

unsigned char * loadBMPFile(char * path)
{
	FILE *fd;
	long len, r;
	unsigned char file[54];
	if (!(fd = fopen(path, "rb"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	
	fread(file, sizeof(unsigned char), 54, fd);
	int width = *(int*)&file[18];
	int height = *(int*)&file[22];
	int size = 3 * width*height;

	if (
		(file[0]) == 'B' &&
		(file[1]) == 'M'
		)
	{
		unsigned char *buffer = new unsigned char[size];
		fread(buffer, sizeof(unsigned char), size, fd);
		fclose(fd);
		for (int i = 0; i < size; i += 3)
		{
			unsigned char temp = buffer[i];
			buffer[i] = buffer[i + 2];
			buffer[i + 2] = temp;
			printf("r=%x \t", (unsigned int)buffer[i]);
			printf("g=%x \t", (unsigned int)buffer[i+1]);
			printf("b=%x \t", (unsigned int)buffer[i+2]);
		}
		return buffer;
	}
	
	printf("ARchivo no valido");

	fclose(fd);
	return NULL;
}
