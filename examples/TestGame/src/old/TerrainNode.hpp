#pragma once

#include <memory>
#include <vector>
#include <NinthEngine\Application\Game.hpp>
#include "Terrain.hpp"

namespace NinthEngine {

class GraphicsContext;
class IndexBuffer;
class ConstantsBuffer;

} // namespace NinthEngine

using namespace NinthEngine;

class TerrainNode {
public:
	TerrainNode(
		const std::shared_ptr<GraphicsContext>& context,
		const std::vector<std::shared_ptr<IndexBuffer>>& indexBuffers,
		const std::shared_ptr<ConstantsBuffer>& constantsModel,
		const glm::vec3 worldPos,
		const glm::vec2 localPos,
		const unsigned level);
	~TerrainNode();

	void update(const glm::vec3 camPos);

	void render();

private:
	std::shared_ptr<GraphicsContext> m_context;
	std::vector<std::shared_ptr<IndexBuffer>> m_indexBuffers;
	std::shared_ptr<ConstantsBuffer> m_constantsModel;
	
	bool m_root;
	std::vector<std::shared_ptr<TerrainNode>> m_children;

	unsigned m_level;
	glm::vec3 m_worldPos;
	glm::vec2 m_localPos;
	float m_scale;

	unsigned m_sideFlag;

	glm::mat4 m_worldMatrix;

};