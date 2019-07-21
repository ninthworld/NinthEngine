#pragma once

#include "NinthEngine/Core.h"
#include "NinthEngine/Graphics/VertexArray.h"
#include "Mesh.h"

namespace NinthEngine {

struct NE_API ModelProps {
	VertexArray* vertexArray;
	std::vector<Mesh*> meshes;
	ModelProps(VertexArray* vertexArray = nullptr, const std::vector<Mesh*>& meshes = {})
		: vertexArray(vertexArray), meshes(meshes) {};
};

class NE_API Model {
public:
	Model(const ModelProps& modelProps)
		: m_vertexArray(modelProps.vertexArray), m_meshes(modelProps.meshes) {}

	virtual ~Model() {}

	inline VertexArray* getVertexArray() { return m_vertexArray; }
	inline void setVertexArray(VertexArray* vertexArray) { m_vertexArray = vertexArray; }

	inline std::vector<Mesh*>& getMeshes() { return m_meshes; }
	inline void addMesh(Mesh* mesh) { m_meshes.push_back(mesh); }
	inline void removeMesh(Mesh* mesh) {
		auto it = std::find(m_meshes.begin(), m_meshes.end(), mesh);
		if (it != m_meshes.end()) m_meshes.erase(it);
	}

private:
	VertexArray* m_vertexArray;
	std::vector<Mesh*> m_meshes;
};

}