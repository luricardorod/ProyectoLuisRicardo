attribute highp vec3 MyVertex;
attribute highp vec2 UV;
varying highp vec2 vecUVCoords;
uniform highp float offsetX;
uniform highp float offsetY;
uniform highp int numFrameX;
uniform highp int numFrameY;
uniform highp mat4 transform;
void main(){
	vecUVCoords = UV;
	vecUVCoords.x += offsetX*float(numFrameX);
	vecUVCoords.y += offsetY*float(numFrameY);

	gl_Position = transform*vec4(MyVertex,1.0);
}