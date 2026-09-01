varying vec2 Texcoord;

uniform sampler2D SlotTexture;

void main() {
    gl_FragColor = texture2D(SlotTexture, Texcoord);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}