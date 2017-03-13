attribute highp vec4 Vertex;

#ifdef USE_NORMALS
attribute highp vec4 Normal;
#endif

#ifdef USE_TANGENTS
attribute highp vec4 Tangent;
#endif

#ifdef USE_BINORMALS
attribute highp vec4 Binormal;
#endif

#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
varying highp vec2 vecUVCoords;
#endif

varying highp vec3 vecTransformed;
uniform highp vec4 lightPoint;
varying highp vec4 light;
uniform highp mat4 WVP;
uniform highp mat4 World;

attribute highp vec4 verColor;


void main()
{
#ifdef USE_NORMALS
	vecTransformed	= normalize(mat3(World)*vec3(Normal));
#else
	vecTransformed	= normalize(mat3(World)*Vertex.xyz);
#endif

#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
#endif
	light = lightPoint;
	gl_Position = WVP*Vertex;
}