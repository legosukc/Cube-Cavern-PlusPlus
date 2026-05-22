#version 130

uniform sampler2D Texture;

in vec3 Normal;
in vec2 Texcoord;

out vec4 OutColor;

void main() {
	OutColor = texture(Texture, Texcoord);
}