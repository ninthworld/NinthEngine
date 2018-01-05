#version 330

in vec3 vs_normal;
in vec2 vs_texCoord;
in vec3 vs_light;

out vec4 out_fragColor;

uniform sampler2D textureColor;
uniform sampler2D textureNrm;

void main(){
	
	vec3 unitNormal = normalize(texture(textureNrm, vs_texCoord).rgb * 2.0 - 1.0);

	float theta = max(dot(unitNormal, normalize(vs_light)), 0.0);

	vec3 color = texture(textureColor, vs_texCoord).rgb;
	
	out_fragColor = vec4(color * theta, 1.0);
}