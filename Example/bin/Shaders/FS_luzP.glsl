uniform mediump sampler2D diffuse;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif
varying highp vec3 vecTransformed;

varying highp vec3 light;
varying highp vec3 color;
varying highp vec3 posPoint;
varying highp vec3 colorPoint;
highp float lightIntensity;

void main()
{
#if defined(USE_GLOBALLIGHT) && defined(USE_TEXCOORD0) && defined(USE_POINT_LIGHT)
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
	lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb*.1 + texture2D(diffuse,vecUVCoords).rgb*lightIntensity*color;
#elif defined(USE_POINT_LIGHT) && defined(USE_TEXCOORD0)
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