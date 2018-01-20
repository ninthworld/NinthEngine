#include "TerrainNode.hpp"

TerrainNode::TerrainNode(
	const std::shared_ptr<Terrain>& root,
	const float localSize,
	const glm::vec2 localPos,
	const glm::vec2 index,
	const int lod)
	: m_root(root)
	, m_localSize(localSize)
	, m_localPos(localPos)
	, m_worldPos((localPos + localSize / 2.0f - 0.5f) * root->m_config.scale.x)
	, m_index(index)
	, m_lod(lod)
	, m_neighbors(0)
	, m_leaf(false)
	, m_childCount(0)
	, m_visible(false)
	, m_bounds(AABB{
		glm::vec3(localPos.x - 0.5f, 0, localPos.y - 0.5f) * root->m_config.scale,
		glm::vec3(localPos.x + localSize - 0.5f, 1, localPos.y + localSize - 0.5f) * root->m_config.scale }) {

	update();
}

TerrainNode::~TerrainNode() {
}

void TerrainNode::update() {

	for (unsigned i = 0; i < m_childCount; ++i) {
		m_children[i]->update();
	}

	// LOD calculation
	float distance = glm::length(glm::vec2(m_root->m_camera->getPosition().x, m_root->m_camera->getPosition().z) - m_worldPos);
	if (distance < m_root->m_config.lodRanges[m_lod]) {
		m_leaf = false;
		if (!m_childCount) {
			float size = m_localSize / 2.0f;
			for (unsigned i = 0; i < 2; ++i) {
				for (unsigned j = 0; j < 2; ++j) {
					m_children[i * 2 + j] = std::make_unique<TerrainNode>(
						m_root,
						size,
						m_localPos + glm::vec2(i * size, j * size),
						glm::vec2(i, j),
						m_lod + 1);
					m_childCount++;
				}
			}
		}
	}
	else {
		m_leaf = true;
		if (m_childCount) {
			for (unsigned i = 0; i < m_children.size(); ++i) {
				m_children[i] = nullptr;
			}
			m_childCount = 0;
		}
	}
}

void TerrainNode::preRender() {

	// Update frustum
	m_visible = isInFrustum(m_bounds, m_root->m_camera->getViewFrustum());

	for (unsigned i = 0; i < m_childCount; ++i) {
		m_children[i]->preRender();
	}

	// Get neighbor LODs
	if (m_leaf) {
		m_neighbors = 0;
		for (unsigned i = 0; i < 4; ++i) {
			m_neighborLods[i] = 0;
			NodeNeighborType neighbor;
			glm::vec2 pos = m_localPos;
			switch (i) {
			case 0: pos += glm::vec2(0, -m_localSize); neighbor = WEST; break;
			case 1: pos += glm::vec2(0, m_localSize); neighbor = EAST; break;
			case 2: pos += glm::vec2(-m_localSize, 0); neighbor = NORTH; break;
			case 3: pos += glm::vec2(m_localSize, 0); neighbor = SOUTH; break;
			}

			if (pos.x >= 0 && pos.x < 1 &&
				pos.y >= 0 && pos.y < 1) {
				int neighborLod = m_root->getLodAt(pos);
				if (neighborLod < m_lod) {
					m_neighbors |= neighbor;
				}
				m_neighborLods[i] = neighborLod;
			}
		}
	}
}

void TerrainNode::render() {


	if (m_visible) {
		if (!m_leaf) {
			for (unsigned i = 0; i < m_childCount; ++i) {
				m_children[i]->render();
			}
		}
		else {
			auto nodeStruct = NodeStruct{
				m_localPos, m_localSize, m_lod, glm::vec4(m_neighborLods[0], m_neighborLods[1], m_neighborLods[2], m_neighborLods[3]) };
			m_root->m_context->setData(m_root->m_constantNode, &nodeStruct);
			//m_root->m_context->drawIndexed(m_root->m_indexBuffers[m_neighbors]);
			m_root->m_context->draw(pow(m_root->m_config.nodeVertexCount, 2));
		}
	}
}