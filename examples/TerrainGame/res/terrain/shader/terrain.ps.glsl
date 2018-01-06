#version 400

in vec2 gs_texCoord;
in vec3 gs_tangent;

out vec4 out_fragColor;

uniform sampler2D normalmap;

void main(){
		
	vec3 normal = normalize(texture(normalmap, gs_texCoord).rgb * 2.0 - 1.0);

	out_fragColor = vec4(normal, 1.0);
}