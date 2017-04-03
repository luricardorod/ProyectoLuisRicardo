Skip to content
This repository
Search
Pull requests
Issues
Gist
 @luricardorod
 Sign out
 Unwatch 2
  Star 0
 Fork 0 luricardorod/ProyectoLuisRicardo
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Pulse  Graphs  Settings
Branch: master Find file Copy pathProyectoLuisRicardo/Example/Example/CObject3D.cpp
eeb73d4  4 days ago
 Daniel Enriquez * Fix file2string function, add up 1 byte more for the \0
1 contributor
RawBlameHistory     
755 lines (683 sloc)  22.6 KB
#include "CObject3D.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include <string>
#include <map>

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

struct infotex {
public:
	unsigned short numberOfTextures;
	std::vector<unsigned short> indicesTextures;
	std::vector<char*> namesTextures;
};

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
	std::vector<infotex> infoMeshTextures;
	std::vector<unsigned short> macroBufferIndex;

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
					tempSizeBufferVertex = std::atof(buffer1);
					offsetVertex = bufferVertex.size();
					bufferVertex.resize(bufferVertex.size() + tempSizeBufferVertex);
					for (int i = 0; i < tempSizeBufferVertex; i++)
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
						bufferVertex[counterBufferVertex].x = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						bufferVertex[counterBufferVertex].y = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						bufferVertex[counterBufferVertex].z = std::atof(buffer1);
						bufferVertex[counterBufferVertex].w = 1;
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
					tempSizeBufferIndex = (std::atof(buffer1) * 3);
					macroBufferIndex.resize(macroBufferIndex.size() + tempSizeBufferIndex);

					for (int i = 0; i < tempSizeBufferIndex; i++)
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

						macroBufferIndex[counterBufferIndex + 2] = std::atof(buffer1) + offsetVertex;
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
						macroBufferIndex[counterBufferIndex + 1] = std::atof(buffer1) + offsetVertex;

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
						macroBufferIndex[counterBufferIndex] = std::atof(buffer1) + offsetVertex;

						counter++;
						counterBufferIndex += 3;
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
					for (int i = 0; i < tempSizeBufferVertex; i++)
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
						bufferVertex[counterBufferVertex].nx = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						bufferVertex[counterBufferVertex].ny = std::atof(buffer1);

						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						bufferVertex[counterBufferVertex].nz = std::atof(buffer1);
						bufferVertex[counterBufferVertex].nw = 1;

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
					for (int i = 0; i < tempSizeBufferVertex; i++)
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
						bufferVertex[counterBufferVertex].s = std::atof(buffer1);
						counter++;

						positionBuffer = 0;
						while (archivo[counter] != ';')
						{
							buffer1[positionBuffer] = archivo[counter];
							positionBuffer++;
							counter++;
						}
						buffer1[positionBuffer] = '\0';
						bufferVertex[counterBufferVertex].t = std::atof(buffer1);
						counterBufferVertex++;
						counter++;
					}
					//enduvs
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

			int temporalCounter = 0;
			while (temporalCounter < tempContentInfotext.numberOfTextures)
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
					archivo[counter + 8] == 'M' &&
					archivo[counter + 9] == 'a' &&
					archivo[counter + 10] == 'p' &&
					archivo[counter + 11] == '"'
				) {

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
						temp[positionBuffer] = archivo[counter];
						positionBuffer++;
						counter++;
					}
					temp[positionBuffer] = '\0';
					tempContentInfotext.namesTextures.push_back(temp);
					temporalCounter++;
				}
			}
			infoMeshTextures.push_back(tempContentInfotext);
		}
		counter++;

	}
	//

	std::vector<std::string> list;

	for (auto it = infoMeshTextures.begin(); it != infoMeshTextures.end(); ++it) {
		for (auto it2 = it->namesTextures.begin(); it2 != it->namesTextures.end(); ++it2) {
			std::string tempString((*it2));
			auto iteradorFind = std::find(list.begin(), list.end(), tempString);
			if (iteradorFind == list.end())
			{
				list.push_back(tempString);
			}
		}
	}

	for (auto i = list.begin(); i != list.end(); i++)
	{
		std::vector<unsigned short> *temp;
		temp = new std::vector<unsigned short>;
		bufferIndexForText.push_back(temp);
	}
	unsigned short counterTemp;
	int counterIndexBuffer = 0;
	for (auto it = infoMeshTextures.begin(); it != infoMeshTextures.end(); ++it) {
		std::map <unsigned short, unsigned short> tempList;
		counterTemp = 0;
		for (auto it2 = it->namesTextures.begin(); it2 != it->namesTextures.end(); ++it2) {
			std::string tempString((*it2));
			auto iteradorFind = std::find(list.begin(), list.end(), tempString);
			unsigned short index = std::distance(list.begin(), iteradorFind);
			tempList[counterTemp] = index;
			counterTemp++;
		}
		for (int i = 0; i < it->indicesTextures.size(); i++)
		{
			bufferIndexForText[tempList[it->indicesTextures[i]]]->push_back(macroBufferIndex[counterIndexBuffer]);
			counterIndexBuffer++;
			bufferIndexForText[tempList[it->indicesTextures[i]]]->push_back(macroBufferIndex[counterIndexBuffer]);
			counterIndexBuffer++;
			bufferIndexForText[tempList[it->indicesTextures[i]]]->push_back(macroBufferIndex[counterIndexBuffer]);
			counterIndexBuffer++;
		}
	}
