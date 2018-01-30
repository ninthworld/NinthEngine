#version 400

// TODO: Fix this
#define USE_MSAA

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
	vec4 windowSize_;
};

#ifdef USE_MSAA
uniform sampler2DMS colorTexture;
uniform sampler2DMS normalTexture;
//uniform sampler2DMS depthTexture;
#else
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
//uniform sampler2D depthTexture;
#endif

void main() {

	vec4 windowSize = vec4(1600.0, 900.0, 0, 0);

	vec3 lightDir = normalize(vec3(0.8, 1.2, 0.6));
	
	// Multisampling
	float cosTheta = 0;
	vec3 color = vec3(0);
#ifdef USE_MSAA
	for (int i = 0; i < SAMPLES; ++i) {
		//float depth = texelFetch(depthTexture, ivec2(vs_texCoord * windowSize.xy), i).r;
		vec3 normal = texelFetch(normalTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb * 2.0 - 1.0;
		
		vec3 diffuse = texelFetch(colorTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb;
		diffuse *= max(dot(normal, lightDir), 0.3);
			
		color += diffuse;
	}
	color /= SAMPLES;
#else
	vec3 normal = texture(normalTexture, vs_texCoord).rgb * 2.0 - 1.0;
	vec3 diffuse = texture(colorTexture, vs_texCoord).rgb;
	diffuse *= max(dot(normal, lightDir), 0.3);
	color += diffuse;
#endif

	ps_color = vec4(color, 1);
}