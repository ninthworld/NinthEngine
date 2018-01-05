#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\IndexBuffer.hpp>
#include <NinthEngine\Render\ConstantsBuffer.hpp>
#include "TerrainNode.hpp"

namespace {

static const unsigned maxSize = 1024;
static const unsigned maxLevel = 8;

const bool inRange(const glm::vec3 camPos, const glm::vec3 worldPos, const float scale) {
	return ((abs(camPos.x - worldPos.x) + abs(camPos.z - worldPos.z)) < (scale * 2.0f + 32.0f));
}

} // namespace

TerrainNode::TerrainNode(
	const std::shared_ptr<GraphicsContext>& context,
	const std::vector<std::shared_ptr<IndexBuffer>>& indexBuffers,
	const std::shared_ptr<ConstantsBuffer>& constantsModel,
	const glm::vec3 worldPos,
	const glm::vec2 localPos,
	const unsigned level) 
	: m_context(context)
	, m_indexBuffers(indexBuffers)
	, m_constantsModel(constantsModel)
	, m_worldPos(worldPos)
	, m_localPos(localPos)
	, m_level(level)
	, m_scale(maxSize / pow<float>(2, m_level))
	, m_root(true)
	, m_sideFlag(0) {

	m_worldMatrix = glm::translate(glm::mat4(1), m_worldPos);
	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale, 1.0f, m_scale));

}

TerrainNode::~TerrainNode() {
}

void TerrainNode::update(const glm::vec3 camPos) {

	if (inRange(camPos, m_worldPos, m_scale)) {
		if (m_level < maxLevel && m_root) {
			m_root = false;
			float offset = maxSize / pow<float>(2, m_level + 1);
			for (unsigned i = 0; i < 2; ++i) {
				for (unsigned j = 0; j < 2; ++j) {
					m_children.push_back(std::make_shared<TerrainNode>(
						m_context,
						m_indexBuffers,
						m_constantsModel,
						m_worldPos - glm::vec3(offset / 2.0f, 0.0f, offset / 2.0f) + glm::vec3(i * offset, 0.0f, j * offset), 
						glm::vec2(i, j), 
						m_level + 1));
				}
			}
		}
	}
	else if(!m_root) {
		m_root = true;
		m_children.clear();
	}

	if (!m_root) {
		for (unsigned i = 0; i < m_children.size(); ++i) {
			m_children[i]->update(camPos);
		}
	}
	else {
		m_sideFlag = 0;

		float parentScale = m_scale * 2.0f;
		glm::vec3 parentPos = m_worldPos - glm::vec3((m_localPos.x * 2.0f - 1.0f) * m_scale / 2.0f, 0.0f, (m_localPos.y * 2.0f - 1.0f) * m_scale / 2.0f);

		if (!inRange(camPos, parentPos - glm::vec3(parentScale, 0.0f, 0.0f), parentScale) && m_localPos.x < 1) {
			m_sideFlag |= NORTH;
		}
		if (!inRange(camPos, parentPos + glm::vec3(parentScale, 0.0f, 0.0f), parentScale) && m_localPos.x > 0) {
			m_sideFlag |= SOUTH;
		}
		if (!inRange(camPos, parentPos - glm::vec3(0.0f, 0.0f, parentScale), parentScale) && m_localPos.y < 1) {
			m_sideFlag |= WEST;
		}
		if (!inRange(camPos, parentPos + glm::vec3(0.0f, 0.0f, parentScale), parentScale) && m_localPos.y > 0) {
			m_sideFlag |= EAST;
		}

			/*
		if (!inRange(camPos, m_worldPos - glm::vec3(m_scale, 0.0f, 0.0f), m_scale) && m_localPos.x < 1) {
			m_sideFlag |= NORTH;
		}

		if (!inRange(camPos, m_worldPos + glm::vec3(m_scale, 0.0f, 0.0f), m_scale) && m_localPos.x > 0) {
			m_sideFlag |= SOUTH;
		}

		if (!inRange(camPos, m_worldPos + glm::vec3(0.0f, 0.0f, m_scale), m_scale) && m_localPos.y > 0) {
			m_sideFlag |= EAST;
		}

		if (!inRange(camPos, m_worldPos - glm::vec3(0.0f, 0.0f, m_scale), m_scale) && m_localPos.y < 1) {
			m_sideFlag |= WEST;
		}
		*/
	}
}

void TerrainNode::render() {

	if (m_root) {
		m_constantsModel->setData((void*)glm::value_ptr(m_worldMatrix));
		m_context->drawIndexed(m_indexBuffers[m_sideFlag], m_indexBuffers[m_sideFlag]->getUnitCount(), 0);
	}
	else {
		for (unsigned i = 0; i < m_children.size(); ++i) {
			m_children[i]->render();
		}
	}
}