#ifdef USING_OPENGL_ES
	shaderID = glCreateProgram();
	char *vsSourceP = file2string("Shaders/VS.glsl");
	char *fsSourceP = file2string("Shaders/FS.glsl");
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS.hlsl");
	char *fsSourceP = file2string("Shaders/FS.hlsl");
#endif

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	std::string Defines = "";

#ifdef USE_TEXCOORD0
	Defines += "#define USE_TEXCOORD0\n\n";
#endif
#ifdef USE_GLOBALLIGHT
	Defines += "#define USE_GLOBALLIGHT\n\n";
#endif
#ifdef	USE_POINTLIGHT
	Defines += "#define USE_POINTLIGHT\n\n";
#endif
#ifdef	USE_DIFFUSE
	Defines += "#define USE_DIFFUSE\n\n";
#endif
	vstr = Defines + vstr;
	fstr = Defines + fstr;

#ifdef USING_OPENGL_ES

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
	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
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
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, bufferVertex.size() * sizeof(CVertex), bufferVertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (int i = 0; i < bufferIndexForText.size(); i++)
	{
		glGenBuffers(1, &IB[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*bufferIndexForText[i]).size() * sizeof(unsigned short), (*bufferIndexForText[i]).data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	int countertext = 0;
	for (auto i = list.begin(); i != list.end(); i++) {
		Texture	*tex = new TextureGL;
		char *tempChar;
		tempChar = new char[((*i).size() + 1)];
		memcpy(tempChar, (*i).c_str(), (*i).size() + 1);
		TexId[countertext] = tex->LoadTexture(tempChar);
		if (TexId[countertext] == -1) {
			delete tex;
		}
		countertext++;
		delete[]tempChar;
	}
#elif defined(USING_D3D11)
	HRESULT hr;
	{
		VS_blob = nullptr; // VS_blob would contain the binary compiled vertex shader program
		ComPtr<ID3DBlob> errorBlob = nullptr; // In case of error, this blob would contain the compilation errors
											  // We compile the source, the entry point is VS in our vertex shader, and we are using shader model 5 (d3d11)
		hr = D3DCompile((char*)vstr.c_str(), (UINT)strlen((char*)vstr.c_str()), 0, 0, 0, "VS", "vs_5_0", 0, 0, &VS_blob, &errorBlob);
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
		hr = D3DCompile((char*)fstr.c_str(), (UINT)strlen((char*)fstr.c_str()), 0, 0, 0, "FS", "ps_5_0", 0, 0, &FS_blob, &errorBlob);
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
	bdesc.ByteWidth = sizeof(CVertex) * bufferVertex.size();
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// Resource sub data is just a structure friendly to set our pointer to the vertex data
	D3D11_SUBRESOURCE_DATA subData = { bufferVertex.data(), 0, 0 };
	hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}
	for (int i = 0; i < list.size(); i++)
	{
		bdesc = { 0 };
		bdesc.ByteWidth = (*bufferIndexForText[i]).size() * sizeof(USHORT);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subData = { &(*bufferIndexForText[i])[0], 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &IB[i]);
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}
	}
	// Same for the index buffer

	int countertext = 0;
	for (auto i = list.begin(); i != list.end(); i++) {
		Texture	*tex = new TextureD3D;
		char *tempChar;
		tempChar = new char[((*i).size() + 1)];
		memcpy(tempChar, (*i).c_str(), (*i).size() + 1);
		TexId[countertext] = tex->LoadTexture(tempChar);
		if (TexId[countertext] == -1) {
			delete tex;
		}
		Textures[countertext] = tex;
		countertext++;
		delete[]tempChar;
	}
#endif
	transform = Identity();
	free(vsSourceP);
	free(fsSourceP);
	free(archivo);
	delete buffer1;
}

