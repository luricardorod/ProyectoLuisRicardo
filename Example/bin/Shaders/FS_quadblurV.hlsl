cbuffer ConstantBuffer {
	float4x4 matTransform;
	float4x4 matTexture;
	float4 CameraPosition;
	float4x4 WVPInverse;
	float4 DirectionGlobalLight;
	float4 ColorGlobalLight;
	float4 PositionPointLight;
	float4 ColorPointLight;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
	float4 Pos		 : TEXCOORD1;
	float4 PosCorner : VPOS;
};

SamplerState SS;
Texture2D tex0 : register(t0);
float4 FS(VS_OUTPUT input) : SV_TARGET{
	float offset = 1.0 / 720.0;
	int pix = 2;
	
	float4 color = 0;
	for (int i = pix; i > 0; i--)
	{
		float4 tempcolor = 0;
		float2 temp = float2(input.texture0.r, input.texture0.g - i * offset);
		tempcolor = tex0.Sample(SS, temp);
		color += tempcolor / i;
	}

	for (int i = pix; i > 0; i--)
	{
		float4 tempcolor = 0;
		float2 temp = float2(input.texture0.r, input.texture0.g + i * offset);
		tempcolor = tex0.Sample(SS, temp);
		color += tempcolor / i;
	}
	color += tex0.Sample(SS, input.texture0);
	color = color / (pix * 2 + 1);
	//color = tex0.Sample(SS, input.texture0);
	return color;
}