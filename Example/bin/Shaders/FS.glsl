uniform highp sampler2D diffuse;

varying highp vec4 vecTransformed;
varying highp vec4 vert;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif
#ifdef USE_GLOBALLIGHT
varying highp vec3 light;
varying highp vec3 color;
#endif

#ifdef	USE_DIFFUSE
varying highp vec3 positionCamera;
#endif

#ifdef	USE_POINTLIGHT
varying highp vec3 posPoint;
varying highp vec3 colorPoint;
#endif

void main(){
	highp float lightIntensity;
	lowp vec3 vector = vec3(0, 0, 0);
	lowp vec3 globalLight = vec3(0, 0, 0);
	lowp vec3 pointLight = vec3(0, 0, 0);
	lowp vec3 diffuseLight = vec3(0, 0, 0);
	highp vec3 colorDiffuse = vec3(1, 1, 1);

#ifdef	USE_DIFFUSE
	lightIntensity = pow(dot(normalize(positionCamera.xyz - vert.xyz),normalize(reflect((vert.xyz - posPoint.xyz), vecTransformed.xyz)))*0.5+0.5,10.0);
	lightIntensity = pow(dot(normalize(posPoint.xyz - vert.xyz + positionCamera.xyz - vert.xyz),vecTransformed.xyz),10.0);
	
	clamp(lightIntensity, 0.0, 1.0);
	diffuseLight = lightIntensity*colorDiffuse;

#endif

#if defined(USE_GLOBALLIGHT)
	lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	globalLight = lightIntensity*color;
#endif

#if defined(USE_POINTLIGHT)
	lightIntensity = 0.0;
	lightIntensity = dot( normalize(posPoint.xyz - vert.xyz ), normalize(vecTransformed.xyz));
	lightIntensity = lightIntensity*(100.0 / length(posPoint.xyz - vert.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	pointLight = lightIntensity*colorPoint;
#endif
#if defined(USE_TEXCOORD0) 
	vector = texture2D(diffuse,vecUVCoords).rgb*.1 +texture2D(diffuse,vecUVCoords).rgb*pointLight + texture2D(diffuse,vecUVCoords).rgb*globalLight + texture2D(diffuse,vecUVCoords).rgb*diffuseLight;
#else
	vector = normalize(vec3(vecTransformed)*0.5 + 0.5)*.3 +normalize(vec3(vecTransformed)*0.5 + 0.5)*pointLight + normalize(vec3(vecTransformed)*0.5 + 0.5)*globalLight;
#endif
	gl_FragColor = vec4(vector,1.0);
}