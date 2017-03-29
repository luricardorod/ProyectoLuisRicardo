cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;
	float4 DirectionGlobalLight;
	float4 ColorGlobalLight;
	float4 PositionPointLight;
	float4 ColorPointLight;
	float4 PosCamera;
}

Texture2D TextureRGB : register(t0);

SamplerState SS;

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float3 hnormal   : NORMAL;
	float2 texture0  : TEXCOORD;
	float4 vert      : VERTICE;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float lightIntensity;
	float3 globalIntensity = {0,0,0};
	float3 pointIntensity = { 0,0,0 };

#if defined(USE_GLOBALLIGHT)
	lightIntensity = dot(normalize(DirectionGlobalLight.xyz), normalize(input.hnormal));
	clamp(lightIntensity, 0.0, 1.0);
	globalIntensity = lightIntensity*ColorGlobalLight.xyz;
#endif
#if defined(USE_POINTLIGHT)
	lightIntensity = dot(normalize(PositionPointLight.xyz - input.vert.xyz), normalize(input.hnormal));
	lightIntensity = lightIntensity*(100.0 / length(PositionPointLight.xyz - input.vert.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	pointIntensity = lightIntensity*ColorPointLight.xyz;
#endif
    float4 color;
	color = TextureRGB.Sample( SS, input.texture0)*.1 + TextureRGB.Sample(SS, input.texture0) * float4(globalIntensity,0) + TextureRGB.Sample(SS, input.texture0) * float4(pointIntensity, 0);
    return color;
}
