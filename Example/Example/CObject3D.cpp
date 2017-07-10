#include "CObject3D.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <string>
#include <map>
#include "BaseDriver.h"
#include "CShaderD3DX.h"
#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void CObject3D::AddChildsAndBrothers() {
	std::vector<int> childs;

	for (int bone = 0; bone < bones.size(); bone++)
	{
		childs.clear();
		for (int i = 0; i < bones.size(); i++)
		{
			if (bones[i].parent == bones[bone].position)
			{
				childs.push_back(i);
			}
		}
		for (int i = 0; i < childs.size(); i++)
		{
			bones[childs[i]].brothers = childs;
		}
		bones[bone].childs = childs;
	}
}

int CObject3D::AddBone(char* archivo, int counter, int father) {
	int positionBuffer;

	char* buffer1 = new char[50];
	char* bufferName = new char[50];

	positionBuffer = 0;
	Bones newBone;
	while (archivo[counter] != ' ')
	{
		bufferName[positionBuffer] = archivo[counter];
		positionBuffer++;
		counter++;
	}
	bufferName[positionBuffer] = '\0';
	newBone.name = bufferName;
	newBone.parent = father;
	newBone.position = bones.size();
	counter += 2;
	while (archivo[counter] != '{')
	{
		counter++;
	}
	counter++;
	for (int i = 0; i < 16; i++)
	{
		positionBuffer = 0;
		while (!(archivo[counter] == ',' || archivo[counter] == ';'))
		{
			buffer1[positionBuffer] = archivo[counter];
			counter++;
			positionBuffer++;
		}
		buffer1[positionBuffer] = '\0';
		newBone.tranformMatrix.v[i] = std::atof(buffer1);
		counter++;
	}

	while (archivo[counter] != '}')
	{
		counter++;
	}
	counter++;

	while (archivo[counter] != '}')
	{
		counter++;
	}
	counter++;

	bones.push_back(newBone);
	
	while (archivo[counter] != '}')
	{
		while (archivo[counter] != '}' && archivo[counter] != 'F' && archivo[counter] != 'M')
		{
			counter++;
		}
		if (archivo[counter] == 'F')
		{
			counter += 6;
			counter = AddBone(archivo, counter, newBone.position);
			counter++;
		} else if (archivo[counter] == 'M')
		{
			bones.pop_back();
			return counter;
		}
	}
	return counter;
}


