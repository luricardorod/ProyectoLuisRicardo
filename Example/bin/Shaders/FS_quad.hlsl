cbuffer ConstantBuffer {
	float4x4 matTransform;
	float4x4 matTexture;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
	float4 vTexCoord   : TEXCOORD20;
};

SamplerState SS;
#ifdef LIGHT_SHADOW_MAP
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
float4 FS(VS_OUTPUT input) : SV_TARGET{
	float4 depth = tex1.Sample(SS, input.vTexCoord);
	//return tex0.Sample(SS, input.texture0);
	return depth;

}
#else
#ifdef DEFERRED_PASS
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);
Texture2D tex4 : register(t4);
float4 FS(VS_OUTPUT input) : SV_TARGET{
	//return tex0.Sample(SS, input.texture0);
	return float4(0,1,0,0);
}
#else
Texture2D tex0 : register(t0);
float4 FS(VS_OUTPUT input) : SV_TARGET{
	//return float4(0,1,1,0);
	float4 temp = tex0.Sample(SS, input.texture0);
	return 	float4(temp.r, temp.r, temp.r, 0);
}
#endif
#endif
