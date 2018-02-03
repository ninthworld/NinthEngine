#pragma once

#include <map>
#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include "VoxelUtils.hpp"
#include "VoxelModel.hpp"

using namespace NinthEngine;

#define CHUNK_SIZE 32
#define MAP_PIXEL_SIZE 4

#define MODEL_STONE_FLOOR	4, 1, 4
#define MODEL_DIRT_FLOOR	4, 1, 4
#define MODEL_STONE_WALL	4, 16, 4
#define MODEL_STONE_PILLAR_SMALL	4, 16, 4

struct ChunkPos {
	int x, y, z;
};
inline bool operator<(const ChunkPos& a, const ChunkPos& b) {
	return (a.x *CHUNK_SIZE * CHUNK_SIZE + a.y * CHUNK_SIZE + a.z <
		b.x * CHUNK_SIZE * CHUNK_SIZE + b.y * CHUNK_SIZE + b.z);
};

const static LayoutConfig chunkStructLayout =
	LayoutConfig().float4();
struct ChunkStruct {
	glm::vec4 chunkPosition;
};

class Chunk;

class ChunkManager : public std::enable_shared_from_this<ChunkManager> {
public:
	ChunkManager(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<ConstantBuffer>& constantCamera);
	~ChunkManager();

	void init();
	void update(const double deltaTime);
	void render();

	const VoxelMaterialType getVoxelAt(const ChunkPos chunkPos, const int x, const int y, const int z) const;
	void setVoxelAt(const int x, const int y, const int z, const VoxelMaterialType material);

private:
	// Constants
	std::shared_ptr<ConstantBuffer> m_constantChunk;
	void initConstants();

	// Textures
	std::shared_ptr<Sampler> m_sampler;
	std::shared_ptr<Texture> m_textureStoneFloor;
	std::shared_ptr<Texture> m_textureDirtFloor;
	std::shared_ptr<Texture> m_textureStoneWall;
	std::shared_ptr<Texture> m_textureStonePillarSmall;
	void initTextures();

	// Shaders
	std::shared_ptr<Shader> m_shaderVoxel;
	void initShaders();

	// Models
	std::shared_ptr<VoxelModel<MODEL_STONE_FLOOR>> m_modelStoneFloor;
	std::shared_ptr<VoxelModel<MODEL_DIRT_FLOOR>> m_modelDirtFloor;
	std::shared_ptr<VoxelModel<MODEL_STONE_WALL>> m_modelStoneWall;
	std::shared_ptr<VoxelModel<MODEL_STONE_PILLAR_SMALL>> m_modelStonePillarSmall;
	void initModels();

private:
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<ConstantBuffer> m_constantCamera;

	std::map<ChunkPos, std::unique_ptr<Chunk>> m_chunks;

	friend class Chunk;
};