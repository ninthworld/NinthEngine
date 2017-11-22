#pragma once

#define GLEW_STATIC
#include <GL\glew.h>

namespace NinthEngine {

class Texture {
public:
	Texture(const GLuint id, const int width, const int height);
	~Texture();
	void bind();
protected:
private:
	GLuint id;
	int width;
	int height;
};

} // namespace NinthEngine