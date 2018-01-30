#version 400

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform CameraRay {
	mat4 camView;
	vec4 camPosition;
};

float cube(vec3 p) {
	return length(p) - 1.0;
	//return length(max(abs(p) - 1.0, 0.0));
}

float raymarch(vec3 o, vec3 r) {

	float t = 0.0;
	for (int i = 0; i < 32; ++i) {
		vec3 p = o + r * t;
		t += cube(p) * 0.5;
	}
	return t;
}

void main() {
	
	vec2 texCoord = vs_texCoord * 2.0 - 1.0;

	vec3 r = vec3(camView * vec4(normalize(vec3(texCoord, 1.0)), 1.0));
	vec3 o = camPosition.xyz;

	float t = raymarch(o, r);

	t = 1.0 / t;

	ps_color = vec4(vec3(t), 1.0);
}