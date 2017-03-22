cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;
	float4 Light;
}

struct VS_INPUT{
    float4 position : POSITION;
	float4 normal   : NORMAL;
    float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	float4 hnormal   : NORMAL;
    float2 texture0  : TEXCOORD;
	float lightIntensity : LU;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
	OUT.hnormal = normalize( mul( World , input.normal ) );
    OUT.texture0 = input.texture0;
	OUT.lightIntensity = dot(Light, OUT.hnormal) / (length(Light)*length(OUT.hnormal))*2.0;
	if (OUT.lightIntensity > 1.0)
		OUT.lightIntensity = 1.0;
	else if (OUT.lightIntensity < 0.0)
		OUT.lightIntensity = 0.0;
    return OUT;
}
