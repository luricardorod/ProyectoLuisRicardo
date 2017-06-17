cbuffer ConstantBuffer{
  float4x4 WVP;
  float4x4 World;
  float4x4 LigthView;
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
Texture2D ShadowMap : register(t4);



SamplerState SS;
struct VS_OUTPUT {
  float4 hposition : SV_POSITION;
  float3 hnormal   : NORMAL;
  float2 texture0  : TEXCOORD;
  float3 hbinormal   : BINORMAL;
  float3 htangente   : TANGENTE;
  float4 vert      : VERTICE;
  float4 lu      : VERTICE2;
};
#ifdef G_BUFFER_PASS
  float4 FS( VS_OUTPUT input )  : SV_TARGET {
    return float4(1,0,0,1);
  }
#else
  #ifdef LIGHT_SHADOW_MAP
    struct FS_OUT {
      float4 color0 : SV_TARGET0;
    //  float  depth : SV_Depth;
    };
    FS_OUT FS(VS_OUTPUT input) {
      FS_OUT fout;
     // fout.depth = input.hposition.z / input.hposition.w;
      fout.color0 = TextureRGB.Sample(SS, input.texture0);
      return fout;
    }
  #else
    float4 FS( VS_OUTPUT input )  : SV_TARGET {
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

      #ifdef  USE_DIFFUSE
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

      float4 posLigth = input.vert;
	  posLigth.xyz /= posLigth.w;
      posLigth.xy = posLigth.xy*0.5+0.5;
	  posLigth.y = 1.0 - posLigth.y;
      float4 shadow = float4(1.0,1.0,1.0,1.0);
      float depthligth = ShadowMap.Sample(SS, posLigth).r;
      if (posLigth.x < 1 && posLigth.x  > 0&& posLigth.y < 1 && posLigth.y  > 0 && posLigth.w > 0.0) {
        if(posLigth.z > depthligth + 0.0001){
          shadow = float4(0.2,0.2,0.2, 1.0);
		}

      }
      float4 original = ColorGlobalLight;
      original *= float4(globalIntensity, 0);
      original *= color;
      float4 final = original + color;
      color = color * float4(globalIntensity, 0) + color * float4(pointIntensity, 0) + color *float4(specularIntensity, 0) + color *0.4;
	  color*=shadow;
	  float rd = ShadowMap.Sample(SS, input.texture0).r;
	//  color = float4(depthligth,depthligth,depthligth,1.0);
      return color;
    }
  #endif
#endif
