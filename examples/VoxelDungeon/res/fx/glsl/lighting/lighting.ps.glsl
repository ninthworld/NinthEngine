#version 400

#define MIN_VIEW 0.99984
#define MAX_VIEW 0.99988

#define SAMPLES	4

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform Window {
	vec4 windowSize;
};

uniform sampler2DMS colorTexture;
uniform sampler2DMS normalTexture;
uniform sampler2DMS depthTexture;

void main() {

	vec3 lightDir = normalize(vec3(0.8, 1.2, 0.6));
	
	// Multisampling
	float cosTheta = 0;
	vec3 color = vec3(0);
	for (int i = 0; i < SAMPLES; ++i) {
		float depth = texelFetch(depthTexture, ivec2(vs_texCoord * windowSize.xy), i).r;
		vec3 normal = texelFetch(normalTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb * 2.0 - 1.0;
		
		//vec3 position = texelFetch(positionTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb;

		vec3 diffuse = texelFetch(colorTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb;
		diffuse *= max(dot(normal, lightDir), 0.3);
			
		color += diffuse;
	}
	color /= SAMPLES;

	//color += vec3(0.3, 0.1, 0.3);
	//color *= vec3(1.0, 1.3, 1.0);
	color = 1.0 - exp(-1.0 * color);

	ps_color = vec4(color, 1);
}