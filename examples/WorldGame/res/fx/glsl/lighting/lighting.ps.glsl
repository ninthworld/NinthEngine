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

layout(std140) uniform Sky {
	vec4 sunPosition;
	vec4 skyColor;
};

layout(std140) uniform ShadowMap {
	mat4 shadowViewProj;
};

uniform sampler2DMS colorTexture;
uniform sampler2DMS normalTexture;
uniform sampler2DMS positionTexture;
uniform sampler2DMS depthTexture;
uniform sampler2D skydomeTexture;
uniform sampler2D shadowTexture;

mat4 biasMatrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

void main() {

	vec3 lightDir = normalize(sunPosition.xzy);
	
	float shadow = texture(shadowTexture, vs_texCoord).r;
	vec3 skydome = texture(skydomeTexture, vs_texCoord).rgb;

	// Multisampling
	float cosTheta = 0;
	vec3 color = vec3(0);
	for (int i = 0; i < SAMPLES; ++i) {
		float depth = texelFetch(depthTexture, ivec2(vs_texCoord * windowSize.xy), i).r;
		vec3 normal = texelFetch(normalTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb * 2.0 - 1.0;
		
		vec3 position = texelFetch(positionTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb;
		position = position * 2.0 - 1.0;
		position *= 1000.0;

		if (depth == 1.0) {
			color += skydome;
			cosTheta += 1.0;
		}
		else {
			vec3 diffuse = texelFetch(colorTexture, ivec2(vs_texCoord * windowSize.xy), i).rgb;
			diffuse *= max(dot(normal, lightDir), 0.3);
			
			mat4 depthBiasMVP = biasMatrix * shadowViewProj;

			float dist = distance(position, camPosition.xyz);
			if (dist > 500.0) {				
				diffuse = mix(skyColor.rgb, diffuse, 1-clamp((dist - 500) / 500, 0, 1));
			}

			color += diffuse;
		}
	}
	color /= SAMPLES;

	ps_color = vec4(color, 1);
	
	if (vs_texCoord.x < 0.25 && vs_texCoord.y < 0.25) {
		vec2 coord = vs_texCoord / 0.25;
		float depth = texture(shadowTexture, coord).r;
		float znear = 0.1;
		float zfar = 4096.0;
		float z = (2.0 * znear) / (zfar + znear - depth * (zfar - znear));
		ps_color = vec4(vec3(z), 1.0);
	}
}