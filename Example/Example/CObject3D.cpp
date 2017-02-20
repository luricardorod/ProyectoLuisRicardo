#include "CObject3D.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <string>

void CObject3D::Create(char * path) {

	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS.glsl");
	char *fsSourceP = file2string("FS.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");

	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc = glGetUniformLocation(shaderID, "World");

	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");

	long sizeFile;
	char *archivo = file2string(path, &sizeFile);
	int counter = 0;
	int dif;

	float numberConvert;
	int positionBuffer;
	bool flag = true;
	char* buffer1 = new char[20];

	char* *pathsNames;
	char**listPathsNames[20];
	int numberOfTextures[20];

	CVertex* vertexTemp [20];
	int sizeVertex[20];
	unsigned short tempSizeIndex[20];
	int numberOfObjects = 0;
	char * pathsTextures[20];
	unsigned short	sizeMeshTexture[20];
	unsigned short*	indexTexture[20];

	unsigned short*	indicesTemp [20];
	unsigned short  sumIndex = 0;
	while (counter < sizeFile)
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
			char* temp = new char[20];
			buffer1 = temp;
			counter += 9;
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
					(sizeVertex[numberOfObjects]) = std::atof(buffer1);
					vertexTemp[numberOfObjects] = new CVertex[(sizeVertex[numberOfObjects])];
					for (int i = 0; i < (sizeVertex[numberOfObjects]); i++)
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
						vertexTemp[numberOfObjects][i].x = numberConvert;
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
						vertexTemp[numberOfObjects][i].y = numberConvert;
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
						vertexTemp[numberOfObjects][i].z = numberConvert;
						vertexTemp[numberOfObjects][i].w = 1;
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
					(tempSizeIndex[numberOfObjects]) = (std::atof(buffer1) * 3);
					indicesTemp[numberOfObjects] = new unsigned short[(tempSizeIndex[numberOfObjects])];
					for (int i = 0; i < (tempSizeIndex[numberOfObjects]); i++)
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
						indicesTemp[numberOfObjects][i + 2] = numberConvert + sumIndex;
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
						indicesTemp[numberOfObjects][i] = numberConvert + sumIndex;
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
						indicesTemp[numberOfObjects][i - 2] = numberConvert + sumIndex;
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
					for (int i = 0; i < (sizeVertex[numberOfObjects]); i++)
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
						vertexTemp[numberOfObjects][i].nx = numberConvert;
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
						vertexTemp[numberOfObjects][i].ny = numberConvert;
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
						vertexTemp[numberOfObjects][i].nz = numberConvert;
						vertexTemp[numberOfObjects][i].nw = 1;
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
					for (int i = 0; i < (sizeVertex[numberOfObjects]); i++)
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
						vertexTemp[numberOfObjects][i].s = numberConvert;
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
						vertexTemp[numberOfObjects][i].t = numberConvert;
						counter++;
					}
					//enduvs
					flag = false;
					sumIndex = sumIndex + sizeVertex[numberOfObjects];
					numberOfObjects++;
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
			char* temp = new char[20];
			buffer1 = temp;
			counter = counter + 12;
			positionBuffer = 0;
			while (archivo[counter] != ';')
			{
				buffer1[positionBuffer] = archivo[counter];
				positionBuffer++;
				counter++;
			}
			buffer1[positionBuffer] = '\0';
			numberOfTextures[numberOfObjects-1] = std::atoi(buffer1);
			counter++;
			positionBuffer = 0;
			while (archivo[counter] != ';')
			{
				buffer1[positionBuffer] = archivo[counter];
				positionBuffer++;
				counter++;
			}
			buffer1[positionBuffer] = '\0';
			sizeMeshTexture[numberOfObjects-1] = std::atoi(buffer1);
			indexTexture[numberOfObjects-1] = new unsigned short[(sizeMeshTexture[numberOfObjects-1])];
			counter++;
			for (int i = 0; i < sizeMeshTexture[numberOfObjects-1]; i++)
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
				numberConvert = std::atof(buffer1);
				indexTexture[numberOfObjects-1][i] = numberConvert;
			}
			pathsNames = new char*[numberOfTextures[numberOfObjects-1]];
			listPathsNames[numberOfObjects - 1] = &pathsNames[0];
			int temporalCounter = 0;
			while (temporalCounter < numberOfTextures[numberOfObjects-1])
			{
				counter++;
				if (
					archivo[counter] == '"' &&
					archivo[counter + 1] == 'd' &&
					archivo[counter + 2] == 'i' &&
					archivo[counter + 3] == 'f' &&
					archivo[counter + 4] == 'f' &&
					archivo[counter + 5] == 'u' &&
					archivo[counter + 6] == 's' &&
					archivo[counter + 7] == 'e' &&
					archivo[counter + 8] == 'M'
					) {

					counter = counter + 8;
					while (archivo[counter] != 92)
					{
						counter++;
					}
					counter += 2;
					char* temp = new char[20];
					buffer1 = temp;
					positionBuffer = 0;
					while (archivo[counter] != '"')
					{
						buffer1[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					buffer1[positionBuffer] = '\0';
					pathsNames[temporalCounter] = &buffer1[0];

					temporalCounter++;
				}
			}

		}
		counter++;

	}

	//map de texturas unicas
	unsigned short asignBuffer = 0;
	for (int i = 0; i < numberOfObjects; i++)
	{
		for (int r = 0; r < numberOfTextures[i]; r++)
		{
			printf("%s", listPathsNames[i][r]);
			std::string tempString(listPathsNames[i][r]);
			if (list.find(tempString) == list.end())
			{
				list[tempString] = asignBuffer;
				asignBuffer++;
			}
		}
	}

	int tempcount = 0;
	for (std::map<std::string, unsigned short>::iterator it = list.begin(); it != list.end(); ++it) {
		tex[tempcount] = new TextureGL;
		char *tempChar;
		tempChar = new char[(it->first.size() + 1)];
		memcpy(tempChar, it->first.c_str(), it->first.size() + 1);
		TexId[tempcount] = tex[tempcount]->LoadTexture(tempChar);
		if (TexId[tempcount] == -1) {
			delete tex;
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + tempcount);
			glBindTexture(GL_TEXTURE_2D, TexId[tempcount]);
		}
		tempcount++;
	}


	//Crear macro size and vertex buffer
	int sumVertexSize = 0;
	unsigned short sumIndexSize = 0;
	for (int i = 0; i < numberOfObjects; i++)
	{
		sumVertexSize += sizeVertex[i];
		sumIndexSize += tempSizeIndex[i];
	}

	vertices = new CVertex[(sumVertexSize)];
	indices = new unsigned short[(sumIndexSize)];

	int sizeTempVert = 0;
	int sizeTempInd = 0;

	for (int i = 0; i < numberOfObjects; i++)
	{
		memcpy(vertices + sizeTempVert, vertexTemp[i], sizeVertex[i] * sizeof(CVertex));
		sizeTempVert = sizeVertex[i];
		memcpy(indices + sizeTempInd, indicesTemp[i], tempSizeIndex[i] * sizeof(unsigned short));
		sizeTempInd = tempSizeIndex[i];
	}
	sizeIndex = sumIndexSize;
	//crear nuevo index buffers
	
	std::map<std::string, unsigned short>::iterator it;
	int counterIndexBuffer = 0;
	for (int i = 0; i < numberOfObjects; i++)
	{
		std::map <unsigned short, unsigned short> tempList;
		for (int k = 0; k < numberOfTextures[i]; k++)
		{
			std::string tempString(listPathsNames[i][k]);
			it = list.find(tempString);
			tempList[k] = it->second;
		}
		for (int j = 0; j < sizeMeshTexture[i]; j++)
		{
			bufferIndex[tempList[indexTexture[i][j]]].push_back(indices[counterIndexBuffer]);
			counterIndexBuffer++;
			bufferIndex[tempList[indexTexture[i][j]]].push_back(indices[counterIndexBuffer]);
			counterIndexBuffer++;
			bufferIndex[tempList[indexTexture[i][j]]].push_back(indices[counterIndexBuffer]);
			counterIndexBuffer++;
		}
	}

	
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sumVertexSize * sizeof(CVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	for (int i = 0; i < list.size(); i++)
	{
		glGenBuffers(1, &IB[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferIndex[i].size() * sizeof(unsigned short), &bufferIndex[i][0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	

	transform = Identity();
	delete vsSourceP;
	delete fsSourceP;
	//delete buffer1;
	delete archivo;
}

void CObject3D::Transform(float *t) {
	transform = t;
}

void CObject3D::Draw(float *t, float *vp) {
	glUseProgram(shaderID);

	if (t)
		transform = t;

	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);

	glEnableVertexAttribArray(vertexAttribLoc);

	if (normalAttribLoc != -1)
		glEnableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	if (normalAttribLoc != -1)
		glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));

	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(32));
	
	for (int i = 0; i < list.size(); i++)
	{

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TexId[i]);
		glUniform1i(diffuseLoc, 0);

		glDrawElements(GL_TRIANGLES, sizeIndex, GL_UNSIGNED_SHORT, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	
	if (normalAttribLoc != -1) {
		glDisableVertexAttribArray(normalAttribLoc);
	}

	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}

	glUseProgram(0);
}

void CObject3D::Destroy() {
	glDeleteProgram(shaderID);
}