#version 330

in vec2 vs_texCoord;

out vec4 out_fragColor;

uniform sampler2D quadTexture;

void main(){
	
	vec3 texColor = texture(quadTexture, vs_texCoord).rgb;

	out_fragColor = vec4(texColor, 1.0) + vec4(0.5, 0, 0, 1.0);
}