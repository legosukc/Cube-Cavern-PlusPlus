#version 140

layout (std140) uniform Matrices {
	mat4 Projection, View;
}

uniform mat4 Model;

attribute vec3 aPos, aNormal;
varying vec3 Normal;

void main() {
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	Normal = aNormal;
}