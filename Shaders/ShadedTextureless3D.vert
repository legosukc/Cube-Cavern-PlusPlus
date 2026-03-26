#version 140

layout (std140) uniform Matrices {
	mat4 Projection, View;
}

uniform mat4 Model;

in vec3 aPos, aNormal;
out vec3 Normal;

void main() {
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	Normal = aNormal;
}