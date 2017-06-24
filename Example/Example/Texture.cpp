

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

typedef unsigned int DWORD;
struct DDS_PIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};
typedef struct {
	union {
	DWORD elements[31];
	struct
	{
		DWORD           dwSize;
		DWORD           dwFlags;
		DWORD           dwHeight;
		DWORD           dwWidth;
		DWORD           dwPitchOrLinearSize;
		DWORD           dwDepth;
		DWORD           dwMipMapCount;
		DWORD           dwReserved1[11];
		DDS_PIXELFORMAT ddspf;
		DWORD           dwCaps;
		DWORD           dwCaps2;
		DWORD           dwCaps3;
		DWORD           dwCaps4;
		DWORD           dwReserved2;
	};
	};
} DDS_HEADER;
int		Texture::LoadTextureCubeMap(char *fn) {
	std::string path = "Textures/";
	path += std::string(fn);
	char *cstrpath = new char[path.length() + 1];
	strcpy(cstrpath, path.c_str());


	FILE *fd;
	long len, r;
	char *file;
	if (!(fd = fopen(cstrpath, "rb"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	file = (char*)malloc((len+1) * sizeof(char));
	r = fread(file, sizeof(char), len, fd);
	file[r] = '\0';
	fclose(fd);
	DWORD dwMagic;
	DDS_HEADER ddsHeader;
	int counter = 0;
	dwMagic = *(int*)&file[counter];
	counter = 4;
	for (int i = 0; i < 31; i++)
	{
		ddsHeader.elements[i] = *(int*)&file[counter + i*4];
	}
	//counter++;
	//ddsHeader.dwSize = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwFlags = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwHeight = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwWidth = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwPitchOrLinearSize = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwDepth = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwMipMapCount = *(int*)&file[counter];
	//counter++;
	//for (int i = 0; i < 11; i++)
	//{
	//	ddsHeader.dwReserved1[i] = *(int*)&file[counter];
	//	counter++;
	//}
	////DDS_PIXELFORMAT ddsHeader.ddspf;
	//ddsHeader.ddspf.dwSize = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwFlags = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwFourCC = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwRGBBitCount = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwRBitMask = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwGBitMask = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwBBitMask = *(int*)&file[counter];
	//counter++;
	//ddsHeader.ddspf.dwABitMask = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwCaps = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwCaps2 = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwCaps3 = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwCaps4 = *(int*)&file[counter];
	//counter++;
	//ddsHeader.dwReserved2 = *(int*)&file[counter];
	//counter++;
	//
	if (ddsHeader.ddspf.dwFlags & 4)
	{
		printf("revisar este caso");
	}
	//
	return 0;
}
int		Texture::LoadTexture(char *fn) {
	std::string path = "Textures/";
	path += std::string(fn);
	int x = 0, y = 0, channels = 0;
	unsigned char *buffer = stbi_load(path.c_str(), &x, &y, &channels, 0);

	if (!buffer)
		return -1;

	size = x*y*channels;
	bounded = 1;
	this->x = x;
	this->y = y;
	mipmaps = 1;
	this->params = params;

	switch (channels) {
	case 1: {
		props |= TEXT_BASIC_FORMAT::CH_ALPHA;
	}break;
	case 3: {
		props |= TEXT_BASIC_FORMAT::CH_RGB;
	}break;
	case 4: {
		props |= TEXT_BASIC_FORMAT::CH_RGBA;
	}break;
	}

	memcpy(&optname[0], fn, strlen(fn));
	optname[strlen(fn)] = '\0';

	LoadAPITexture(buffer);
	stbi_image_free(buffer);
	return id;
}

void Texture::DestroyTex() {
	DestroyAPITexture();
}