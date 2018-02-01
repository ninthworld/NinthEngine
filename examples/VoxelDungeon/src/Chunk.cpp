#include "Chunk.hpp"

Chunk::Chunk(
	const std::shared_ptr<ChunkManager>& chunkManager,
	const ChunkPos chunkPos)
	: VoxelModel<CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>(chunkManager->m_context)
	, m_chunkManager(chunkManager)
	, m_chunkPos(chunkPos) {
}

Chunk::~Chunk() {
}