#version 330

in vec2 vs_texCoord;

out vec4 out_fragColor;

uniform sampler2D skyTexture;

void main(){
	
	out_fragColor = vec4(texture(skyTexture, vs_texCoord).rgb, 1.0);
}