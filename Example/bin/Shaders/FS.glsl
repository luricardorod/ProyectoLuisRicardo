uniform highp sampler2D diffuse;
uniform highp sampler2D normalLoc;

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

varying highp vec3 binormal1;
varying highp vec3 tangente1;
varying highp vec3 normal1;



void main(){

	highp mat3 ejes;
	ejes[0] = normalize(tangente1);
	ejes[1] = normalize(binormal1);
	ejes[2] = normalize(normal1);
	highp vec3 lucas = texture2D(normalLoc,vecUVCoords).rgb * 2.0 - 1.0;
	lucas.g = -lucas.g;
	lucas = normalize(ejes*lucas);
	highp float lightIntensity;
	lowp vec3 vector = vec3(0, 0, 0);
	lowp vec3 globalLight = vec3(0, 0, 0);
	lowp vec3 pointLight = vec3(0, 0, 0);
	lowp vec3 diffuseLight = vec3(0, 0, 0);
	highp vec3 colorDiffuse = vec3(1, 1, 1);

#ifdef	USE_DIFFUSE
	lightIntensity = pow(dot(normalize(PositionCamera.xyz - vert.xyz),normalize(reflect((vert.xyz - PositionPointLight.xyz), lucas.xyz)))*0.5+0.5,10.0);
	//lightIntensity = pow(dot(normalize(PositionPointLight.xyz - vert.xyz + PositionCamera.xyz - vert.xyz),lucas.xyz)*0.5+0.5,10.0);
	
	clamp(lightIntensity, 0.0, 1.0);
	diffuseLight = lightIntensity*colorDiffuse;

#endif

#if defined(USE_GLOBALLIGHT)
	lightIntensity = dot(normalize(DirectionGlobalLight.xyz), normalize(lucas.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	globalLight = lightIntensity*ColorGlobalLight;
#endif

#if defined(USE_POINTLIGHT)
	lightIntensity = 0.0;
	lightIntensity = dot( normalize(PositionPointLight.xyz - vert.xyz ), normalize(lucas.xyz));
	lightIntensity = lightIntensity*(100.0 / length(PositionPointLight.xyz - vert.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	pointLight = lightIntensity*ColorPointLight;
#endif
#if defined(USE_TEXCOORD0) 
	vector = texture2D(diffuse,vecUVCoords).rgb*.1 +texture2D(diffuse,vecUVCoords).rgb*pointLight + texture2D(diffuse,vecUVCoords).rgb*globalLight + texture2D(diffuse,vecUVCoords).rgb*diffuseLight;

#endif
	gl_FragColor = vec4(vector,1.0);
}