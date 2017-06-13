cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
	float4x4 LigthView;
	float4 DirectionGlobalLight;
	float4 ColorGlobalLight;
	float4 PositionPointLight;
	float4 ColorPointLight;
	float4 PosCamera;
}

struct VS_INPUT {
	float4 position : POSITION;
	float4 normal   : NORMAL;
	float2 texture0 : TEXCOORD;
	float4 binormal : BINORMAL;
	float4 tangente : TANGENTE;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float3 hnormal   : NORMAL;
	float2 texture0  : TEXCOORD;
	float3 hbinormal   : BINORMAL;
	float3 htangente   : TANGENTE;
	float4 vert      : VERTICE;
	float4 lu      : VERTICE2;
};

VS_OUTPUT VS(VS_INPUT input) {
#ifdef USE_NORMAL_TEXTURE
	float3x3 rotationTransform;
	rotationTransform[0] = World[0].xyz;
	rotationTransform[1] = World[1].xyz;
	rotationTransform[2] = World[2].xyz;
#endif

	VS_OUTPUT OUT;
	OUT.lu = input.position;
	OUT.vert = mul(LigthView, input.position);
	OUT.hposition = mul(WVP, input.position);
#ifdef USE_NORMAL_TEXTURE
	OUT.hnormal = normalize(mul(rotationTransform, input.normal.xyz));
	OUT.hbinormal = normalize(mul(rotationTransform, input.binormal.xyz));
	OUT.htangente = normalize(mul(rotationTransform, input.tangente.xyz));
#else
	OUT.hnormal = normalize(input.normal.xyz);
#endif

	OUT.texture0 = input.texture0;
	return OUT;
}
