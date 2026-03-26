#version 130

uniform vec4 Color;

in vec3 Normal;
out vec4 OutColor;

void main() {
	OutColor = Color;
}