#version 100

precision highp float;

varying vec2 Texcoord;

uniform sampler2D LavaTexture;
uniform sampler2D CurrentFrame;

uniform float Time;

void main() {
	vec2 SampleCoord = Texcoord + vec2(sin(gl_FragCoord.x + Time), cos(gl_FragCoord.y - Time));
	vec4 LavaSample = texture2D(LavaTexture, SampleCoord);

	gl_FragCoord = vec4(mix(texture2D(CurrentFrame, SampleCoord).rgb, LavaSample.rgb, LavaSample.a), 1.0);
}