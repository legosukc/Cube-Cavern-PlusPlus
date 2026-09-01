varying vec3 Normal;
varying vec2 Texcoord;
varying vec3 FragPos;

const vec3 LightPos = vec3(0.0, 1.0, 0.0);
const vec3 LightColor = vec3(1.0);

void main() {
    gl_FragColor = vec4((
        // ambient
		(0.4 * LightColor)

        // diffuse
	    + max(dot(
            normalize(Normal),
            normalize(LightPos - FragPos)
        ), 0.0) * LightColor

	) * LightColor, 1.0);
}