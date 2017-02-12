#include "CerdoGL.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <string>

void CerdoGL::Create() {
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

	char *archivo = file2string("Cerdo.X");
	
	int counter = 0;
	int sizeVertex;
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
					sizeVertex = std::atof(buffer1);
					vertices = new CVertex[sizeVertex];
					for (int i = 0; i < sizeVertex; i++)
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
					sizeIndex = (std::atof(buffer1) * 3);
					indices = new unsigned short[sizeIndex];
					for (int i = 0; i < sizeIndex; i++)
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
						indices[i+2] = numberConvert;
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
						indices[i-2] = numberConvert;
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
					for (int i = 0; i < sizeVertex; i++)
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
					for (int i = 0; i < sizeVertex; i++)
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
	}


	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeVertex * sizeof(CVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	D3DXMatrixIdentity(&transform);
}

void CerdoGL::Transform(float *t) {
	transform = t;
}

void CerdoGL::Draw(float *t, float *vp) {
	glUseProgram(shaderID);

	if (t)
		transform = t;

	D3DXMATRIX VP = D3DXMATRIX(vp);
	D3DXMATRIX WVP = transform*VP;

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));

	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(32));

	glDrawElements(GL_TRIANGLES, sizeIndex, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}

	glUseProgram(0);
}

void CerdoGL::Destroy() {
	glDeleteProgram(shaderID);
}