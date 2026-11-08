#version 100

precision highp float;

varying vec3 Normal;

uniform vec4 Color;

void main() {
	gl_FragColor = Color;
}