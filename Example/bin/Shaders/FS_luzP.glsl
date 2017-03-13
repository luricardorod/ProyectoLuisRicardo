uniform mediump sampler2D diffuse;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif
varying highp vec3 vecTransformed;
varying highp vec4 light;
highp float lightIntensity;

void main()
{
	lightIntensity = dot(light.xyz, vecTransformed.xyz) / (length(light)*length(vecTransformed.xyz));
	if (lightIntensity > 1.0)
		lightIntensity = 1.0;
	else if (lightIntensity < 0.0)
		lightIntensity = 0.0;
#ifdef USE_TEXCOORD0
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb*lightIntensity;
#else
	highp vec3 vector = normalize(vecTransformed*0.5 + 0.5);
#endif
	gl_FragColor = vec4(vector,1.0);
}