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

uniform highp mat4 WVP;
uniform highp mat4 World;


varying highp vec3 light;
varying highp vec3 color;
varying highp vec3 posPoint;
varying highp vec3 colorPoint;

uniform highp vec3 DirectionGlobalLight;
uniform highp vec3 ColorGlobalLight;
uniform highp vec3 PositionPointLight;
uniform highp vec3 ColorPointLight;

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
	light = DirectionGlobalLight;
	color = ColorGlobalLight;
	posPoint = PositionPointLight;
	colorPoint = ColorPointLight;
	gl_Position = WVP*Vertex;
}