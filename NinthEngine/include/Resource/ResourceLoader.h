#pragma once

#include "NinthEngine/Core.h"
#include "NinthEngine/Graphics/Graphics.h"
#include "NinthEngine/Model/Model.h"

namespace NinthEngine {

class NE_API ResourceLoader {
protected:
	ResourceLoader(Graphics* graphics);

public:
	~ResourceLoader();

	Texture* loadTexture(const std::string& file);
	Shader* loadShader(const std::string& vertexShader, const std::string& pixelShader);
	Model* loadModel(const std::string& file);
	
	static ResourceLoader* create(Graphics* graphics);

	static std::string readFile(const std::string& file);

private:
	Graphics* m_graphics;
	std::vector<Model*> m_models;
};

}