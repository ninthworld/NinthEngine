#version 330

in vec3 vs_vertex;
in vec2 vs_texCoord;

out vec4 out_fragColor;

uniform sampler2D skyTexture;

void main(){
	
	out_fragColor = mix(vec4(texture(skyTexture, vs_texCoord).rgb, 1.0), vec4(0.57, 0.67, 0.87, 1.0), (1.0 - vs_vertex.y) * (1.0 - vs_vertex.y));
}