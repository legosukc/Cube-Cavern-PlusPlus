
uniform mat4 Model;

attribute vec3 aPos;
attribute vec2 aTexcoord;

varying vec2 Texcoord;

void main() {
	gl_Position = vec4(aPos, 1.0);
	Texcoord = aTexcoord;
}