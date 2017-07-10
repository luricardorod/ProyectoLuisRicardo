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
Texture2D tex1 : register(t1);

float4 FS(VS_OUTPUT input) : SV_TARGET{
	float4 color1 = tex0.Sample(SS, input.texture0);
	float4 color2 = tex1.Sample(SS, input.texture0);

	return color1 + color2;
}