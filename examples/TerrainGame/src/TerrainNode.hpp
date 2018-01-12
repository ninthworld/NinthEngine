#pragma once

#include <memory>
#include <vector>
#include "Terrain.hpp"

using namespace NinthEngine;

class TerrainNode {
public:
	TerrainNode(
		const std::shared_ptr<Terrain>& root,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<VertexArray>& vertexArray,
		const std::shared_ptr<Buffer>& constantNode,
		const glm::vec2 location,
		const int lod,
		const glm::vec2 index);
	~TerrainNode();

	void update(const glm::vec3 cameraPos);

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	
	// Constant Buffers
	std::shared_ptr<Buffer> m_constantNode;
	
	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Root
	std::shared_ptr<Terrain> m_root;

	// Node
	glm::mat4 m_localMatrix;
	glm::vec3 m_worldPos;
	glm::vec2 m_location;
	glm::vec2 m_index;
	float m_size;
	int m_lod;
	bool m_leaf;
	std::vector<bool> m_neighbors;

	// Children
	std::vector<std::unique_ptr<TerrainNode>> m_children;

	friend class Terrain;
};