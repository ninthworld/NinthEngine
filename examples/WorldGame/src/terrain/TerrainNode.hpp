#pragma once

#include <memory>
#include <vector>
#include <array>
#include <NinthEngine\Utils\MathUtils.hpp>
#include "Terrain.hpp"

class TerrainNode {
public:
	TerrainNode(
		const std::shared_ptr<Terrain>& root,
		const float localSize,
		const glm::vec2 localPos,
		const glm::vec2 index,
		const int lod);
	~TerrainNode();

	void update();
	void preRender();
	void render();

private:
	std::shared_ptr<Terrain> m_root;

	float m_localSize;
	glm::vec2 m_localPos;
	glm::vec2 m_worldPos;
	glm::vec2 m_index;
	int m_lod;
	bool m_leaf;

	bool m_visible;
	AABB m_bounds;

	NodeNeighborBit m_neighbors;
	std::array<int, 4> m_neighborLods;

	unsigned m_childCount;
	std::array<std::unique_ptr<TerrainNode>, 4> m_children;

	friend class Terrain;
};