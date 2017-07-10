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
	float offset = 1.0 / 1280.0;
	int pix = 2;

	float4 color = 0;
	for (int i = pix; i > 0; i--)
	{
		float4 tempcolor = 0;
		float2 temp = float2(input.texture0.r - i * offset, input.texture0.g);
		tempcolor = tex0.Sample(SS, temp);
		color += tempcolor / i;
	}

	for (int i = pix; i > 0; i--)
	{
		float4 tempcolor = 0;
		float2 temp = float2(input.texture0.r + i * offset, input.texture0.g);
		tempcolor = tex0.Sample(SS, temp);
		color += tempcolor / i;
	}
	color += tex0.Sample(SS, input.texture0);
	color = color / (pix * 2 + 1);

	return color;

	/*float2 temp = float2(input.texture0.r - 2 * offset, input.texture0.g);
	float4 color0 = tex0.Sample(SS, temp);
	temp.r = input.texture0.r - offset;
	float4 color1 = tex0.Sample(SS, temp);
	float4 color2 = tex0.Sample(SS, input.texture0);
	temp.r = input.texture0.r + offset;
	float4 color3 = tex0.Sample(SS, temp);

	temp.r = input.texture0.r + 2*offset;
	float4 color4 = tex0.Sample(SS, temp);
	float4 color = color0*.2 + color1*.4 + color2*.8 +color3*.4 + color4*.2;
	color = color * .25;
	return float4(.4,0,0,0);*/
}