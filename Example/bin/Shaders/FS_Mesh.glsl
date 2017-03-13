uniform mediump sampler2D diffuse;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif
varying highp vec3 vecTransformed;
varying highp float lightIntensity;

void main()
{
#ifdef USE_TEXCOORD0
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb*lightIntensity;
#else
	highp vec3 vector = normalize(vecTransformed*0.5 + 0.5);
#endif
	gl_FragColor = vec4(vector,1.0);
}