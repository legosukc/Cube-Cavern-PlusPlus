#version 140

layout (std140) uniform Matrices {
	mat4 Projection, View;
}

uniform mat4 Model;

in vec3 aPos, aNormal;
in vec2 aTexcoord;

out vec3 Normal;
out vec2 Texcoord;

void main() {
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	Normal = aNormal;
	Texcoord = aTexcoord;
}