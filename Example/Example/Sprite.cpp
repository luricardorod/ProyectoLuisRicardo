#include "Sprite.h"
#include <string>

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void Sprite::Create(char *path) {
#ifdef USING_OPENGL_ES

	Texture	*tex = new TextureGL;
	tex->props = 4;
	idTexture = tex->LoadTexture(path);
	if (idTexture == -1) {
		delete tex;
		printf("error textura sprite %s", path);
	}
	else
	{
		uvx = fuvWidth / tex->x;
		uvy = fuvHeight / tex->y;
	}
#elif defined(USING_D3D11)
	Texture	*tex = new TextureD3D;
	if (tex->LoadTexture(path) == -1) {
		delete tex;
		printf("error textura no encontrada %s", path);
	}
	else
	{
		uvx = fuvWidth / tex->x;
		uvy = fuvHeight / tex->y;
		texture = tex;
	}

#endif
	vertices[0] = { fpositionX,  fpositionY, 0.0f , 0.0f, 0.0f };
	vertices[1] = { fpositionX, fpositionY - fheight, 0.0f , 0.0f, uvy };
	vertices[2] = { fpositionX + fwidth, fpositionY - fheight, 0.0f , uvx,  uvy };
	vertices[3] = { fpositionX + fwidth, fpositionY, 0.0f , uvx, 0.0f };

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;
#ifdef USING_OPENGL_ES

	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_sprite.glsl");
	char *fsSourceP = file2string("Shaders/FS_sprite.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "MyVertex");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");
	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
	offsetX = glGetUniformLocation(shaderID, "offsetX");
	offsetY = glGetUniformLocation(shaderID, "offsetY");
	numFrameX = glGetUniformLocation(shaderID, "numFrameX");
	numFrameY = glGetUniformLocation(shaderID, "numFrameY");
	matTransform = glGetUniformLocation(shaderID, "transform");

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(spriteVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
	char *vsSourceP = file2string("Shaders/VS_sprite.hlsl");
	char *fsSourceP = file2string("Shaders/FS_sprite.hlsl");
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	std::string Defines = "";


	vstr = Defines + vstr;
	fstr = Defines + fstr;

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
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	bdesc.ByteWidth = sizeof(Sprite::CSpriteBuffer);
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
		bdesc.ByteWidth = sizeof(CVertex) * 4;
		bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// Resource sub data is just a structure friendly to set our pointer to the vertex data
		D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };
		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &VB);
		if (hr != S_OK) {
			printf("Error Creating Vertex Buffer\n");
			return;
		}
		bdesc = { 0 };
		bdesc.ByteWidth = 6 * sizeof(USHORT);
		bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subData = { indices, 0, 0 };

		hr = D3D11Device->CreateBuffer(&bdesc, &subData, &IB);
		if (hr != S_OK) {
			printf("Error Creating Index Buffer\n");
			return;
		}

#endif
	transform = Identity();
	
}

void Sprite::Transform(float *t) {
	transform = t;
}

void Sprite::Draw(float *t, float *vp) {
	if (inumberFrames)
	{
		updateAnimation();
	}

	if (t)
		transform = t;
#ifdef USING_OPENGL_ES

	glUseProgram(shaderID);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(uvAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(spriteVertex), (void*)0);
	glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(spriteVertex), (void*)12);

	glUniform1f(offsetX, uvx);
	glUniform1f(offsetY, uvy);
	glUniform1i(numFrameX, numberFrameX);
	glUniform1i(numFrameY, numberFrameY);
	glUniformMatrix4fv(matTransform, 1, GL_FALSE, &transform.m[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, idTexture);

	glUniform1i(diffuseLoc, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(vertexAttribLoc);
	glDisableVertexAttribArray(uvAttribLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

#elif defined(USING_D3D11)
	
	CnstBuffer.matTransform = transform;
	CnstBuffer.offsetX = uvx;
	CnstBuffer.offsetY = uvy;
	CnstBuffer.numFrameX = numberFrameX;
	CnstBuffer.numFrameY = numberFrameY;


	UINT stride = sizeof(spriteVertex);
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
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf()); \

	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	TextureD3D *texd3d = dynamic_cast<TextureD3D*>(texture);
	D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
	D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
	// Instruct to use triangle list
	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);
		
#endif
}

void Sprite::Destroy() {
#ifdef USING_OPENGL_ES
	glDeleteProgram(shaderID);
#endif
}

void Sprite::Init(float width, float height, float positionX, float positionY, float numberFrames, float uvWidth, float uvHeight)
{
	fwidth = width / 640.0f;
	fheight = height / 360.0f;
	fpositionX = positionX / 640.0f -1;
	fpositionY = positionY / 360.0f -1;
	inumberFrames = numberFrames;
	fuvWidth = uvWidth;
	fuvHeight = uvHeight;
	if (numberFrames)
	{
		setAnimation(0, 0, .2f, 3);
		updateAnimation();
	}
}

void Sprite::setAnimation(int x, int y, float time, int frames)
{
	unica.ix = x;
	unica.iy = x;
	unica.ftime = time;
	unica.iframes = frames;
	unica.initTime = 0;
	DtTimer.Init();

}

void Sprite::updateAnimation()
{
	DtTimer.Update();
	unica.initTime += DtTimer.GetDTSecs();
	if (unica.initTime > unica.ftime)
	{
		unica.initTime = 0;
		numberFrameX++;
		if (unica.iframes < numberFrameX) {
			numberFrameX = 0;
		}
	}
}

Sprite::~Sprite()
{
}