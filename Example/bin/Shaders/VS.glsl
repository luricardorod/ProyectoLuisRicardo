attribute highp vec4 Vertex;
attribute highp vec4 Normal;
#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
#endif
attribute highp vec4 Binormal;
attribute highp vec4 Tangente;


varying highp vec4 vert;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

uniform highp mat4 WVP;
uniform highp mat4 World;



varying highp vec3 binormal1;
varying highp vec3 tangente1;
varying highp vec3 normal1;
varying  highp mat3 world3all;

void main(){

highp mat3 World3;

World3[0] = World[0].xyz;
World3[1] = World[1].xyz;
World3[2] = World[2].xyz;

binormal1 = normalize(World3 *Binormal.xyz);
tangente1 = normalize(World3 *Tangente.xyz);
normal1 = normalize(World3 *Normal.xyz);


#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
#endif

	vert = World*Vertex;
	gl_Position = WVP*Vertex;
}
