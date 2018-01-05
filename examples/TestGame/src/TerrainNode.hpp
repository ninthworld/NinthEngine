#pragma once

#include <memory>
#include <vector>
#include <NinthEngine\Application\Game.hpp>
#include "Terrain.hpp"

using namespace NinthEngine;

class TerrainNode {
public:
	TerrainNode(
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<VertexArray>& vertexArray,
		const std::shared_ptr<IndexBuffer>& indexBuffer,
		const std::shared_ptr<ConstantBuffer>& constantNode,
		const glm::vec2 location,
		const int lod,
		const glm::vec2 index);
	~TerrainNode();

	void update(const glm::vec3 cameraPos);

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	
	// Constant Buffers
	std::shared_ptr<ConstantBuffer> m_constantNode;

	// Index Buffers
	std::shared_ptr<IndexBuffer> m_indexBuffer;

	// Vertex Array
	std::shared_ptr<VertexArray> m_vertexArray;

	// Node
	glm::mat4 m_localMatrix;
	glm::vec3 m_worldPos;
	glm::vec2 m_location;
	glm::vec2 m_index;
	float m_size;
	int m_lod;
	bool m_leaf;

	// Children
	std::vector<std::unique_ptr<TerrainNode>> m_children;

};