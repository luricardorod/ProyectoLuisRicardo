attribute highp vec4 Vertex;
attribute highp vec4 Normal;
varying highp vec4 vecTransformed;
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

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
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
	vecTransformed	= normalize(Normal*World);
	vert = World*Vertex;
	gl_Position = WVP*Vertex;
}