#pragma once

#include <vector>
#include <array>
#include <NinthEngine\Application\Game.hpp>
#include "VoxelUtils.hpp"

using namespace NinthEngine;

template<std::size_t N>
class VoxelModel {
public:
	VoxelModel(const std::shared_ptr<GraphicsContext>& context)
		: m_context(context) {
	};
	~VoxelModel() {};
	
	void render() {
		if (m_vertexArray) {
			m_context->bind(m_vertexArray);
			m_context->drawIndexed(m_indexBuffer);
		}
	};

	const std::size_t getSize() const { return N; };

	const VoxelMaterialType getVoxelAt(const int x, const int y, const int z) const {
		if (!inBounds(x, y, z)) {
			return VM_NONE;
		}
		return m_data[getDataIndex(x, y, z)];
	};

	void setVoxelAt(const int x, const int y, const int z, const VoxelMaterialType material) {
		assert(inBounds(x, y, z));
		m_data[getDataIndex(x, y, z)] = material;
	};

	void generateFaces(std::array<VoxelFace, N * N * N>& faces) {
		for (unsigned i = 0; i < N; ++i) {
			for (unsigned j = 0; j < N; ++j) {
				for (unsigned k = 0; k < N; ++k) {
					unsigned index = getDataIndex(i, j, k);
					if (getVoxelAt(i, j, k) == VM_NONE) continue;
					if (getVoxelAt(i - 1, j, k) == VM_NONE) faces[index] |= VF_LEFT;
					if (getVoxelAt(i + 1, j, k) == VM_NONE) faces[index] |= VF_RIGHT;
					if (getVoxelAt(i, j - 1, k) == VM_NONE) faces[index] |= VF_BOTTOM;
					if (getVoxelAt(i, j + 1, k) == VM_NONE) faces[index] |= VF_TOP;
					if (getVoxelAt(i, j, k - 1) == VM_NONE) faces[index] |= VF_BACK;
					if (getVoxelAt(i, j, k + 1) == VM_NONE) faces[index] |= VF_FRONT;
				}
			}
		}
	};

