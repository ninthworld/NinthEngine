#pragma once

#include <GL\glew.h>

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