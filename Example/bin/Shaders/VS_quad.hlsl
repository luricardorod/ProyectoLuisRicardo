cbuffer ConstantBuffer {
	float4x4 matTransform;
};

struct VS_INPUT {
	float4 position : POSITION;
	float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input) {

	VS_OUTPUT OUT;
	OUT.hposition = mul(matTransform, float4(input.position));
	OUT.texture0 = input.texture0;
	return OUT;
}
