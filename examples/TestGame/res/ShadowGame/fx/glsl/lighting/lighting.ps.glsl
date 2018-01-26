#version 400

in vec2 vs_texCoord;

layout(location=0) out vec4 ps_color;

layout(std140) uniform Sky {
	vec4 sunPosition;
};

layout(std140) uniform Camera {
	mat4 camViewProj;
	vec4 camPosition;
};

layout(std140) uniform CameraExt {
	mat4 camView;
	mat4 camProj;
	mat4 camInvView;
	mat4 camInvProj;
};

layout(std140) uniform ShadowMap {
	mat4 shadowCamViewProj[3];
	vec4 shadowCamPosition[3];
};

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;

#define Z_NEAR	0.1
#define Z_FAR	1000.0

float linearize(float depth) {
	return 2.0 * Z_NEAR * Z_FAR / (Z_FAR + Z_NEAR - (depth * 2.0 - 1.0) * (Z_FAR - Z_NEAR));
}

vec3 worldPosFromDepth(float depth) {
	vec4 ssPos = camInvProj * vec4(vs_texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
	vec4 wsPos = camInvView * ssPos;
	return wsPos.xyz / wsPos.w;
}

mat4 biasMatrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

void main() {

	vec3 lightDir = normalize(sunPosition.xyz);
	
	vec3 color = texture(colorTexture, vs_texCoord).rgb;
	vec3 normal = texture(normalTexture, vs_texCoord).rgb * 2.0 - 1.0;
	float depth = texture(depthTexture, vs_texCoord).r;
	vec3 position = worldPosFromDepth(depth);

	if (depth != 1.0) {
		float cosTheta = clamp(dot(normal, lightDir), 0, 1);

		float camDist = distance(position, camPosition.xyz);
		vec4 shadowCoord = biasMatrix * shadowCamViewProj * vec4(position, 1.0);
		float visibility = 1.0;
		if (camDist < 10.0) {
			if (texture(shadowMap0, shadowCoord.xy).r < shadowCoord.z - 0.005) {
				visibility = 0.0;
			}
		}
		else if (camDist < 50.0) {
			if (texture(shadowMap1, shadowCoord.xy).r < shadowCoord.z - 0.005) {
				visibility = 0.0;
			}
		}
		else {
			if (texture(shadowMap2, shadowCoord.xy).r < shadowCoord.z - 0.005) {
				visibility = 0.0;
			}
		}

		color *= clamp(cosTheta * visibility, 0.3, 1.0);
	}

	ps_color = vec4(color, 1);

	/*
	if (vs_texCoord.x < 0.2 && vs_texCoord.y < 0.2 * 16/9) {
		vec2 coord = vs_texCoord / vec2(0.2, 0.2*16/9);
		ps_color = vec4(vec3(linearize(texture(shadowDepthTexture, coord).r)), 1.0);
	}
	*/
}