void CObject3D::Create(char * path) {

	long sizeFile;
	char *archivo = file2string(path, &sizeFile);
	int counter = 0;
	int tempSizeBufferIndex = 0;
	int counterBufferIndex = 0;
	int offsetVertex = 0;
	int counterBufferVertex = 0;
	int tempSizeBufferVertex = 0;
	int dif;
	int positionBuffer;
	bool flag = true;
	char* buffer1 = new char[20];

	mesh tempMesh;
	while (counter < sizeFile)
	{
		if (
			archivo[counter] == 'F' &&
			archivo[counter + 1] == 'r' &&
			archivo[counter + 2] == 'a' &&
			archivo[counter + 3] == 'm' &&
			archivo[counter + 4] == 'e' &&
			archivo[counter + 5] == ' ' &&
			archivo[counter + 6] != '{'
			) 
		{
			counter += 6;
			counter = AddBone(archivo, counter, -1);
			if (archivo[counter] == 'M')
			{
				AddChildsAndBrothers();
			}
		}
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
			bones;
			flag = true;
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
					tempMesh.bufferVertex.resize(std::atof(buffer1));
					tempMesh.bonsInlfluenceForVertex.resize(std::atof(buffer1));

					for (int i = 0; i < tempMesh.bufferVertex.size(); i++)
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
						tempMesh.bufferVertex[i].x = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferVertex[i].y = std::atof(buffer1);

						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferVertex[i].z = std::atof(buffer1);
						tempMesh.bufferVertex[i].w = 1;


						counterBufferVertex++;
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
					tempMesh.bufferIndex.resize(std::atof(buffer1) * 3);
					tempMesh.meshbufferIndex.resize(std::atof(buffer1) * 3 * 2);

					counterBufferIndex = 0;
					for (int i = 0; i < tempMesh.bufferIndex.size(); i +=3)
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

						tempMesh.bufferIndex[i + 2] = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferIndex[i + 1] = std::atof(buffer1);


						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							counter++;
							positionBuffer++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferIndex[i] = std::atof(buffer1);

						counter++;
						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i];
						counterBufferIndex ++;
						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i + 1];
						counterBufferIndex++;

						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i];
						counterBufferIndex++;
						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i + 2];
						counterBufferIndex++;

						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i + 1];
						counterBufferIndex++;
						tempMesh.meshbufferIndex[counterBufferIndex] = tempMesh.bufferIndex[i + 2];
						counterBufferIndex++;
					}
					//endIndex

					//normals
					counter += 2;
					dif = counter - dif;
					while (archivo[counter] != ';')
					{
						counter++;
					}
					counterBufferVertex -= tempSizeBufferVertex;
					for (int i = 0; i < tempMesh.bufferVertex.size(); i++)
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
						tempMesh.bufferVertex[i].nx = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferVertex[i].ny = std::atof(buffer1);


						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferVertex[i].nz = std::atof(buffer1);
						tempMesh.bufferVertex[i].nw = 1;


						counter++;
						counterBufferVertex++;
					}
					//endnormals

					//uvs
					counter += dif + 30;
					while (archivo[counter] != ';')
					{
						counter++;
					}
					counter++;
					counterBufferVertex -= tempSizeBufferVertex;
					for (int i = 0; i < tempMesh.bufferVertex.size(); i++)
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
						tempMesh.bufferVertex[i].s = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						tempMesh.bufferVertex[i].t = std::atof(buffer1);

						counterBufferVertex++;
						counter++;
					}
					//enduvs

					//DEclData
					while (archivo[counter] != '{')
					{
						counter++;
					}
					counter++;
					positionBuffer = 0;
					while (archivo[counter] != ';')
					{
						buffer1[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					buffer1[positionBuffer] = '\0';
					//numero de valores del decl data
					while (!(archivo[counter] == ';' && archivo[counter+1] == ';'))
					{
						counter++;
					}
					counter += 2;

					positionBuffer = 0;
					while (archivo[counter] != ';')
					{
						buffer1[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					buffer1[positionBuffer] = '\0';
					unsigned int sizeDeclData = std::atoi(buffer1);
					counter++;
					///
					unsigned long tempNumber;
					float *castFloat;
					for (int i = 0; i < tempMesh.bufferVertex.size(); i++)
					{
						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].bix = (*castFloat);
						
						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].biy = (*castFloat);

						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].biz = (*castFloat);
						tempMesh.bufferVertex[i].biw = 1;


						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].tanx = (*castFloat);

						positionBuffer = 0;
						while (archivo[counter] != ',')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].tany = (*castFloat);

						positionBuffer = 0;
						while (archivo[counter] != ',' && archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						counter++;
						buffer1[positionBuffer] = '\0';
						tempNumber = std::strtoul(buffer1, NULL, 0);
						castFloat = (float*)&tempNumber;
						tempMesh.bufferVertex[i].tanz = (*castFloat);
						tempMesh.bufferVertex[i].tanw = 1;

					}

					//
					while (archivo[counter] != '}')
					{
						counter++;
					}
					counter++;
					int counterOpenBrackets = 1;
					while (!(counterOpenBrackets == 0))
					{
						if (archivo[counter + 4] == 'M' &&
							archivo[counter + 5] == 'e' &&
							archivo[counter + 6] == 's' &&
							archivo[counter + 7] == 'h' &&
							archivo[counter + 8] == 'M' &&
							archivo[counter + 9] == 'a' &&
							archivo[counter + 10] == 't')
						{
							counterOpenBrackets--;
						}
						if (archivo[counter] == '{')
						{
							counterOpenBrackets++;
						}
						if (archivo[counter] == '}')
						{
							counterOpenBrackets--;
						}
						if (archivo[counter] == 'S' &&
							archivo[counter + 1] == 'k' &&
							archivo[counter + 2] == 'i' &&
							archivo[counter + 3] == 'n' &&
							archivo[counter + 4] == 'W' &&
							archivo[counter + 5] == 'e' &&
							archivo[counter + 6] == 'i' &&
							archivo[counter + 7] == 'g' &&
							archivo[counter + 8] == 'h' &&
							archivo[counter + 9] == 't' &&
							archivo[counter + 10] == 's')
						{
							counterOpenBrackets++;
							while (archivo[counter] != '"')
							{
								counter++;
							}
							counter++;
							positionBuffer = 0;
							while (archivo[counter] != '"')
							{
								buffer1[positionBuffer] = archivo[counter];
								positionBuffer++;
								counter++;
							}
							counter += 3;
							buffer1[positionBuffer] = '\0';

							int selectedBone;
							for (int i = 0; i < bones.size(); i++)
							{
								if (strcmp(buffer1, bones[i].name) == 0)
								{
									selectedBone = i;
									break;
								}
							}

							int numberOfVertexAfectedForBone;
							positionBuffer = 0;
							std::vector<int> vertexAfected;
							std::vector<float> influencVertexAfected;

							while (archivo[counter] != ';')
							{
								buffer1[positionBuffer] = archivo[counter];
								positionBuffer++;
								counter++;
							}
							buffer1[positionBuffer] = '\0';
							numberOfVertexAfectedForBone = std::atoi(buffer1);
							counter++;
							for (int i = 0; i < numberOfVertexAfectedForBone; i++)
							{
								positionBuffer = 0;
								while (archivo[counter] != ';' && archivo[counter] != ',')
								{
									buffer1[positionBuffer] = archivo[counter];
									positionBuffer++;
									counter++;
								}
								counter++;
								buffer1[positionBuffer] = '\0';
								vertexAfected.push_back(std::atoi(buffer1));
							}
							tempMesh.bonsInlfluenceForVertex[std::atoi(buffer1)];
							counter++;
							for (int i = 0; i < numberOfVertexAfectedForBone; i++)
							{
								positionBuffer = 0;
								while (archivo[counter] != ';' && archivo[counter] != ',')
								{
									buffer1[positionBuffer] = archivo[counter];
									positionBuffer++;
									counter++;
								}
								counter++;
								buffer1[positionBuffer] = '\0';
								influencVertexAfected.push_back(std::atoi(buffer1));
 							}
							counter++;

							for (int i = 0; i < vertexAfected.size(); i++)
							{
								tempMesh.bonsInlfluenceForVertex[vertexAfected[i]].bones.push_back(selectedBone);
								tempMesh.bonsInlfluenceForVertex[vertexAfected[i]].influence.push_back(influencVertexAfected[i]);
							}
							

						}
						counter++;
					}
					counter++;
					flag = false;
				}
				counter++;
			}
		}
		if (
			archivo[counter] == 'L' &&
			archivo[counter + 1] == 'i' &&
			archivo[counter + 2] == 's' &&
			archivo[counter + 3] == 't' &&
			archivo[counter + 4] == ' ' &&
			archivo[counter + 5] == 'm' &&
			archivo[counter + 6] == 't' &&
			archivo[counter + 7] == 'l' &&
			archivo[counter + 8] == 's'
			)
		{
			infotex tempContentInfotext;
			counter = counter + 12;
			positionBuffer = 0;
			while (archivo[counter] != ';')
			{
				buffer1[positionBuffer] = archivo[counter];
				positionBuffer++;
				counter++;
			}
			buffer1[positionBuffer] = '\0';
			tempContentInfotext.numberOfTextures = std::atoi(buffer1);
			counter++;
			positionBuffer = 0;
			while (archivo[counter] != ';')
			{
				buffer1[positionBuffer] = archivo[counter];
				positionBuffer++;
				counter++;
			}
			buffer1[positionBuffer] = '\0';
			tempContentInfotext.indicesTextures.resize(std::atoi(buffer1));
			counter++;
			for (int i = 0; i < tempContentInfotext.indicesTextures.size(); i++)
			{
				positionBuffer = 0;
				while (archivo[counter] != ';' && archivo[counter] != ',')
				{
					buffer1[positionBuffer] = archivo[counter];
					positionBuffer++;
					counter++;
				}
				counter++;
				buffer1[positionBuffer] = '\0';
				tempContentInfotext.indicesTextures[i] = std::atof(buffer1);
			}

			textureByMesh tempTextureMesh;
			while (tempContentInfotext.textures.size() < tempContentInfotext.numberOfTextures)
			{
				if (
					archivo[counter-1] == ' ' &&
					archivo[counter] == 'M' &&
					archivo[counter + 1] == 'a' &&
					archivo[counter + 2] == 't' &&
					archivo[counter + 3] == 'e' &&
					archivo[counter + 4] == 'r' &&
					archivo[counter + 5] == 'i' &&
					archivo[counter + 6] == 'a' &&
					archivo[counter + 7] == 'l' &&
					archivo[counter + 8] == ' '
					) {
					counter += 8;
					tempTextureMesh.gloss = false;
					tempTextureMesh.normal = false;
					tempTextureMesh.specular = false;
				}
				if (
					archivo[counter] == 'T' &&
					archivo[counter + 1] == 'e' &&
					archivo[counter + 2] == 'x' &&
					archivo[counter + 3] == 't' &&
					archivo[counter + 4] == 'u' &&
					archivo[counter + 5] == 'r' &&
					archivo[counter + 6] == 'e' &&
					archivo[counter + 7] == 'F' &&
					archivo[counter + 8] == 'i' &&
					archivo[counter + 9] == 'l' &&
					archivo[counter + 10] == 'e' &&
					archivo[counter + 11] == 'n' &&
					archivo[counter + 12] == 'a' &&
					archivo[counter + 13] == 'm' &&
					archivo[counter + 14] == 'e'
				) {
					counter += 15;
					tempContentInfotext.textures.push_back(tempTextureMesh);
				}
				if (
					archivo[counter] == '"' &&
					archivo[counter + 1] == 'd' &&
					archivo[counter + 2] == 'i' &&
					archivo[counter + 3] == 'f' &&
					archivo[counter + 4] == 'f' &&
					archivo[counter + 5] == 'u' &&
					archivo[counter + 6] == 's' &&
					archivo[counter + 7] == 'e' &&
					archivo[counter + 8] == 'M' &&
					archivo[counter + 9] == 'a' &&
					archivo[counter + 10] == 'p' &&
					archivo[counter + 11] == '"'
					)
				{
					counter = counter + 12;

					while (archivo[counter] != '"')
					{
						counter++;
					}
					counter++;
					char* temp = new char[30];
					positionBuffer = 0;
					while (archivo[counter] != '"')
					{
						if (archivo[counter] == 92) {
							positionBuffer = 0;
							counter++;
						}
						else {
							temp[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
					}
					temp[positionBuffer] = '\0';
					tempTextureMesh.diffuseName = temp;
				}
				//SPECULAR
				if (
					archivo[counter] == '"' &&
					archivo[counter + 1] == 's' &&
					archivo[counter + 2] == 'p' &&
					archivo[counter + 3] == 'e' &&
					archivo[counter + 4] == 'c' &&
					archivo[counter + 5] == 'u' &&
					archivo[counter + 6] == 'l' &&
					archivo[counter + 7] == 'a' &&
					archivo[counter + 8] == 'r' &&
					archivo[counter + 9] == 'M' &&
					archivo[counter + 10] == 'a' &&
					archivo[counter + 11] == 'p' &&
					archivo[counter + 12] == '"'
					)
				{
					counter = counter + 13;

					while (archivo[counter] != '"')
					{
						counter++;
					}
					counter++;
					char* temp = new char[30];
					positionBuffer = 0;
					while (archivo[counter] != '"')
					{
						if (archivo[counter] == 92) {
							positionBuffer = 0;
							counter++;
						}
						else {
							temp[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
					}
					temp[positionBuffer] = '\0';
					tempTextureMesh.specularName = temp;
					tempTextureMesh.specular = true;
				}
				//GLOSSS
				if (
					archivo[counter] == '"' &&
					archivo[counter + 1] == 'g' &&
					archivo[counter + 2] == 'l' &&
					archivo[counter + 3] == 'o' &&
					archivo[counter + 4] == 's' &&
					archivo[counter + 5] == 's' &&
					archivo[counter + 6] == 'M' &&
					archivo[counter + 7] == 'a' &&
					archivo[counter + 8] == 'p' &&
					archivo[counter + 9] == '"'
					)
				{
					counter = counter + 10;

					while (archivo[counter] != '"')
					{
						counter++;
					}
					counter++;
					char* temp = new char[30];
					positionBuffer = 0;
					while (archivo[counter] != '"')
					{
						if (archivo[counter] == 92) {
							positionBuffer = 0;
							counter++;
						}
						else {
							temp[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
					}
					temp[positionBuffer] = '\0';
					tempTextureMesh.glossName = temp;
					tempTextureMesh.gloss = true;
				}
				//NORMAL
				if (
					archivo[counter] == '"' &&
					archivo[counter + 1] == 'n' &&
					archivo[counter + 2] == 'o' &&
					archivo[counter + 3] == 'r' &&
					archivo[counter + 4] == 'm' &&
					archivo[counter + 5] == 'a' &&
					archivo[counter + 6] == 'l' &&
					archivo[counter + 7] == 'M' &&
					archivo[counter + 8] == 'a' &&
					archivo[counter + 9] == 'p' &&
					archivo[counter + 10] == '"'
					)
				{
					counter = counter + 11;

					while (archivo[counter] != '"')
					{
						counter++;
					}
					counter++;
					char* temp = new char[30];
					positionBuffer = 0;
					while (archivo[counter] != '"')
					{
						if (archivo[counter] == 92) {
							positionBuffer = 0;
							counter++;
						}
						else {
							temp[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
					}
					temp[positionBuffer] = '\0';
					tempTextureMesh.normalName = temp;
					tempTextureMesh.normal = true;

				}
				counter++;
			}
			tempMesh.infoTexture = tempContentInfotext;
			meshes.push_back(tempMesh);
			//infoMeshTextures.push_back(tempContentInfotext);
		}
		counter++;

	}
	for (auto i = meshes.begin(); i != meshes.end(); i++) {
		
		for (int j = 0; j < i->infoTexture.numberOfTextures; j++)
		{
			std::vector<unsigned short> *temp;
			temp = new std::vector<unsigned short>;
			i->bufferIndexForTextures.push_back(temp);
		}

		int countIndex = 0;
		for (auto textureCount = i->infoTexture.indicesTextures.begin(); textureCount != i->infoTexture.indicesTextures.end(); textureCount++)
		{
			i->bufferIndexForTextures[(*textureCount)]->push_back(i->bufferIndex[countIndex]);
			countIndex++;
			i->bufferIndexForTextures[(*textureCount)]->push_back(i->bufferIndex[countIndex]);
			countIndex++;
			i->bufferIndexForTextures[(*textureCount)]->push_back(i->bufferIndex[countIndex]);
			countIndex++;
		}
	}
	
#ifdef USING_GL_COMMON
	char *vsSourceP = file2string("Shaders/VS.glsl");
	char *fsSourceP = file2string("Shaders/FS.glsl");
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS.hlsl");
	char *fsSourceP = file2string("Shaders/FS.hlsl");
#endif

	

#ifdef USING_GL_COMMON
	std::string wireframeShader = "attribute highp vec4 Vertex;\n\n";
	wireframeShader += "uniform highp mat4 WVP;\n\n";

	wireframeShader += "void main(){\n\n";
	wireframeShader += "gl_Position = WVP*Vertex;\n\n";
	wireframeShader += "}\n\n";

	std::string vertex = wireframeShader;

	wireframeShader = "";
	wireframeShader = "void main() { gl_FragColor = vec4(1.0, 0.8, 0.2, 1.0);}";
	std::string fragment = wireframeShader;

	shaderWireFrame = glCreateProgram();
	GLuint vshader_id0 = createShader(GL_VERTEX_SHADER, (char*)vertex.c_str());
	GLuint fshader_id0 = createShader(GL_FRAGMENT_SHADER, (char*)fragment.c_str());


	glAttachShader(shaderWireFrame, vshader_id0);
	glAttachShader(shaderWireFrame, fshader_id0);

	glLinkProgram(shaderWireFrame);
	glUseProgram(shaderWireFrame);

	vertexAttribLocSimple = glGetAttribLocation(shaderWireFrame, "Vertex");
	matWorldViewProjUniformLocSimple = glGetUniformLocation(shaderWireFrame, "WVP");

	glUseProgram(0);

	shaderID = glCreateProgram();
	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vstr.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fstr.c_str());

	if (vshader_id && fshader_id)
	{
		glAttachShader(shaderID, vshader_id);
		glAttachShader(shaderID, fshader_id);

		glLinkProgram(shaderID);
		glUseProgram(shaderID);

		vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
		normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
		binormalAttribLoc = glGetAttribLocation(shaderID, "Binormal");
		tangenteAttribLoc = glGetAttribLocation(shaderID, "Tangente");
		uvAttribLoc = glGetAttribLocation(shaderID, "UV");

		matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
		matWorldUniformLoc = glGetUniformLocation(shaderID, "World");
		diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
		specularLoc = glGetUniformLocation(shaderID, "specularLoc");
		glossLoc = glGetUniformLocation(shaderID, "glossLoc");

		normalLoc = glGetUniformLocation(shaderID, "normalLoc");

	#ifdef USE_GLOBALLIGHT
		DirectionGlobalLight = glGetUniformLocation(shaderID, "DirectionGlobalLight");
		ColorGlobalLight = glGetUniformLocation(shaderID, "ColorGlobalLight");
	#endif
	#ifdef	USE_POINTLIGHT
		PositionPointLight = glGetUniformLocation(shaderID, "PositionPointLight");
		ColorPointLight = glGetUniformLocation(shaderID, "ColorPointLight");
	#endif
	#ifdef	USE_DIFFUSE
		PosCamera = glGetUniformLocation(shaderID, "PositionCamera");
	#endif
		glUseProgram(0);
	}
	else
	{
		shaderID = shaderWireFrame;
	}

	for (auto i = meshes.begin(); i != meshes.end(); i++)
	{
		glGenBuffers(1, &((*i).VB));
		glBindBuffer(GL_ARRAY_BUFFER, ((*i).VB));
		glBufferData(GL_ARRAY_BUFFER, (*i).bufferVertex.size() * sizeof(CVertex), (*i).bufferVertex.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (int index = 0; index < (*i).bufferIndexForTextures.size(); index++)
		{
			glGenBuffers(1, &((*i).IB[index]));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((*i).IB[index]));
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*i).bufferIndexForTextures[index]->size() * sizeof(unsigned short), (*i).bufferIndexForTextures[index]->data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glGenBuffers(1, &((*i).IBMesh));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((*i).IBMesh));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*i).meshbufferIndex.size() * sizeof(unsigned short), (*i).meshbufferIndex.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	std::map<std::string, int> list;
	for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
	{
		for (auto textures = (*mesh).infoTexture.textures.begin(); textures != (*mesh).infoTexture.textures.end(); textures++) {
			std::string tempString((*textures).diffuseName);
			auto iteradorFind = list.find(tempString);
			if (iteradorFind == list.end())
			{
				Texture	*tex = new TextureGL;
				list[tempString] = tex->LoadTexture((*textures).diffuseName);
				if (list[tempString] == -1) {
					list[tempString] = tex->LoadTexture("cheker.tga");
				}

				(*textures).idDiffuse = list[tempString];

			}
			else
			{
				(*textures).idDiffuse = list[tempString];
			}
			if ((*textures).specular)
			{
				std::string tempString((*textures).specularName);
				auto iteradorFind = list.find(tempString);
				if (iteradorFind == list.end())
				{
					Texture	*tex = new TextureGL;
					list[tempString] = tex->LoadTexture((*textures).specularName);
					if (list[tempString] == -1) {
						delete tex;
						textures->specular = false;
					}
					else
					{
						(*textures).idSpecular = list[tempString];
					}
				}
				else
				{
					(*textures).idSpecular = list[tempString];
				}
			}
			if ((*textures).gloss)
			{
				std::string tempString((*textures).glossName);
				auto iteradorFind = list.find(tempString);
				if (iteradorFind == list.end())
				{
					Texture	*tex = new TextureGL;
					list[tempString] = tex->LoadTexture((*textures).glossName);
					if (list[tempString] == -1) {
						delete tex;
						textures->gloss = false;
					}
					else
					{
						(*textures).idGloss = list[tempString];
					}
				}
				else
				{
					(*textures).idGloss = list[tempString];
				}
			}
			if ((*textures).normal)
			{
				std::string tempString((*textures).normalName);				
				auto iteradorFind = list.find(tempString);
				if (iteradorFind == list.end())
				{
					Texture	*tex = new TextureGL;
					list[tempString] = tex->LoadTexture((*textures).normalName);
					if (list[tempString] == -1) {
						delete tex;
						textures->normal = false;
					}
					else
					{
						(*textures).idNormal = list[tempString];
					}
				}
				else
				{
					(*textures).idNormal = list[tempString];
				}
			}
		}
	}
	
#elif defined(USING_D3D11)
	HRESULT hr;

	std::string wireframeShader = "cbuffer ConstantBuffer {\n\n";
	wireframeShader += "	float4x4 WVP;";
	wireframeShader += "}";
	wireframeShader += "struct VS_INPUT {";
	wireframeShader += "	float4 position : POSITION;";
	wireframeShader += "};";
	wireframeShader += "struct VS_OUTPUT {";
	wireframeShader += "	float4 hposition : SV_POSITION;";
	wireframeShader += "};";
	wireframeShader += "VS_OUTPUT VS(VS_INPUT input) {";
	wireframeShader += "	VS_OUTPUT OUT;";
	wireframeShader += "	OUT.hposition = mul(WVP, input.position);";
	wireframeShader += "	return OUT;\n\n";
	wireframeShader += "}";

	std::string vertex = wireframeShader;

	wireframeShader = "";
	wireframeShader += "cbuffer ConstantBuffer {";
	wireframeShader += "	float4x4 WVP;";
	wireframeShader += "}";
	wireframeShader += "";
	wireframeShader += "struct VS_OUTPUT {";
	wireframeShader += "float4 hposition : SV_POSITION;";
	wireframeShader += "};";
	wireframeShader += "";
	wireframeShader += "float4 FS(VS_OUTPUT input) : SV_TARGET{";
	wireframeShader += "return float4(0, 1, 0, 0);";
	wireframeShader += "}";


	std::string fragment = wireframeShader;

	////////////*

	std::string vstr2 = vertex;

	std::string fstr2 = fragment;
	{
		VS_blobRes = nullptr; // VS_blob would contain the binary compiled vertex shader program
		ComPtr<ID3DBlob> errorBlob = nullptr; // In case of error, this blob would contain the compilation errors
											  // We compile the source, the entry point is VS in our vertex shader, and we are using shader model 5 (d3d11)
		hr = D3DCompile((char*)vstr2.c_str(), (UINT)strlen((char*)vstr2.c_str()), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blobRes, &errorBlob);
		if (hr != S_OK) { // some error

			if (errorBlob) { // print the error if the blob is valid
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}
			// No binary data, return.
			if (VS_blobRes) {
				return;
			}
		}
		// With the binary blob now we create the Vertex Shader Object
		hr = D3D11Device->CreateVertexShader(VS_blobRes->GetBufferPointer(), VS_blobRes->GetBufferSize(), 0, &pVSRes);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			return;
		}
	}
	// Same for the Pixel Shader, just change the entry point, blob, etc, same exact method
	{
		FS_blobRes = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile((char*)fstr2.c_str(), (UINT)strlen((char*)fstr2.c_str()), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blobRes, &errorBlob);
		if (hr != S_OK) {
			if (errorBlob) {
				printf("errorBlob shader1[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (FS_blobRes) {
				return;
			}
		}

		hr = D3D11Device->CreatePixelShader(FS_blobRes->GetBufferPointer(), FS_blobRes->GetBufferSize(), 0, &pFSRes);
		if (hr != S_OK) {
			printf("Error Creating Pixel Shader\n");
			return;
		}
	}

	//D3D11DeviceContext->VSSetShader(pVSRes.Get(), 0, 0);
	//D3D11DeviceContext->PSSetShader(pFSRes.Get(), 0, 0);

	D3D11_INPUT_ELEMENT_DESC vertexDeclaration2[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTE"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = D3D11Device->CreateInputLayout(vertexDeclaration2, ARRAYSIZE(vertexDeclaration2), VS_blobRes->GetBufferPointer(), VS_blobRes->GetBufferSize(), &LayoutRes);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}
	D3D11DeviceContext->IASetInputLayout(LayoutRes.Get());
	D3D11_BUFFER_DESC bdesc = { 0 };

	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(CObject3D::CBufferRes);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBufferRes.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}
	// Set the constant buffer to the shader programs: Note that we use the Device Context to manage the resources
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBufferRes.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBufferRes.GetAddressOf());
	// We Bound the input layout
	for (auto i = meshes.begin(); i != meshes.end(); i++)
	{
		bdesc = { 0 };
		bdesc.ByteWidth = sizeof(CVertex) * (*i).bufferVertex.size();
		bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// Resource sub data is just a structure friendly to set our pointer to the vertex data
		D3D11_SUBRESOURCE_DATA subData = { (*i).bufferVertex.data(), 0, 0 };
		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &((*i).VB));
		if (hr != S_OK) {
			printf("Error Creating Vertex Buffer\n");
			return;
		}
		for (int index = 0; index < (*i).bufferIndexForTextures.size(); index++)
		{
			bdesc = { 0 };
			bdesc.ByteWidth = (*i).bufferIndexForTextures[index]->size() * sizeof(USHORT);
			bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			subData = { ((*i).bufferIndexForTextures[index]->data()), 0, 0 };

			hr = D3D11Device->CreateBuffer(&bdesc, &subData, &((*i).IB[index]));
			if (hr != S_OK) {
				printf("Error Creating Index Buffer\n");
				return;
			}
		}

		bdesc = { 0 };
		bdesc.ByteWidth = (*i).meshbufferIndex.size() * sizeof(USHORT);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subData = { (*i).meshbufferIndex.data(), 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &((*i).IBMesh));
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}
	}
	
	////////////////////////////////////////////////////////
	//SIGANTURE

	int Sig = 0;

	Sig |= Signature::HAS_NORMALS;
	Sig |= Signature::HAS_TEXCOORDS0;
	Sig |= Signature::HAS_TANGENTS;
	Sig |= Signature::HAS_BINORMALS;
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	

	if (meshes[0].infoTexture.textures[0].normal)
	{
		Sig |= Signature::NORMAL_MAP;
	}
	signature = Sig;
	flagShader = true;
	if (g_pBaseDriver->CreateShader(vstr, fstr, Sig) == -1)
	{
		flagShader = false;
	}
	else
	{
		Sig = signature;
		Sig |= Signature::LIGTHSHADOWMAP;
		g_pBaseDriver->CreateShader(vstr, fstr, Sig);
		Sig = signature;
		Sig |= Signature::GBUFF_PASS;
		g_pBaseDriver->CreateShader(vstr, fstr, Sig);
		Sig |= Signature::NO_LIGHT_AT_ALL;
		g_pBaseDriver->CreateShader(vstr, fstr, Sig);
	}

	if (flagShader)
	{

		CShaderD3DX *s = dynamic_cast<CShaderD3DX*>(g_pBaseDriver->GetShaderSig(signature));

		// We Bound the input layout
		D3D11DeviceContext->IASetInputLayout(s->Layout.Get());

		bdesc.Usage = D3D11_USAGE_DEFAULT;
		bdesc.ByteWidth = sizeof(CObject3D::CBuffer);
		bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
		if (hr != S_OK) {
			printf("Error Creating Buffer Layout\n");
			return;
		}
		// Set the constant buffer to the shader programs: Note that we use the Device Context to manage the resources

		std::map<std::string, int> list;
		int counterText = 0;
		for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
		{
			for (auto textures = (*mesh).infoTexture.textures.begin(); textures != (*mesh).infoTexture.textures.end(); textures++) {

				std::string tempString((*textures).diffuseName);
				auto iteradorFind = list.find(tempString);
				if (iteradorFind == list.end())
				{
					Texture	*tex = new TextureD3D;
					if (tex->LoadTexture((*textures).diffuseName) == -1) {
						printf("error textura no encontrada %s", tempString);
						if (tex->LoadTexture("cheker.tga") == -1) {
							delete tex;
							printf("error cheker no encotrada");
						}
						else
						{
							TexturesObj[counterText] = tex;
							list[tempString] = counterText;
							counterText++;
							(*textures).idDiffuse = list[tempString];
						}
					}
					else
					{
						TexturesObj[counterText] = tex;
						list[tempString] = counterText;
						counterText++;
						(*textures).idDiffuse = list[tempString];
					}
				}
				else
				{
					(*textures).idDiffuse = list[tempString];
				}
				if ((*textures).specular)
				{
					std::string tempString((*textures).specularName);
					auto iteradorFind = list.find(tempString);
					if (iteradorFind == list.end())
					{
						Texture	*tex = new TextureD3D;
						if (tex->LoadTexture((*textures).specularName) == -1) {
							delete tex;
							printf("error textura no encontrada %s", tempString.c_str());
							textures->specular = false;
						}
						else
						{
							TexturesObj[counterText] = tex;
							list[tempString] = counterText;
							counterText++;
							(*textures).idSpecular = list[tempString];
						}
					}
					else
					{
						(*textures).idSpecular = list[tempString];
					}
				}
				if ((*textures).gloss)
				{
					std::string tempString((*textures).glossName);
					auto iteradorFind = list.find(tempString);
					if (iteradorFind == list.end())
					{
						Texture	*tex = new TextureD3D;
						if (tex->LoadTexture((*textures).glossName) == -1) {
							delete tex;
							printf("error textura no encontrada %s", tempString);
							textures->gloss = false;
						}
						else
						{
							TexturesObj[counterText] = tex;
							list[tempString] = counterText;
							counterText++;
							(*textures).idGloss = list[tempString];
						}
					}
					else
					{
						(*textures).idGloss = list[tempString];
					}
				}
				if ((*textures).normal)
				{
					std::string tempString((*textures).normalName);
					auto iteradorFind = list.find(tempString);
					if (iteradorFind == list.end())
					{
						Texture	*tex = new TextureD3D;
						if (tex->LoadTexture((*textures).normalName) == -1) {
							delete tex;
							printf("error textura no encontrada %s", tempString);
							textures->normal = false;
						}
						else
						{
							TexturesObj[counterText] = tex;
							list[tempString] = counterText;
							counterText++;
							(*textures).idNormal = list[tempString];
						}
					}
					else
					{
						(*textures).idNormal = list[tempString];
					}
				}
			}
		}
	}

#endif

	transform = Identity();
	free(vsSourceP);
	free(fsSourceP);
	free(archivo);
	delete buffer1;


	Texture	*tex = new TextureD3D;
	tex->LoadTextureCubeMap("earth-cubemap.dds");
}

void CObject3D::Transform(float *t) {
	transform = t;
}

void CObject3D::Draw(float *t, float *vp) {

	if (t)
		transform = t;
#ifdef USING_GL_COMMON

	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;

	if (lights->flagWireFrame || shaderWireFrame == shaderID)
	{

		glUseProgram(shaderWireFrame);
		glUniformMatrix4fv(matWorldViewProjUniformLocSimple, 1, GL_FALSE, &WVP.m[0][0]);


		for (auto i = meshes.begin(); i != meshes.end(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, (*i).VB);
			glEnableVertexAttribArray(vertexAttribLocSimple);
			glVertexAttribPointer(vertexAttribLocSimple, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i).IBMesh);
			glDrawElements(GL_LINES, (*i).meshbufferIndex.size(), GL_UNSIGNED_SHORT, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vertexAttribLocSimple);
		glUseProgram(0);

		return;
	}

	glUseProgram(shaderID);

#ifdef	USE_DIFFUSE
	glUniform3f(PosCamera, lights->posCamera->x, lights->posCamera->y, lights->posCamera->z);
#endif
#ifdef USE_GLOBALLIGHT
	glUniform3f(DirectionGlobalLight, lights->dirGlobal.x, lights->dirGlobal.y, lights->dirGlobal.z);
	glUniform3f(ColorGlobalLight, lights->colorGlobal.x, lights->colorGlobal.y, lights->colorGlobal.z);
#endif
#ifdef	USE_POINTLIGHT
	glUniform3f(PositionPointLight, lights->posPoint.x, lights->posPoint.y, lights->posPoint.z);
	glUniform3f(ColorPointLight, lights->colorPoint.x, lights->colorPoint.y, lights->colorPoint.z);
#endif

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);


	

	for (auto i = meshes.begin(); i != meshes.end(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, (*i).VB);
		glEnableVertexAttribArray(vertexAttribLoc);
		glEnableVertexAttribArray(normalAttribLoc);

		if (uvAttribLoc != -1)
			glEnableVertexAttribArray(uvAttribLoc);
		if (binormalAttribLoc != -1)
			glEnableVertexAttribArray(binormalAttribLoc);
		if (tangenteAttribLoc != -1)
			glEnableVertexAttribArray(tangenteAttribLoc);

		glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
		glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));

		if (uvAttribLoc != -1)
			glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(32));
		if (binormalAttribLoc != -1)
			glVertexAttribPointer(binormalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(40));
		if (tangenteAttribLoc != -1)
			glVertexAttribPointer(tangenteAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(56));

		

		for (int index = 0; index < (*i).bufferIndexForTextures.size(); index++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, (*i).infoTexture.textures[index].idDiffuse);
			glUniform1i(diffuseLoc, 0);
			if ((*i).infoTexture.textures[index].normal)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, (*i).infoTexture.textures[index].idNormal);
				glUniform1i(normalLoc, 1);
			}
			if ((*i).infoTexture.textures[index].specular)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, (*i).infoTexture.textures[index].idSpecular);
				glUniform1i(specularLoc, 2);
			}
			if ((*i).infoTexture.textures[index].gloss)
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, (*i).infoTexture.textures[index].idGloss);
				glUniform1i(glossLoc, 3);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i).IB[index]);
			glDrawElements(GL_TRIANGLES, (*i).bufferIndexForTextures[index]->size(), GL_UNSIGNED_SHORT, 0);
		}
		
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}

	glUseProgram(0);
	glUseProgram(0);
