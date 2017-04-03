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
Texture2D NormalRGB : register(t1);
Texture2D GlossRGB : register(t2);
Texture2D SpecularRGB : register(t3);


SamplerState SS;

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float3 hnormal   : NORMAL;
	float2 texture0  : TEXCOORD;
	float3 hbinormal   : BINORMAL;
	float3 htangente   : TANGENTE;
	float4 vert      : VERTICE;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float lightIntensity;
	float3 globalIntensity = {0,0,0};
	float3 pointIntensity = { 0,0,0 };
	float3 specularIntensity = { 0,0,0 };


	float3x3 ejes;
	ejes[0] = normalize(input.htangente);
	ejes[1] = normalize(input.hbinormal);
	ejes[2] = normalize(input.hnormal);
	float4 norRGB = NormalRGB.Sample(SS, input.texture0);
	float3 newNormal = norRGB.rgb  *2.0 -1.0;
	newNormal.g = -newNormal.g;
	newNormal = normalize(mul(ejes, newNormal));

#ifdef	USE_DIFFUSE
	lightIntensity = pow(dot(normalize(PosCamera.xyz - input.vert.xyz), normalize(reflect((input.vert.xyz - PositionPointLight.xyz), newNormal.xyz)))*0.5 + 0.5, 10.0);
	//lightIntensity = pow(dot(normalize(PositionPointLight.xyz - vert.xyz + PositionCamera.xyz - vert.xyz),lucas.xyz)ss,10.0);

	clamp(lightIntensity, 0.0, 1.0);
	specularIntensity = lightIntensity*SpecularRGB.Sample(SS, input.texture0).rgb;

#endif
#if defined(USE_GLOBALLIGHT)
	lightIntensity = dot(normalize(DirectionGlobalLight.xyz), newNormal);
	clamp(lightIntensity, 1.0, 1.0);
	globalIntensity = lightIntensity*ColorGlobalLight.xyz;
#endif
#if defined(USE_POINTLIGHT)
	lightIntensity = dot(normalize(PositionPointLight.xyz - input.vert.xyz), newNormal);
	lightIntensity = lightIntensity*(100.0 / length(PositionPointLight.xyz - input.vert.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	pointIntensity = lightIntensity*ColorPointLight.xyz;
#endif
    float4 color = TextureRGB.Sample(SS, input.texture0);
	color = color * float4(globalIntensity, 0) + color * float4(pointIntensity, 0) + color *float4(specularIntensity, 0) + color *0.1;
	return  color;
}
