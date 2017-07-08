cbuffer ConstantBuffer {
	float4x4 matTransform;
	float4x4 matTexture;
	float4 CameraPosition;
	float4x4 WVPInverse;
};

struct VS_INPUT {
	float4 position : POSITION;
	float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
	float4 Pos		 : TEXCOORD1;
	float4 PosCorner : VPOS;
};

VS_OUTPUT VS(VS_INPUT input) {

	VS_OUTPUT OUT;
	OUT.hposition = mul(matTransform, float4(input.position));
	OUT.texture0 = input.texture0;

	OUT.Pos = OUT.hposition;
	OUT.PosCorner = mul(WVPInverse, float4(input.position.xy, 1.0, 1.0));
	OUT.PosCorner.xyz /= OUT.PosCorner.w;
	OUT.PosCorner = OUT.PosCorner - CameraPosition;
	return OUT;
}
