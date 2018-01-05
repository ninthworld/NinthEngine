#include <NinthEngine\Camera\GameCamera.hpp>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TerrainNode.hpp"

namespace {

} // namespace

TerrainNode::TerrainNode(
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<VertexArray>& vertexArray,
	const std::shared_ptr<IndexBuffer>& indexBuffer,
	const std::shared_ptr<ConstantBuffer>& constantNode,
	const glm::vec2 location,
	const int lod,
	const glm::vec2 index)
	: m_context(context)
	, m_vertexArray(vertexArray)
	, m_indexBuffer(indexBuffer)
	, m_constantNode(constantNode)
	, m_location(location)
	, m_index(index)
	, m_lod(lod)
	, m_size(0.0f)
	, m_localMatrix(glm::mat4(1))
	, m_worldPos(glm::vec3(0))
	, m_leaf(true) {

	m_size = 1.0f / ((float)rootPatches * pow(2.0f, (float)m_lod));

	glm::vec2 pos = (location + m_size / 2.0f) * scaleXZ - scaleXZ / 2.0f;
	m_worldPos = glm::vec3(pos.x, 0.0f, pos.y);

	m_localMatrix = glm::translate(m_localMatrix, glm::vec3(m_location.x, 0.0f, m_location.y));
	m_localMatrix = glm::scale(m_localMatrix, glm::vec3(m_size, 0.0f, m_size));
}

TerrainNode::~TerrainNode() {
}

void TerrainNode::update(const glm::vec3 cameraPos) {

	float distance = glm::length(cameraPos - m_worldPos);

	if (distance < lodRange[m_lod]) {
		if(m_leaf) m_leaf = false;
		if (!m_children.size()) {
			for (unsigned i = 0; i < 2; ++i) {
				for (unsigned j = 0; j < 2; ++j) {
					m_children.push_back(
						std::make_unique<TerrainNode>(
							m_context,
							m_vertexArray,
							m_indexBuffer,
							m_constantNode,
							m_location + glm::vec2(i * m_size / 2.0f, j * m_size / 2.0f),
							m_lod + 1,
							glm::vec2(i, j)));
				}
			}
		}
	}
	else if (distance >= lodRange[m_lod]) {
		if (!m_leaf) m_leaf = true;
		if(m_children.size()) m_children.clear();
	}

	for (unsigned i = 0; i < m_children.size(); ++i) {
		m_children[i]->update(cameraPos);
	}
}

void TerrainNode::render() {

	if (m_leaf) {
		// Bind Constants
		//m_constantNode->bind(VERTEX_SHADER_BIT);

		// Update Constants
		NodeData data{ m_localMatrix, m_location, m_index, m_size, m_lod, glm::vec2() };
		m_constantNode->setData(&data);

		// Draw Terrain
		m_vertexArray->bind();
		m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount(), 0);
		m_vertexArray->unbind();

		// Unbind All
		//m_constantNode->unbind(VERTEX_SHADER_BIT);
	}
	else {
		for (unsigned i = 0; i < m_children.size(); ++i) {
			m_children[i]->render();
		}
	}
}