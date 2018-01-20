#version 400

in vec2 vs_texCoord;
in float vs_blend;

layout(location=0) out vec4 ps_color;

layout(std140) uniform Skydome {
	vec4 skyColor;
	vec4 scale;
};

uniform sampler2D skyTexture;

void main() {
	
	vec4 skyTex = texture(skyTexture, vs_texCoord);
	
	ps_color = mix(skyTex, skyColor, vs_blend);
}