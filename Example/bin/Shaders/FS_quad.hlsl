cbuffer ConstantBuffer {
	float4x4 matTransform;
	float4x4 matTexture;
	float4 CameraPosition;
	float4x4 WVPInverse;
	float4 DirectionGlobalLight;
	float4 ColorGlobalLight;
	float4 PositionPointLight;
	float4 ColorPointLight;
};

struct VS_OUTPUT {
	float4 hposition : SV_POSITION;
	float2 texture0  : TEXCOORD;
	float4 Pos		 : TEXCOORD1;
	float4 PosCorner : VPOS;
};

SamplerState SS;
#ifdef LIGHT_SHADOW_MAP
Texture2D tex0 : register(t0);
float4 FS(VS_OUTPUT input) : SV_TARGET{
	return tex0.Sample(SS, input.texture0);
}
#else
#ifdef DEFERRED_PASS
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);
Texture2D tex4 : register(t4);
Texture2D tex5 : register(t5);

float4 FS(VS_OUTPUT input) : SV_TARGET{
	//return tex0.Sample(SS, input.texture0);
	float4 Final = float4(0.0,0.0,0.0,1.0);

	float4 Specular = float4(1.0, 1.0, 1.0, 1.0);
	float4 Lambert = float4(1.0, 1.0, 1.0, 1.0);
	float cutoff = 0.8;
	float rad = 300;

	float depth = tex4.Sample(SS, input.texture0);
	float4 position = CameraPosition + input.PosCorner*depth;

	float4 color = tex0.Sample(SS, input.texture0);
	float4 normalmap = tex1.Sample(SS, input.texture0);
	float4 normal = normalmap*2.0 - 1.0;
	float4 specularmap = tex2.Sample(SS, input.texture0);
	normal = normalize(normal);
	float3  EyeDir = normalize(CameraPosition - position).xyz;
	float dist = distance(PositionPointLight, position);
	if (dist < rad * 2) {

		float3	LightDir = normalize(PositionPointLight - position).xyz;
		float   att = 1.0;
		att = dot(normal.xyz, LightDir)*0.5 + 0.5;;
		att = pow(att, 2.0);
		att = clamp(att, 0.0, 1.0);
		Lambert *= color*att;

		float  specular = 0.0;
		float specIntesivity = 1.5;
		float shinness = 4.0;
		shinness = normal.a + shinness;

		float3 ReflectedLight = normalize(EyeDir + LightDir);
		specular = max(dot(ReflectedLight, normal.xyz)*0.5 + 0.5, 0.0);
		specular = pow(specular, shinness);

		specular *= att;
		specular *= specIntesivity;
		Specular *= specular;
		Specular.xyz *= specularmap.xyz;

		float d = max(dist - rad, 0.0);
		float denom = d / rad + 1.0;

		float attenuation = 1.0 / (denom*denom);

		attenuation = (attenuation - cutoff) / (1.0 - cutoff);
		attenuation = max(attenuation, 0.0);

		Final += Lambert*attenuation*2;
		Final += Specular*attenuation;
	}
	return Final + color*0.2;
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
