#version 100

precision highp float;

varying vec2 Texcoord;

uniform sampler2D SlotTexture;

void main() {
    gl_FragColor = texture2D(SlotTexture, Texcoord); // vec4(1.0, 0.0, 0.0, 1.0);
}