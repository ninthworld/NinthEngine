#pragma once

#include <memory>
#include <array>
#include <NinthEngine\Application\Game.hpp>
#include "ChunkManager.hpp"
#include "VoxelModel.hpp"

using namespace NinthEngine;

class Chunk : public VoxelModel<CHUNK_SIZE> {
public:
	Chunk(
		const std::shared_ptr<ChunkManager>& m_chunkManager,
		const ChunkPos chunkPos);
	~Chunk();

	const ChunkPos getChunkPos() const { return m_chunkPos; };
	
	/*
	const VoxelMaterialType getVoxelAt(const int x, const int y, const int z) const override {
		if (!inBounds(x, y, z)) {
			return m_chunkManager->getVoxelAt(m_chunkPos, x, y, z);
		}
		return m_data[getDataIndex(x, y, z)];
	};
	*/

private:
	std::shared_ptr<ChunkManager> m_chunkManager;
	ChunkPos m_chunkPos;

};