void CObject3D::Transform(float *t) {
	transform = t;
}

void CObject3D::Draw(float *t, float *vp) {

	if (t)
		transform = t;
#ifdef USING_OPENGL_ES

	glUseProgram(shaderID);
	CMatrix4D VP = CMatrix4D(vp);
	CMatrix4D WVP = transform*VP;
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

	glBindBuffer(GL_ARRAY_BUFFER, VB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(16));

	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), BUFFER_OFFSET(32));

	for (int i = 0; i < bufferIndexForText.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexId[i]);
		glUniform1i(diffuseLoc, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
		glDrawElements(GL_TRIANGLES, (*bufferIndexForText[i]).size(), GL_UNSIGNED_SHORT, 0);
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
	for (int i = 0; i < bufferIndexForText.size(); i++)
	{
		TextureD3D *texd3d = dynamic_cast<TextureD3D*>(Textures[i]);
		D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());

		D3D11DeviceContext->IASetIndexBuffer(IB[i].Get(), DXGI_FORMAT_R16_UINT, 0);
		// Instruct to use triangle list
		D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Draw the primitive sending the number of indices
		D3D11DeviceContext->DrawIndexed((*bufferIndexForText[i]).size(), 0, 0);
	}
#endif
}

void CObject3D::Destroy() {
#ifdef USING_OPENGL_ES
	glDeleteProgram(shaderID);
#endif
}

//#if defined(USE_GLOBALLIGHT) && defined(USE_TEXCOORD0) && defined(USE_POINTLIGHT)
//highp float lightIntensity;
//lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
//if (lightIntensity > 1.0)
//lightIntensity = 1.0;
//else if (lightIntensity < 0.0)
//	lightIntensity = 0.0;
//lowp vec3 vector = lightIntensity*color;
//
//lightIntensity = dot((vecTransformed.xyz - posPoint.xyz), vecTransformed.xyz) / (length(vecTransformed.xyz - posPoint.xyz)*length(vecTransformed.xyz));
//if (lightIntensity > 1.0)
//lightIntensity = 1.0;
//else if (lightIntensity < 0.0)
//	lightIntensity = 0.0;
//vector = texture2D(diffuse, vecUVCoords).rgb*.3 + texture2D(diffuse, vecUVCoords).rgb*lightIntensity*colorPoint + texture2D(diffuse, vecUVCoords).rgb*vector;
//
//#elif defined(USE_GLOBALLIGHT) && defined(USE_TEXCOORD0)
//highp float lightIntensity;
//lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
//if (lightIntensity > 1.0)
//lightIntensity = 1.0;
//else if (lightIntensity < 0.0)
//	lightIntensity = 0.0;
//lowp vec3 vector = texture2D(diffuse, vecUVCoords).rgb*.3 + texture2D(diffuse, vecUVCoords).rgb*lightIntensity*color;
//#elif defined(USE_POINTLIGHT) && defined(USE_TEXCOORD0)
//highp float lightIntensity;
//lightIntensity = 0.0;
//if (length(posPoint.xyz - vert.xyz) < 10.0) {
//	lightIntensity = dot(normalize(posPoint.xyz - vert.xyz), normalize(vecTransformed.xyz));
//}
//
//if (lightIntensity > 1.0)
//lightIntensity = 1.0;
//else if (lightIntensity < 0.0)
//	lightIntensity = 0.0;
//lowp vec3 vector = texture2D(diffuse, vecUVCoords).rgb*.3 + lightIntensity*colorPoint*texture2D(diffuse, vecUVCoords).rgb;
//#elif defined(USE_TEXCOORD0) 
//lowp vec3 vector = texture2D(diffuse, vecUVCoords).rgb;
//#else
//highp vec3 vector = normalize(vecTransformed*0.5 + 0.5);
//#endif
Contact GitHub API Training Shop Blog About
© 2017 GitHub, Inc. Terms Privacy Security Status Help