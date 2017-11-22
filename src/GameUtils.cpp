#include "..\include\NinthEngine\GameUtils.hpp"
#include "..\include\NinthEngine\Texture.hpp"

#define GLEW_STATIC
#include <GL\glew.h>

namespace NinthEngine {

GameUtils::Exception::Exception(const std::string msg) :
	msg(msg) {
}

const char *GameUtils::Exception::what() {
	return msg.c_str();
}

const std::string GameUtils::readFile(const std::string file) {
	std::string out("");
	std::string line;
	std::ifstream in(file);
	if (in.is_open()) {
		while (std::getline(in, line)) {
			out += line + "\n";
		}
		in.close();
	}
	else {
		throw GameUtils::Exception("Failed to open file: " + file);
	}

	return out;
}

Texture *GameUtils::loadBMP(const std::string file) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char *data;

	FILE *in;
	fopen_s(&in, file.c_str(), "rb");
	if (!in) throw GameUtils::Exception("Failed to load BMP: " + file);
	if (fread(header, 1, 54, in) != 54) throw GameUtils::Exception("BMP header incorrect: " + file);
	if (header[0] != 'B' || header[1] != 'M') throw GameUtils::Exception("BMP header incorrect: " + file);

	dataPos = *(int*) &(header[0x0A]);
	imageSize = *(int*) &(header[0x22]);
	width = *(int*) &(header[0x12]);
	height = *(int*) &(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, in);
	fclose(in);

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete data;
	return new Texture(id, width, height);
}

} // namespace NinthEngine