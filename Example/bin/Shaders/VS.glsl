attribute highp vec4 Vertex;
attribute highp vec4 Normal;

varying highp vec3 vecTransformed;
varying highp vec4 vert;
#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
varying highp vec2 vecUVCoords;
#endif

#ifdef USE_GLOBALLIGHT
uniform highp vec3 DirectionGlobalLight;
uniform highp vec3 ColorGlobalLight;
varying highp vec3 light;
varying highp vec3 color;
#endif

#ifdef	USE_POINTLIGHT
uniform highp vec3 PositionPointLight;
uniform highp vec3 ColorPointLight;
varying highp vec3 posPoint;
varying highp vec3 colorPoint;
#endif

#ifdef	USE_DIFFUSE
uniform highp vec3 PositionCamera; 
varying highp vec3 positionCamera;
#endif

uniform highp mat4 WVP;
uniform highp mat4 World;
highp mat3 World3;

attribute highp vec4 Binormal;
attribute highp vec4 Tangente;
varying highp vec3 binormal1;
varying highp vec3 tangente1;
varying highp vec3 normal1;
varying  highp mat3 world3all;

void main(){

World3[0] = World[0].xyz;
World3[1] = World[1].xyz;
World3[2] = World[2].xyz;

binormal1 = normalize(World3 *Binormal.xyz);
tangente1 = normalize(World3 *Tangente.xyz);
normal1 = normalize(World3 *Normal.xyz);
world3all = World3;
#ifdef USE_GLOBALLIGHT
	light = DirectionGlobalLight;
	color = ColorGlobalLight;
#endif
#ifdef USE_POINTLIGHT
	posPoint = PositionPointLight;
	colorPoint = ColorPointLight;
#endif
#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
#endif
#ifdef USE_DIFFUSE
	positionCamera = PositionCamera;
#endif
	vecTransformed	= normalize(World3 *Normal.xyz);
	vert = World*Vertex;
	gl_Position = WVP*Vertex;
}