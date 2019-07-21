#pragma once

#include "NinthEngine/Core.h"
#include "NinthEngine/Graphics/Graphics.h"
#include "NinthEngine/Model/Model.h"

namespace NinthEngine {

class NE_API ResourceManager {
public:
	ResourceManager(Graphics* graphics);
	~ResourceManager();

	Texture* loadTexture(const std::string& file);
	Shader* loadShader(const std::string& vertexShader, const std::string& pixelShader);
	Model* loadModel(const std::string& file);

protected:
	Model* createModel(const ModelProps& props);
	Mesh* createMesh(const MeshProps& props);

	static std::string readFile(const std::string& file);

private:
	Graphics* m_graphics;
	std::vector<Model*> m_models;
	std::vector<Mesh*> m_meshes;
};

}