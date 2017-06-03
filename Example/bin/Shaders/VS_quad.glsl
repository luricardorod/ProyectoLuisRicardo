attribute highp vec3 MyVertex;
attribute highp vec2 UV;
varying highp vec2 vecUVCoords;
uniform highp mat4 transform;
void main(){
	vecUVCoords = UV;

	gl_Position = transform*vec4(MyVertex,1.0);
}
