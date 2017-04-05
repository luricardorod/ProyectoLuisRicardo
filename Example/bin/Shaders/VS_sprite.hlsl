cbuffer ConstantBuffer {
	float4x4 matTransform;
	float offsetX;
	float offsetY;
	int numFrameX;
	int numFrameY;
};

struct VS_INPUT {
	float3 position : POSITION;
	float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input) {

	VS_OUTPUT OUT;
	OUT.hposition = mul(matTransform, float4(input.position, 1));
	OUT.texture0 = input.texture0;
	OUT.texture0.x += offsetX*float(numFrameX);
	OUT.texture0.y += offsetY*float(numFrameY);
	return OUT;
}