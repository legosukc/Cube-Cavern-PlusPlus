#version 100

precision highp float;

varying vec3 Normal;
varying vec2 Texcoord;
varying vec3 FragPos;

const vec3 LightPos = vec3(0.0, 1.0, 0.0);
const vec3 LightColor = vec3(1.0);

void main() {
    gl_FragColor = vec4(LightColor * vec3(0.4) + max(dot(Normal, normalize(LightPos - FragPos)), 0.0), 1.0);
}