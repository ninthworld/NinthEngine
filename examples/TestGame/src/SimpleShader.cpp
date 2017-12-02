#include "SimpleShader.hpp"
#include <NinthEngine\GameUtils.hpp>

void SimpleShader::init() {
	addShader(GameUtils::readFile("res/shaders/simple.vs.glsl"), GL_VERTEX_SHADER);
	addShader(GameUtils::readFile("res/shaders/simple.fs.glsl"), GL_FRAGMENT_SHADER);
	compileShader();

	addUniform("viewProjMatrix");
	addUniform("modelMatrix");
	addUniform("texture0");
}

void SimpleShader::loadViewProjMatrix(const glm::mat4 viewProjMatrix) {
	setUniform("viewProjMatrix", viewProjMatrix);
}

void SimpleShader::loadModelMatrix(const glm::mat4 modelMatrix) {
	setUniform("modelMatrix", modelMatrix);
}

void SimpleShader::loadTexture(Texture *texture) {
	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	setUniform("texture0", 0);
}