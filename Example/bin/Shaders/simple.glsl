attribute highp vec4 Vertex;
uniform highp mat4 WVP;

void main(){

highp mat3 World3;
	gl_Position = WVP*Vertex;
}
