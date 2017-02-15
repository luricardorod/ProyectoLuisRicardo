#include "UtilsGL.h"
#include <stdio.h>    
#include <stdlib.h>
#include <string>

void checkcompilederrors(GLuint shader, GLenum type) {
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
		}
		else if (type == GL_VERTEX_SHADER) {
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else {
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	checkcompilederrors(shader, type);
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
	str = (char*)malloc(len * sizeof(char));
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
	str = (char*)malloc(len * sizeof(char));
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

void loadVertexIndexForFile(char * path, CVertex * vertices, unsigned short * indices, int * sizeIndex, int *sizeVertex)
{
	long sizeFile;
	char *archivo = file2string(path, &sizeFile);
	int counter = 0;
	int dif;

	float numberConvert;
	int positionBuffer;
	bool flag = true;
	char* buffer1 = new char[20];
	while (flag)
	{
		if (
			archivo[counter] == 'M' &&
			archivo[counter + 1] == 'e' &&
			archivo[counter + 2] == 's' &&
			archivo[counter + 3] == 'h' &&
			archivo[counter + 4] == ' ' &&
			archivo[counter + 5] == 'm'
			)
		{
			counter += 9;
			while (flag)
			{
				if (archivo[counter] == '{')
				{
					counter += 1;
					positionBuffer = 0;

					while (archivo[counter] != ';')
					{
						buffer1[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					buffer1[positionBuffer] = '\0';
					(*sizeVertex) = std::atof(buffer1);
					vertices = new CVertex[(*sizeVertex)];
					for (int i = 0; i < (*sizeVertex); i++)
					{
						counter++;
						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].x = numberConvert;
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].y = numberConvert;
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].z = numberConvert;
						vertices[i].w = 1;
						counter++;
					}
					//end vertex

					//startIndex
					counter += 2;
					dif = counter;
					positionBuffer = 0;

					while (archivo[counter] != ';')
					{
						buffer1[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					buffer1[positionBuffer] = '\0';
					dif -= std::atof(buffer1);
					(*sizeIndex) = (std::atof(buffer1) * 3);
					indices = new unsigned short[(*sizeIndex)];
					for (int i = 0; i < (*sizeIndex); i++)
					{

						counter++;
						while (archivo[counter] != ';')
						{
							counter++;
						}
						counter++;
						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						indices[i + 2] = numberConvert;
						counter++;
						i++;

						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						indices[i] = numberConvert;
						counter++;
						i++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						indices[i - 2] = numberConvert;
						counter++;

					}
					//endIndex

					//normals
					counter += 2;
					dif = counter - dif;
					while (archivo[counter] != ';')
					{
						counter++;
					}
					for (int i = 0; i < (*sizeVertex); i++)
					{
						counter++;
						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].nx = numberConvert;
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].ny = numberConvert;
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].nz = numberConvert;
						vertices[i].nw = 1;
						counter++;
					}
					//endnormals

					//uvs
					counter += dif + 30;
					while (archivo[counter] != ';')
					{
						counter++;
					}
					counter++;
					for (int i = 0; i < (*sizeVertex); i++)
					{
						counter++;
						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].s = numberConvert;
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						numberConvert = std::atof(buffer1);
						vertices[i].t = numberConvert;
						counter++;
					}
					//enduvs
					flag = false;
				}
				counter++;
			}
		}
		counter++;
		if (counter > sizeFile)
		{
			flag = false;
			printf("Error al leer el archivo no mesh");
			vertices = NULL;
			indices = NULL;
			sizeIndex = NULL;
			sizeVertex = NULL;
		}
	}
	//delete archivo;
}

