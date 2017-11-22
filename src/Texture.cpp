#include "..\include\NinthEngine\Texture.hpp"

namespace NinthEngine {

Texture::Texture(const GLuint id, const int width, const int height) :
	id(id),
	width(width),
	height(height) {
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

} // namespace NinthEngine