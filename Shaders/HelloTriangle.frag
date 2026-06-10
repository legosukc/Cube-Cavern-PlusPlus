
//#extension GL_EXT_draw_buffers : require

//layout(location = 0) out vec4 outColor0;
//layout(location = 1) out vec4 outColor1;

uniform sampler2D Texture;

varying vec3 Normal;
varying vec2 Texcoord;

void main() {
	gl_FragColor = texture2D(Texture, Texcoord);
}