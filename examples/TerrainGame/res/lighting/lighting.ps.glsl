#version 400

in vec2 vs_texCoord;

out vec4 out_fragColor;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D positionTexture;

void main() {

	vec3 color = texture(colorTexture, vs_texCoord).rgb;
	vec3 normal = normalize(texture(normalTexture, vs_texCoord).rgb * 2.0 - 1.0);
	vec3 position = texture(positionTexture, vs_texCoord).rgb;
	
	vec3 lightDir = normalize(vec3(1, 1, 1));
	float cosTheta = max(dot(normal, lightDir), 0.1);

	out_fragColor = vec4(color * cosTheta, 1) + vec4(vec3(0) * position, 1);
}