#pragma once

#include <NinthEngine\Shader.hpp>
#include <NinthEngine\Texture.hpp>

using namespace NinthEngine;

class SimpleShader : public Shader {
public:
	void init();
	void loadViewProjMatrix(const glm::mat4 viewProjMatrix);
	void loadModelMatrix(const glm::mat4 modelMatrix);
	void loadTexture(Texture *texture);
protected:
private:
};