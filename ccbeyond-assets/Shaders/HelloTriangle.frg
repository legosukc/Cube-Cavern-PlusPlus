#version 100

precision highp float;

//#extension GL_EXT_draw_buffers : require

//layout(location = 0) out vec4 outColor0;
//layout(location = 1) out vec4 outColor1;

varying vec3 Normal;
varying vec2 Texcoord;

uniform sampler2D Texture;

void main() {
	gl_FragColor = texture2D(Texture, Texcoord);
}