varying highp vec2 vecUVCoords;

void main(){

	highp vec4 texel = vec4(vecUVCoords.x, vecUVCoords.y, .2, .2);

	gl_FragColor = texel;
}
