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
  float4 Pos		: TEXCOORD1;
};
#if defined(G_BUFFER_PASS)

struct FS_OUT {
	float4 color0 : SV_TARGET0;
	float4 color1 : SV_TARGET1;
	float4 color2 : SV_TARGET2;
	float4 color3 : SV_TARGET3;
	float  depth : SV_Depth;
};

FS_OUT FS(VS_OUTPUT input) {
	float4  color = float4(0.5, 0.5, 0.5, 1.0);
	float4  normal = float4(0.5, 0.5, 0.5, 1.0);
	float4  specular = float4(0.5, 0.5, 0.5, 1.0);
	float4  reflect = float4(0.5, 0.5, 0.5, 1.0);

	float specIntesivity = 0.8;
	float shinness = 2.0;

	color = TextureRGB.Sample(SS, input.texture0);

	normal.xyz = normalize(input.hnormal).xyz;
	float3 normalTex = NormalRGB.Sample(SS, input.texture0).xyz;
	normalTex = normalTex*float3(2.0, 2.0, 2.0) - float3(1.0, 1.0, 1.0);
	normalTex = normalize(normalTex);
	normalTex.g = -normalTex.g;
	float3 tangent = normalize(input.htangente).xyz;
	float3 binormal = normalize(input.hbinormal).xyz;
	float3x3	TBN = float3x3(tangent, binormal, normal.xyz);
	normal.xyz = mul(normalTex, TBN);
	normal.xyz = normalize(normal.xyz)*0.5 + 0.5;
	normal.xyz = normal.xyz*0.5 + 0.5;

	specular.rgb = SpecularRGB.Sample(SS, input.texture0).rgb;

	shinness = GlossRGB.Sample(SS, input.texture0).r + shinness;

	FS_OUT fout;
	fout.color0.rgb = color.rgb;
	fout.color0.a = specIntesivity;

	fout.color1.rgb = normal.rgb;
	fout.color1.a = shinness;

	fout.color2.rgb = specular.rgb;
	fout.color2.a = shinness;

	fout.color3 = float4(1.0, 0.0, 1.0, 1.0);
	fout.color3 = float4(0.0, 0.0, 0.0, 1.0);
	fout.color3 = float4(0.0, 0.0, 1.0, 1.0);
  fout.depth = input.Pos.z / PosCamera.y;
  fout.color0 = TextureRGB.Sample(SS, input.texture0);
  #if defined(NORMAL_MAP)
    fout.color1 = NormalRGB.Sample(SS, input.texture0);
  #endif
  fout.color2 = GlossRGB.Sample(SS, input.texture0);
  //fout.color3 = SpecularRGB.Sample(SS, input.texture0);
  fout.color3 = input.Pos;

	return fout;
}
#else
struct FS_OUT {
	float4 color0 : SV_TARGET0;
	float  depth : SV_Depth;
};
FS_OUT FS( VS_OUTPUT input ) : SV_TARGET  {
	float lightIntensity;
	float3 globalIntensity = {0,0,0};
	float3 pointIntensity = { 0,0,0 };
	float3 specularIntensity = { 0,0,0 };

#ifdef USE_NORMAL_TEXTURE
	float3x3 ejes;
	ejes[0] = normalize(input.htangente);
	ejes[1] = normalize(input.hbinormal);
	ejes[2] = normalize(input.hnormal);
	float4 norRGB = NormalRGB.Sample(SS, input.texture0);
	float3 newNormal = norRGB.rgb  *2.0 -1.0;
	newNormal.g = -newNormal.g;
	newNormal = normalize(mul(ejes, newNormal));
#else
	float3 newNormal = input.hnormal;
#endif

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
	float4 original = ColorGlobalLight;
	original *= float4(globalIntensity, 0);
	original *= color;
	float4 final = original + color;
	color = color * float4(globalIntensity, 0) + color * float4(pointIntensity, 0) + color *float4(specularIntensity, 0) + color *0.5;
	//color = color * float4(globalIntensity, 0);
  //color = float4(newNormal, 0);
  FS_OUT fout;
  #if defined(LIGHT_SHADOW_MAP)
    fout.depth = input.hposition.z  / PositionPointLight.y;
  #else
    fout.depth = input.hposition.z  / PosCamera.y;
  #endif

  fout.color0 = color;
	return  fout;
}
#endif
