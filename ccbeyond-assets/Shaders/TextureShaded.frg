#version 100

precision highp float;

varying vec3 Normal;
varying vec2 Texcoord;

uniform sampler2D Texture;

void main() {
	gl_FragColor = texture2D(Texture, Texcoord);
}