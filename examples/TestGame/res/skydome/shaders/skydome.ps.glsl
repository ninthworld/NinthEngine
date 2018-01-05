#version 400

in vec2 vs_texCoord;
in float vs_blend;

out vec4 out_fragColor;

layout(std140) uniform Skydome {
	vec4 skyColor;
	vec4 scale;
};

uniform sampler2D skyTexture;

void main(){
	
	vec4 skyTex = texture(skyTexture, vs_texCoord);
	
	out_fragColor = mix(skyTex, skyColor, vs_blend);
}