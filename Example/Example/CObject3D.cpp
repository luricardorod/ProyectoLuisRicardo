#include "CObject3D.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <string>

#define CHANGE_TO_RH 0
#define DEBUG_MODEL 0

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif


void CObject3D::Create(char * path)
{
#ifdef USING_OPENGL_ES
	char *vsSourceP = file2string("Shaders/VS_luzP.glsl");
	char *fsSourceP = file2string("Shaders/FS_luzP.glsl");
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS_Mesh.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Mesh.hlsl");
#endif
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	std::string Defines = "";

	Defines += "#define USE_NORMALS\n\n";
	Defines += "#define USE_TEXCOORD0\n\n";

	vstr = Defines + vstr;
	fstr = Defines + fstr;

#ifdef USING_OPENGL_ES
	shaderID = glCreateProgram();

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vstr.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fstr.c_str());

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");

	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc = glGetUniformLocation(shaderID, "World");

	lightGL = glGetUniformLocation(shaderID, "lightPoint");

	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
#endif

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
#ifdef USING_D3D11
	HRESULT hr;
	{
		VS_blob = nullptr; // VS_blob would contain the binary compiled vertex shader program
		ComPtr<ID3DBlob> errorBlob = nullptr; // In case of error, this blob would contain the compilation errors
												// We compile the source, the entry point is VS in our vertex shader, and we are using shader model 5 (d3d11)
		hr = D3DCompile(vsSourceP, (UINT)strlen(vsSourceP), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
		if (hr != S_OK) { // some error

			if (errorBlob) { // print the error if the blob is valid
				printf("errorBlob shader[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}
			// No binary data, return.
			if (VS_blob) {
				return;
			}
		}
		// With the binary blob now we create the Vertex Shader Object
		hr = D3D11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), 0, &pVS);
		if (hr != S_OK) {
			printf("Error Creating Vertex Shader\n");
			return;
		}
	}
	// Same for the Pixel Shader, just change the entry point, blob, etc, same exact method
	{
		FS_blob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		hr = D3DCompile(fsSourceP, (UINT)strlen(fsSourceP), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
		if (hr != S_OK) {
			if (errorBlob) {
				printf("errorBlob shader1[%s]", (char*)errorBlob->GetBufferPointer());
				return;
			}

			if (FS_blob) {
				return;
			}
		}

		hr = D3D11Device->CreatePixelShader(FS_blob->GetBufferPointer(), FS_blob->GetBufferSize(), 0, &pFS);
		if (hr != S_OK) {
			printf("Error Creating Pixel Shader\n");
			return;
		}
	}

	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);

	D3D11_INPUT_ELEMENT_DESC vertexDeclaration[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = D3D11Device->CreateInputLayout(vertexDeclaration, ARRAYSIZE(vertexDeclaration), VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), &Layout);
	if (hr != S_OK) {
		printf("Error Creating Input Layout\n");
		return;
	}

	// We Bound the input layout
	D3D11DeviceContext->IASetInputLayout(Layout.Get());
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(CObject3D::CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

	// Set the constant buffer to the shader programs: Note that we use the Device Context to manage the resources
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	bdesc = { 0 };
	bdesc.ByteWidth = sizeof(CVertex) * sumVertexSize;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// Resource sub data is just a structure friendly to set our pointer to the vertex data
	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };
	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}
	for (int i = 0; i < list.size(); i++)
	{
		bdesc = { 0 };
		bdesc.ByteWidth = bufferIndex[i].size() * sizeof(USHORT);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subData = { &bufferIndex[i][0], 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &IB[i]);
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}
	}
	// Same for the index buffer
	
	for (std::map<std::string, unsigned short>::iterator it = list.begin(); it != list.end(); ++it) {
		Texture	*tex = new TextureD3D;
		char *tempChar;
		tempChar = new char[(it->first.size() + 1)];
		memcpy(tempChar, it->first.c_str(), it->first.size() + 1);
		TexId[it->second] = tex->LoadTexture(tempChar);
		if (TexId[it->second] == -1) {
			delete tex;
		}
		Textures[it->second] = tex;;
		delete tempChar;
	}
	
#endif // USING_D3D11

#ifdef USING_OPENGL_ES
	//no ligting   busediffusemap diffusemap speclevel specularmap glossmap glossiness normalmap bflipgreenchanel
	for (std::map<std::string, unsigned short>::iterator it = list.begin(); it != list.end(); ++it) {
		Texture	*tex = new TextureGL;
		char *tempChar;
		tempChar = new char[(it->first.size() + 1)];
		memcpy(tempChar, it->first.c_str(), it->first.size() + 1);
		TexId[it->second] = tex->LoadTexture(tempChar);
		if (TexId[it->second] == -1) {
			delete tex;
		}
		delete tempChar;
	}


	//crear nuevo index buffers



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

#endif
	transform = Identity();
	delete[] vsSourceP;
	delete[] fsSourceP;
	//delete[] buffer1;
	//delete[] archivo;
}

void CObject3D::Transform(float *t)
{
	transform = t;
}

void CObject3D::Draw(float *t, float *vp)
{
	if (t)
		transform = t;
#ifdef USING_OPENGL_ES

	glUseProgram(shaderID);
	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;

	glUniform4f(lightGL, (*lightPosBase).x, (*lightPosBase).y , (*lightPosBase).z, (*lightPosBase).w);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexId[i]);
		glUniform1i(diffuseLoc, 0);

		glDrawElements(GL_TRIANGLES, bufferIndex[i].size(), GL_UNSIGNED_SHORT, 0);
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
#elif defined(USING_D3D11)

	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;
	CnstBuffer.WVP = WVP;
	CnstBuffer.World = transform;

	UINT stride = sizeof(CVertex);
	UINT offset = 0;
	// We bound to use the vertex and pixel shader of this primitive
	D3D11DeviceContext->VSSetShader(pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(pFS.Get(), 0, 0);
	// Set the input layout to let the shader program know what kind of vertex data we have
	D3D11DeviceContext->IASetInputLayout(Layout.Get());
	// We update the constant buffer with the current matrices
	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
	// Once updated the constant buffer we send them to the shader programs
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

	// We let d3d that we are using our vertex and index buffers, they require the stride and offset
	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	// Same for the index buffer
	for (int i = 0; i < list.size(); i++)
	{
		TextureD3D *texd3d = dynamic_cast<TextureD3D*>(Textures[i]);
		D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

		D3D11DeviceContext->IASetIndexBuffer(IB[i].Get(), DXGI_FORMAT_R16_UINT, 0);
		// Instruct to use triangle list
		D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Draw the primitive sending the number of indices
		D3D11DeviceContext->DrawIndexed(sizeIndex, 0, 0);
	}
#endif
}

void CObject3D::Destroy()
{
#ifdef USING_OPENGL_ES
	glDeleteProgram(shaderID);
#endif
}
