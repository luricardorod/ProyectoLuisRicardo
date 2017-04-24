uniform highp sampler2D diffuse;
uniform highp sampler2D specularLoc;
uniform highp sampler2D glossLoc;

#ifdef USE_NORMAL_TEXTURE
uniform highp sampler2D normalLoc;
#endif

#ifdef USE_GLOBALLIGHT
uniform highp vec3 DirectionGlobalLight;
uniform highp vec3 ColorGlobalLight;
#endif

#ifdef	USE_POINTLIGHT
uniform highp vec3 PositionPointLight;
uniform highp vec3 ColorPointLight;
#endif

#ifdef	USE_DIFFUSE
uniform highp vec3 PositionCamera;
#endif


varying highp vec4 vert;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

#ifdef USE_NORMAL_TEXTURE
varying highp vec3 binormal1;
varying highp vec3 tangente1;
#endif
varying highp vec3 normal1;

void main(){

#ifdef USE_NORMAL_TEXTURE

	highp mat3 ejes;
	ejes[0] = normalize(tangente1);
	ejes[1] = normalize(binormal1);
	ejes[2] = normalize(normal1);
	highp vec3 normalInWorld = texture2D(normalLoc,vecUVCoords).rgb * 2.0 - 1.0;
	normalInWorld.g = -normalInWorld.g;
	normalInWorld = normalize(ejes*normalInWorld);
#else
	highp vec3 normalInWorld = normalize(normal1);
#endif

	highp float lightIntensity;

	lowp vec3 vector = vec3(0, 0, 0);
	lowp vec3 globalLight = vec3(0, 0, 0);
	lowp vec3 pointLight = vec3(0, 0, 0);
	lowp vec3 diffuseLight = vec3(0, 0, 0);
	highp vec3 colorDiffuse = vec3(1, 1, 1);


#if defined(USE_TEXCOORD0)
	highp vec3 textureColor = texture2D(diffuse,vecUVCoords).rgb;
#endif

#ifdef	USE_DIFFUSE
	lightIntensity = pow(dot(normalize(-PositionCamera.xyz - vert.xyz),normalize(reflect((vert.xyz - PositionPointLight.xyz), normalInWorld.xyz)))*0.5+0.5,1.0);
	//lightIntensity = pow(dot(normalize(PositionPointLight.xyz - vert.xyz + PositionCamera.xyz - vert.xyz),normalInWorld.xyz),10.0);
	
	clamp(lightIntensity, 0.0, 1.0);
	diffuseLight = lightIntensity*colorDiffuse;

#endif

#if defined(USE_GLOBALLIGHT)
	lightIntensity = dot(normalize(DirectionGlobalLight.xyz), normalize(normalInWorld.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	globalLight = lightIntensity*ColorGlobalLight;
#endif

#if defined(USE_POINTLIGHT)
	lightIntensity = 0.0;
	lightIntensity = dot( normalize(PositionPointLight.xyz - vert.xyz ), normalize(normalInWorld.xyz));
	lightIntensity = lightIntensity*(100.0 / length(PositionPointLight.xyz - vert.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	pointLight = lightIntensity*ColorPointLight;
#endif
#if defined(USE_TEXCOORD0) 	// ESTAS ACCESANDO MUCHAS VECES AL SAMPLER, ESO ES COSTOSO Y NO ES NECESARIO, HAZLO SOLO UNA VEZ
	
	vector = textureColor *.1 + textureColor * pointLight + textureColor*globalLight + textureColor*diffuseLight;

#endif
	gl_FragColor = vec4(vector,1.0);
	//gl_FragColor =  texture2D(glossLoc,vecUVCoords);

}