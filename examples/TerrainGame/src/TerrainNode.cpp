#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TerrainNode.hpp"

TerrainNode::TerrainNode(
	const std::shared_ptr<Terrain>& root,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameCamera>& camera,
	const std::shared_ptr<VertexArray>& vertexArray,
	const std::shared_ptr<ConstantBuffer>& constantNode,
	const glm::vec2 location,
	const int lod,
	const glm::vec2 index)
	: m_root(root)
	, m_context(context)
	, m_camera(camera)
	, m_vertexArray(vertexArray)
	, m_constantNode(constantNode)
	, m_location(location)
	, m_index(index)
	, m_lod(lod)
	, m_size(0.0f)
	, m_localMatrix(glm::mat4(1))
	, m_worldPos(glm::vec3(0))
	, m_neighbors(std::vector<bool>(4, false))
	, m_bounds(AABB{ glm::vec3(0), glm::vec3(0) })
	, m_leaf(true) {
	
	m_size = 1.0f / ((float)rootPatches * pow(2.0f, (float)m_lod));

	glm::vec2 pos = (location + m_size / 2.0f) * scaleXZ - scaleXZ / 2.0f;
	m_worldPos = glm::vec3(pos.x, 0.0f, pos.y);

	glm::vec2 pos0 = location * scaleXZ - scaleXZ / 2.0f;
	glm::vec2 pos1 = (location + m_size) * scaleXZ - scaleXZ / 2.0f;

	m_bounds = AABB{
		glm::vec3(pos0.x, 0, pos0.y),
		glm::vec3(pos1.x, root->getMaxHeightAt(m_location, m_location + glm::vec2(m_size)) * 2.0f, pos1.y)
	};

	m_localMatrix = glm::translate(m_localMatrix, glm::vec3(m_location.x, 0.0f, m_location.y));
	m_localMatrix = glm::scale(m_localMatrix, glm::vec3(m_size, 0.0f, m_size));
}

TerrainNode::~TerrainNode() {
}

void TerrainNode::update() {

	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->update();
	}

	glm::vec3 camPos = m_camera->getPosition();
	if (camPos.y > m_bounds.max.y) camPos.y -= m_bounds.max.y;
	else camPos.y = 0;

	float distance = glm::length(camPos - m_worldPos);

	if (distance < lodRange[m_lod]) {
		m_leaf = false;
		if (!m_children.size()) {
			for (unsigned i = 0; i < 2; ++i) {
				for (unsigned j = 0; j < 2; ++j) {
					m_children.push_back(
						std::make_unique<TerrainNode>(
							m_root,
							m_context,
							m_camera,
							m_vertexArray,
							m_constantNode,
							m_location + glm::vec2(i * m_size / 2.0f, j * m_size / 2.0f),
							m_lod + 1,
							glm::vec2(i, j)));
				}
			}
		}
	}
	else if (distance >= lodRange[m_lod]) {
		m_leaf = true;
		if (m_children.size()) {
			m_children.clear();
		}
	}

	if (m_leaf) {
		for (unsigned i = 0; i < 2; ++i) {
			for (unsigned j = 0; j < 2; ++j) {
				float a = (j == 0 ? -1 : 1);
				glm::vec2 pos = m_location + glm::vec2(i * a, (1 - i) * a) * m_size;
				if (pos.x >= 0 && pos.x < 1 && pos.y >= 0 && pos.y < 1) {
					int neighborLod = m_root->getLodAt(pos);
					m_neighbors[i * 2 + j] = (neighborLod > m_lod);
				}
			}
		}
	}
}

void TerrainNode::render() {

	if (!m_leaf) {
		for (unsigned i = 0; i < m_children.size(); ++i) {
			m_children[i]->render();
		}
	}
	else if(isInFrustum(m_bounds, m_camera->getViewFrustum())) {

		// Update Constants
		NodeData data{ m_localMatrix, glm::vec4(), m_location, m_index, m_size, m_lod, glm::vec2() };
		for (unsigned i = 0; i < 4; ++i) data.neighbors[i] = m_neighbors[i];
		m_context->setData(m_constantNode, &data);

		// Draw Terrain
		m_context->bind(m_vertexArray);
		m_context->draw(patchSize);
	}
}