#elif defined(USING_D3D11)
	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;
	CnstBufferRes.WVP = WVP;
	CnstBuffer.WVP = WVP;
	CnstBuffer.World = transform;
	CnstBuffer.LigthView = transform * lights->LigthView;
#ifdef	USE_DIFFUSE
	CnstBuffer.PosCamera = (*lights->posCamera);
#endif
#ifdef USE_GLOBALLIGHT
	CnstBuffer.DirectionGlobalLight = (lights->dirGlobal);
	CnstBuffer.ColorGlobalLight = (lights->colorGlobal);
#endif
#ifdef	USE_POINTLIGHT
	CnstBuffer.PositionPointLight = (lights->posPoint);
	CnstBuffer.ColorPointLight = (lights->colorPoint);
#endif

	UINT stride = sizeof(CVertex);
	UINT offset = 0;
	// We bound to use the vertex and pixel shader of this primitive

	if (lights->flagWireFrame || !flagShader)
	//if (true)

	{
		D3D11DeviceContext->VSSetShader(pVSRes.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(pFSRes.Get(), 0, 0);
		// Set the input layout to let the shader program know what kind of vertex data we have
		D3D11DeviceContext->IASetInputLayout(LayoutRes.Get());
		// We update the constant buffer with the current matrices
		D3D11DeviceContext->UpdateSubresource(pd3dConstantBufferRes.Get(), 0, 0, &CnstBufferRes, 0, 0);
		// Once updated the constant buffer we send them to the shader programs
		D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBufferRes.GetAddressOf());
		D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBufferRes.GetAddressOf());


		for (auto i = meshes.begin(); i != meshes.end(); i++)
		{

			// We let d3d that we are using our vertex and index buffers, they require the stride and offset

			D3D11DeviceContext->IASetVertexBuffers(0, 1, (*i).VB.GetAddressOf(), &stride, &offset);
			D3D11DeviceContext->IASetIndexBuffer((*i).IBMesh.Get(), DXGI_FORMAT_R16_UINT, 0);
			// Instruct to use triangle list
			D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			D3D11DeviceContext->DrawIndexed((*i).meshbufferIndex.size(), 0, 0);
		}
	}
	else
	{
		CShaderD3DX *s = 0;
		unsigned int sig = signature;
		sig |= gSig;
	
		s = dynamic_cast<CShaderD3DX*>(g_pBaseDriver->GetShaderSig(sig));
		
		D3D11DeviceContext->VSSetShader(s->pVS.Get(), 0, 0);
		D3D11DeviceContext->PSSetShader(s->pFS.Get(), 0, 0);
		D3D11DeviceContext->IASetInputLayout(s->Layout.Get());
		// We update the constant buffer with the current matrices
		D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
		// Once updated the constant buffer we send them to the shader programs
		D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
		D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
		for (int i = 0; i < 8; i++) {
			d3dxTextures[i] = dynamic_cast<TextureD3D*>(Textures[i]);
		}
		for (auto i = meshes.begin(); i != meshes.end(); i++)
		{

		// We let d3d that we are using our vertex and index buffers, they require the stride and offset
	
			D3D11DeviceContext->IASetVertexBuffers(0, 1, (*i).VB.GetAddressOf(), &stride, &offset);
		

				for (int index = 0; index < (*i).bufferIndexForTextures.size(); index++)
				{
					TextureD3D *texd3d = dynamic_cast<TextureD3D*>(TexturesObj[(*i).infoTexture.textures[index].idDiffuse]);
					D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
					D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

					if (i->infoTexture.textures[index].normal)
					{
						TextureD3D *texd3d2 = dynamic_cast<TextureD3D*>(TexturesObj[(*i).infoTexture.textures[index].idNormal]);
						D3D11DeviceContext->PSSetShaderResources(1, 1, texd3d2->pSRVTex.GetAddressOf());
						D3D11DeviceContext->PSSetSamplers(1, 1, texd3d2->pSampler.GetAddressOf());
					}
					if (i->infoTexture.textures[index].gloss)
					{
						TextureD3D *texd3d3 = dynamic_cast<TextureD3D*>(TexturesObj[(*i).infoTexture.textures[index].idGloss]);
						D3D11DeviceContext->PSSetShaderResources(2, 1, texd3d3->pSRVTex.GetAddressOf());
						D3D11DeviceContext->PSSetSamplers(2, 1, texd3d3->pSampler.GetAddressOf());
					}
					if (i->infoTexture.textures[index].specular)
					{
						TextureD3D *texd3d4 = dynamic_cast<TextureD3D*>(TexturesObj[(*i).infoTexture.textures[index].idSpecular]);
						D3D11DeviceContext->PSSetShaderResources(3, 1, texd3d4->pSRVTex.GetAddressOf());
						D3D11DeviceContext->PSSetSamplers(3, 1, texd3d4->pSampler.GetAddressOf());
					}
					if (lights->flagShadowMap)
					{
						D3D11DeviceContext->PSSetShaderResources(4, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
					}
					D3D11DeviceContext->IASetIndexBuffer((*i).IB[index].Get(), DXGI_FORMAT_R16_UINT, 0);
					// Instruct to use triangle list
					D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					D3D11DeviceContext->DrawIndexed((*i).bufferIndexForTextures[index]->size(), 0, 0);
				}	
		}
	}


	
		

#endif
}

void CObject3D::Destroy() {
#ifdef USING_GL_COMMON
	glDeleteProgram(shaderID);
#endif
}