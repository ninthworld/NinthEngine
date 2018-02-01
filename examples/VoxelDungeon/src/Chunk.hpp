#pragma once

#include <memory>
#include <array>
#include <NinthEngine\Application\Game.hpp>
#include "ChunkManager.hpp"
#include "VoxelModel.hpp"

using namespace NinthEngine;

class Chunk : public VoxelModel<CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE> {
public:
	Chunk(
		const std::shared_ptr<ChunkManager>& m_chunkManager,
		const ChunkPos chunkPos);
	~Chunk();

	const ChunkPos getChunkPos() const { return m_chunkPos; };
	
	template<std::size_t X, std::size_t Y, std::size_t Z>
	void appendModel(
		const std::shared_ptr<VoxelModel<X, Y, Z>>& model,
		const unsigned x, const unsigned y, const unsigned z,
		const unsigned rotateY = 0) {

		for (unsigned i = 0; i < X; ++i) {
			for (unsigned j = 0; j < Y; ++j) {
				for (unsigned k = 0; k < Z; ++k) {
					VoxelMaterialType material = model->getVoxelAt(i, j, k);

					unsigned nI = i, nJ = j, nK = k;
					if (rotateY == 1) {
						unsigned temp = nI;
						nI = Z - nK - 1;
						nK = temp;
					}
					else if (rotateY == 2) {
						nI = X - nI;
						nK = Z - nK - 1;
					}
					else if (rotateY == 3) {
						unsigned temp = nI;
						nI = nK;
						nK = X - temp;
					}

					int newX = nI + x, newY = nJ + y, newZ = nK + z;
					if (newX >= 0 && newX < CHUNK_SIZE &&
						newY >= 0 && newY < CHUNK_SIZE &&
						newZ >= 0 && newZ < CHUNK_SIZE &&
						material != VM_NONE) {
						assert(material != VM_NONE);						
						setVoxelAt(newX, newY, newZ, model->getVoxelAt(i, j, k));
					}
				}
			}
		}
	};

private:
	std::shared_ptr<ChunkManager> m_chunkManager;
	ChunkPos m_chunkPos;

};