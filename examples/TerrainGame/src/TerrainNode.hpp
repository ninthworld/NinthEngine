#pragma once

#include <memory>
#include <vector>
#include "Terrain.hpp"

using namespace NinthEngine;

class TerrainNode {
public:
	TerrainNode(
		const std::shared_ptr<Terrain>& root,
		const glm::vec2 location,
		const int lod,
		const glm::vec2 index);
	~TerrainNode();

	void update();

	void preRender();

	void renderTerrain();

	void renderGrass();

private:	
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
	AABB m_bounds;
	bool m_inBounds;

	// Children
	std::vector<std::unique_ptr<TerrainNode>> m_children;

	friend class Terrain;
};