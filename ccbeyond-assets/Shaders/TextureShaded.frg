
uniform sampler2D Texture;

varying vec3 Normal;
varying vec2 Texcoord;

void main() {
	gl_FragColor = texture2D(Texture, Texcoord);
}