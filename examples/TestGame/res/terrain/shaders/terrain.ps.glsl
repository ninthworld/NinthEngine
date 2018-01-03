#version 330

in vec2 vs_position;
in vec3 vs_normal;

out vec4 out_fragColor;

uniform sampler2D rockDiffuseTexture;
uniform sampler2D rockNormalTexture;

void main(){

	vec3 lightPos = vec3(1, 1, 1);

	vec3 diffuse = texture(rockDiffuseTexture, vs_position).rgb;
	vec3 normal = normalize(texture(rockNormalTexture, vs_position).rgb * 2.0 - 1.0);

	float theta = dot(normalize(normal + vs_normal), lightPos);
	theta = clamp(theta, 0.1, 1.0);

	out_fragColor = vec4(diffuse * theta, 1.0);
}