	void generateGeometry(std::vector<VoxelVertex>& vertices, std::vector<int>& indices) {

		auto faces = std::array<VoxelFace, N * N * N>();
		generateFaces(faces);

		unsigned indicesIndex = 0;
		VoxelFace face = VF_TOP;
		for (int side = 0; side < 6; side++, face <<= 1) {
			auto mask = std::array<bool, N * N * N>();
			for (unsigned i = 0; i < N; ++i) {
				for (unsigned j = 0; j < N; ++j) {
					for (unsigned k = 0; k < N; ++k) {
						unsigned x, y;
						if (face == VF_TOP || face == VF_BOTTOM) x = k, y = i;
						else if (face == VF_RIGHT || face == VF_LEFT) x = j, y = k;
						else if (face == VF_FRONT || face == VF_BACK) x = i, y = j;

						if (!mask[getDataIndex(i, j, k)] && faces[getDataIndex(i, j, k)] & face) {
							auto material = getVoxelAt(i, j, k);

							int width = 0;
							int height = 0;
							for (unsigned m = x; m < N; ++m) {
								bool mx = (m == x);
								for (unsigned n = y; n < N && (mx || n < width + y); ++n) {
									unsigned index;
									if (face == VF_TOP || face == VF_BOTTOM) index = getDataIndex(n, j, m);
									else if (face == VF_RIGHT || face == VF_LEFT) index = getDataIndex(i, m, n);
									else if (face == VF_FRONT || face == VF_BACK) index = getDataIndex(m, n, k);

									if (!(faces[index] & face) ||
										mask[index] ||
										m_data[index] != material) {
										if (mx) break;
										else goto masking;
									}
									else if (mx) {
										width++;
									}
								}
								height++;
							}

						masking:
							for (unsigned m = x; m < height + x; ++m) {
								for (unsigned n = y; n < width + y; ++n) {
									unsigned index;
									if (face == VF_TOP || face == VF_BOTTOM) index = getDataIndex(n, j, m);
									else if (face == VF_RIGHT || face == VF_LEFT) index = getDataIndex(i, m, n);
									else if (face == VF_FRONT || face == VF_BACK) index = getDataIndex(m, n, k);
									mask[index] = true;
								}
							}

							glm::vec3 normal;
							if (face == VF_TOP || face == VF_BOTTOM) normal = glm::vec3(0, 1, 0);
							else if (face == VF_RIGHT || face == VF_LEFT) normal = glm::vec3(1, 0, 0);
							else if (face == VF_FRONT || face == VF_BACK) normal = glm::vec3(0, 0, 1);
							normal *= ((side + 1) % 2 ? 1.0f : -1.0f);

							glm::vec3 position[4];
							for (unsigned m = 0; m < 4; ++m) {
								position[m] = glm::vec3(i, j, k);

								if (m == 1 || m == 2) {
									if (face == VF_TOP || face == VF_BOTTOM) position[m] += glm::vec3(0, 0, height);
									else if (face == VF_RIGHT || face == VF_LEFT) position[m] += glm::vec3(0, height, 0);
									else if (face == VF_FRONT || face == VF_BACK) position[m] += glm::vec3(height, 0, 0);
								}

								if (m > 1) {
									if (face == VF_TOP || face == VF_BOTTOM) position[m] += glm::vec3(width, 0, 0);
									else if (face == VF_RIGHT || face == VF_LEFT) position[m] += glm::vec3(0, 0, width);
									else if (face == VF_FRONT || face == VF_BACK) position[m] += glm::vec3(0, width, 0);
								}

								if ((side + 1) % 2) {
									if (face == VF_TOP || face == VF_BOTTOM) position[m] += glm::vec3(0, 1, 0);
									else if (face == VF_RIGHT || face == VF_LEFT) position[m] += glm::vec3(1, 0, 0);
									else if (face == VF_FRONT || face == VF_BACK) position[m] += glm::vec3(0, 0, 1);
								}
							}

							if ((side + 1) % 2) {
								vertices.push_back(packVoxelVertex(position[0], normal, material));
								vertices.push_back(packVoxelVertex(position[1], normal, material));
								vertices.push_back(packVoxelVertex(position[2], normal, material));
								vertices.push_back(packVoxelVertex(position[3], normal, material));
							}
							else {
								vertices.push_back(packVoxelVertex(position[2], normal, material));
								vertices.push_back(packVoxelVertex(position[1], normal, material));
								vertices.push_back(packVoxelVertex(position[0], normal, material));
								vertices.push_back(packVoxelVertex(position[3], normal, material));
							}

							indices.push_back(indicesIndex);
							indices.push_back(indicesIndex + 1);
							indices.push_back(indicesIndex + 2);
							indices.push_back(indicesIndex + 2);
							indices.push_back(indicesIndex + 3);
							indices.push_back(indicesIndex);
							indicesIndex += 4;
						}
					}
				}
			}
		}
	};

	void generateVertexArray(const std::shared_ptr<GraphicsDevice>& device) {

		std::vector<VoxelVertex> vertices;
		std::vector<int> indices;
		generateGeometry(vertices, indices);

		m_vertexArray = device->createVertexArray();
		m_vertexArray->bind(device->createVertexBuffer()
			.withLayout(voxelVertexLayout)
			.withData(vertices.size(), vertices.data())
			.build());

		m_indexBuffer = device->createIndexBuffer()
			.withLayout(LayoutConfig().int1())
			.withData(indices.size(), indices.data())
			.build();
	};

protected:
	const bool inBounds(const int x, const int y, const int z) const {
		return (x >= 0 && x < N && y >= 0 && y < N && z >= 0 && z < N);
	};

	const unsigned getDataIndex(const int x, const int y, const int z) const {
		assert(inBounds(x, y, z));
		return x * N * N + y * N + z;
	};
	
protected:
	std::shared_ptr<GraphicsContext> m_context;

	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;

	std::array<VoxelMaterialType, N * N * N> m_data;

};