#version 400

layout(location=0) in vec2 in_position;

out vec2 vs_texCoord;

void main() {
	
	vs_texCoord = (vec2(in_position.x, in_position.y) + 1) / 2;
	gl_Position = vec4(in_position.xy, 0.0, 1.0);
}