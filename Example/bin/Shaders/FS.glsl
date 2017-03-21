uniform highp sampler2D diffuse;

varying highp vec3 vecTransformed;
#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif
#ifdef USE_GLOBALLIGHT
varying highp vec3 light;
varying highp vec3 color;
#endif

#ifdef	USE_POINTLIGHT
varying highp vec3 posPoint;
varying highp vec3 colorPoint;
#endif

void main(){
#if defined(USE_GLOBALLIGHT) && defined(USE_TEXCOORD0) && defined(USE_POINTLIGHT)
	highp float lightIntensity;
	lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	lowp vec3 vector = lightIntensity*color;

	lightIntensity = dot( (vecTransformed.xyz -posPoint.xyz), vecTransformed.xyz) / (length(vecTransformed.xyz -posPoint.xyz)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	vector = texture2D(diffuse,vecUVCoords).rgb*.1 + texture2D(diffuse,vecUVCoords).rgb*lightIntensity*colorPoint + texture2D(diffuse,vecUVCoords).rgb*vector;

#elif defined(USE_GLOBALLIGHT) && defined(USE_TEXCOORD0)
	highp float lightIntensity;
	lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb*.1 + texture2D(diffuse,vecUVCoords).rgb*lightIntensity*color;
#elif defined(USE_POINTLIGHT) && defined(USE_TEXCOORD0)
	highp float lightIntensity;
	lightIntensity = dot( (vecTransformed.xyz -posPoint.xyz), vecTransformed.xyz) / (length(vecTransformed.xyz -posPoint.xyz)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb*.1 + texture2D(diffuse,vecUVCoords).rgb*lightIntensity*colorPoint;
#elif defined(USE_TEXCOORD0) 
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb;
#else
	highp vec3 vector = normalize(vecTransformed*0.5 + 0.5);
#endif
	gl_FragColor = vec4(vector,1.0);
}