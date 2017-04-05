cbuffer ConstantBuffer {
	float4x4 matTransform;
	float offsetX;
	float offsetY;
	int numFrameX;
	int numFrameY;
};
	
Texture2D TextureRGB : register(t0);
SamplerState SS;


struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
};

float4 FS(VS_OUTPUT input) : SV_TARGET{
	
float4 color = TextureRGB.Sample(SS, input.texture0);
if (color.a < 0.5)
	discard;

return  color;
}
