
uniform sampler2D YoyoTexture;

varying vec2 Texcoord;

void main() {
	gl_FragColor = texture2D(YoyoTexture, Texcoord);
}