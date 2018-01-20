#version 400

//#define MIN_VIEW 0.99994
#define MIN_VIEW 0.49988
#define MAX_VIEW 0.99988

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform Window {
	vec4 windowSize;
};

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2DMS depthTexture;
uniform sampler2D skydomeTexture;

void main() {

	// Sample Textures
	vec3 color = texture(colorTexture, vs_texCoord).rgb;
	vec3 normal = texture(normalTexture, vs_texCoord).rgb;
	vec3 skydome = texture(skydomeTexture, vs_texCoord).rgb;
	
	// Depth
	float depth = 0;
	for (int i = 0; i < 4; ++i) {
		depth += texelFetch(depthTexture, ivec2(vs_texCoord * windowSize.xy), i).r;
	}
	depth /= 4.0;

	// Normalize Textures
	normal = normalize(normal * 2.0 - 1.0);

	// Calculate Lighting
	vec3 lightDir = normalize(vec3(1, 1, 1));
	float cosTheta = max(dot(normal, lightDir), 0.1);
	
	ps_color = vec4(color * cosTheta, 1);

	// Poor-man's stencil
	if (depth > MIN_VIEW) {
		ps_color = mix(ps_color, vec4(skydome, 1.0), clamp((depth - MIN_VIEW) / (1.0 - MAX_VIEW), 0, 1));
	}
}