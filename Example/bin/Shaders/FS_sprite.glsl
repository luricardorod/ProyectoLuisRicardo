varying highp vec2 vecUVCoords;
uniform highp sampler2D diffuse;


void main(){

	highp vec4 texel = texture2D(diffuse,vecUVCoords);
	if(texel.a < 0.5)
		discard;
	gl_FragColor = texel;
}