
layout (std140) uniform Matrices {
	mat4 Projection, View;
}

uniform mat4 Model;

attribute vec3 aPos;
attribute vec3 aNormal;

varying vec3 Normal;

void main() {
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	Normal = aNormal;
}