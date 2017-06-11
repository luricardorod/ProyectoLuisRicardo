#include "CQuad.h"
#include "CShaderD3DX.h"
#include <string>

#ifdef USING_D3D11
extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
#endif

void CQuad::Create(char * path)
{

	vertices[0] = { -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[1] = { -1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[2] = { 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[3] = { 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;
#ifdef USING_GL_COMMON

	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_quad.glsl");
	char *fsSourceP = file2string("Shaders/FS_quad.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "MyVertex");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");
	diffuseLoc = glGetUniformLocation(shaderID, "diffuse");
	matTransform = glGetUniformLocation(shaderID, "transform");


	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(quadVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#elif defined(USING_D3D11)
	SigBase = Signature::HAS_TEXCOORDS0;

	char *vsSourceP = file2string("Shaders/VS_quad.hlsl");
	char *fsSourceP = file2string("Shaders/FS_quad.hlsl");
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	int shaderID = g_pBaseDriver->CreateShader(vstr, fstr, SigBase);
	unsigned int Dest;

	Dest = SigBase | Signature::DEFERRED_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::GBUFF_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);


	Dest = SigBase | Signature::LIGTHSHADOWMAP;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	// We Bound the input layout

	CShaderD3DX* s = dynamic_cast<CShaderD3DX*>(g_pBaseDriver->GetShaderIdx(shaderID));

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(CQuad::CQuadBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = D3D11Device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Buffer Layout\n");
		return;
	}

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

	D3D11_SAMPLER_DESC sdesc;
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.MinLOD = 0;
	sdesc.MaxLOD = 0;
	D3D11Device->CreateSamplerState(&sdesc, pSampler.GetAddressOf());
#endif
	transform = Identity();
}

void CQuad::Transform(float * t)
{
	transform = t;
}

void CQuad::Draw(float * t, float * vp)
{
	if (t)
		transform = t;
#ifdef USING_GL_COMMON

	glUseProgram(shaderID);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(uvAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)0);
	glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*)12);

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

	CMatrix4D VP = CMatrix4D(vp);
	CnstBuffer.matTransform = transform;
	unsigned int sig = SigBase;
	sig |= gSig;
	if (sig&Signature::LIGTHSHADOWMAP) {
		float fTexOffsx = 0.5 + (0.5 / 1280);
		float fTexOffsy = 0.5 + (0.5 / 720);

		CMatrix4D matTexAdj(0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			fTexOffsx, fTexOffsy, 0.0f, 1.0f);

		CnstBuffer.matTexture = VP * matTexAdj;
	}
	
	CShaderD3DX * s = dynamic_cast<CShaderD3DX*>(g_pBaseDriver->GetShaderSig(sig));
	UINT stride = sizeof(quadVertex);
	UINT offset = 0;
	
	D3D11DeviceContext->VSSetShader(s->pVS.Get(), 0, 0);
	D3D11DeviceContext->PSSetShader(s->pFS.Get(), 0, 0);
	D3D11DeviceContext->IASetInputLayout(s->Layout.Get());
	// We update the constant buffer with the current matrices
	D3D11DeviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
	// Once updated the constant buffer we send them to the shader programs
	D3D11DeviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
	D3D11DeviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf()); \

	D3D11DeviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);
	//TextureD3D *texd3d = dynamic_cast<TextureD3D*>(texture);
	//D3D11DeviceContext->PSSetShaderResources(0, 1, texd3d->pSRVTex.GetAddressOf());
	//D3D11DeviceContext->PSSetSamplers(0, 1, texd3d->pSampler.GetAddressOf());
	D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
	for (int i = 0; i < 8; i++) {
		d3dxTextures[i] = dynamic_cast<TextureD3D*>(Textures[i]);
	}

	if (sig&Signature::DEFERRED_PASS) {
		D3D11DeviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetShaderResources(1, 1, d3dxTextures[1]->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetShaderResources(2, 1, d3dxTextures[2]->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetShaderResources(3, 1, d3dxTextures[3]->pSRVTex.GetAddressOf());
		D3D11DeviceContext->PSSetShaderResources(4, 1, d3dxTextures[4]->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::LIGTHSHADOWMAP) {
		D3D11DeviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
	}
	else {
		D3D11DeviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
	}


	D3D11DeviceContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11DeviceContext->DrawIndexed(6, 0, 0);


#endif
}

void CQuad::Destroy()
{
#ifdef USING_GL_COMMON
	glDeleteProgram(shaderID);
#endif
}

CQuad::~CQuad()
{
}
