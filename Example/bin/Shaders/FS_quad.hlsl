cbuffer ConstantBuffer {
	float4x4 matTransform;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
};

float4 FS(VS_OUTPUT input) : SV_TARGET{
float4 color = float4(input.texture0.x, input.texture0.y, .2, .2);

return  color;
}
