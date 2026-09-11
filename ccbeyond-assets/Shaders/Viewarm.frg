varying vec3 Normal;
varying vec2 Texcoord;
varying vec3 FragPos;

uniform sampler2D ArmTexture;

const vec3 LightPos = vec3(0.0, 1.0, 0.0);
const vec3 LightColor = vec3(1.0);

void main() {
    gl_FragColor = texture2D(ArmTexture, Texcoord) * vec4(LightColor * vec3(0.4) + max(dot(Normal, normalize(LightPos - FragPos)), 0.0), 1.0);
}