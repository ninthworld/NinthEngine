#include <NinthEngine\Utils\LodePNG\lodepng.h>
#include "Chunk.hpp"
#include "ChunkManager.hpp"

ChunkManager::ChunkManager(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<ConstantBuffer>& constantCamera)
	: m_device(device)
	, m_context(context)
	, m_constantCamera(constantCamera) {

}

ChunkManager::~ChunkManager() {
}

void ChunkManager::init() {
	
	initConstants();
	initTextures();
	initShaders();
	initModels();

	std::vector<unsigned char> imageGround;
	std::vector<unsigned char> imageModels;
	unsigned width, height;
	unsigned widthM, heightM;
	lodepng::decode(imageGround, width, height, "res/maps/dungeon/map01_ground.png", LodePNGColorType::LCT_RGBA, 8);
	lodepng::decode(imageModels, widthM, heightM, "res/maps/dungeon/map01_models.png", LodePNGColorType::LCT_RGBA, 8);
	assert(width == widthM && height == heightM);

	unsigned chunkPixel = (CHUNK_SIZE / MAP_PIXEL_SIZE);
	for (unsigned i = 0; i < width / chunkPixel; ++i) {
		for (unsigned j = 0; j < height / chunkPixel; ++j) {
			ChunkPos pos{ i, 0, j };
			auto chunk = std::make_unique<Chunk>(shared_from_this(), pos);
			m_chunks.insert(std::make_pair(pos, std::move(chunk)));
		}
	}

	for (unsigned i = 0; i < imageGround.size(); i += 4) {
		auto colorGround = glm::vec4(imageGround[i], imageGround[i + 1], imageGround[i + 2], imageGround[i + 3]);
		auto colorModels = glm::vec4(imageModels[i], imageModels[i + 1], imageModels[i + 2], imageModels[i + 3]);
		unsigned iX = (i / 4) % width;
		unsigned iZ = floor((i / 4) / float(width));
		int cX = floor(iX / float(chunkPixel));
		int cZ = floor(iZ / float(chunkPixel));
		unsigned x = iX % (chunkPixel);
		unsigned z = iZ % (chunkPixel);

		auto& chunk = m_chunks[{ cX, 0, cZ }];
		if (colorGround.a) {
			switch (unsigned(colorGround.r)) {
			case 0: chunk->appendModel(m_modelStoneFloor, x * MAP_PIXEL_SIZE, colorGround.b * 16, z * MAP_PIXEL_SIZE); break;
			case 1: chunk->appendModel(m_modelDirtFloor, x * MAP_PIXEL_SIZE, colorGround.b * 16, z * MAP_PIXEL_SIZE); break;
			}			
		}

		if (colorModels.a) {
			switch (unsigned(colorModels.r)) {
			case 0: chunk->appendModel(m_modelStoneWall, x * MAP_PIXEL_SIZE, 1 + colorModels.b * 16, z * MAP_PIXEL_SIZE, colorModels.g, (unsigned(colorModels.g) % 2)); break;
			case 1: chunk->appendModel(m_modelStonePillarSmall, x * MAP_PIXEL_SIZE, 1 + colorModels.b * 16, z * MAP_PIXEL_SIZE); break;
			}
		}
	}

	// Generate Vertex Arrays
	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
		it->second->generateVertexArray(m_device);
	}
}

void ChunkManager::initConstants() {
	ChunkStruct chunkStruct{ glm::vec4() };
	m_constantChunk = m_device->createConstantBuffer()
		.withLayout(chunkStructLayout)
		.withData(&chunkStruct)
		.build();
}

void ChunkManager::initTextures() {

	m_sampler = m_device->createSampler()
		.withEdge(WRAP)
		.withFilter(NEAREST)
		.build();

	m_textureStoneFloor = m_device->createTexture()
		.withFile("res/textures/dungeon/stone_floor.png")
		.build();
	m_textureStoneFloor->setSampler(m_sampler);

	m_textureDirtFloor = m_device->createTexture()
		.withFile("res/textures/dungeon/dirt_floor.png")
		.build();
	m_textureDirtFloor->setSampler(m_sampler);

	m_textureStoneWall = m_device->createTexture()
		.withFile("res/textures/dungeon/stone_wall.png")
		.build();
	m_textureStoneWall->setSampler(m_sampler);

	m_textureStonePillarSmall = m_device->createTexture()
		.withFile("res/textures/dungeon/stone_pillar_small.png")
		.build();
	m_textureStonePillarSmall->setSampler(m_sampler);
}

void ChunkManager::initShaders() {

	m_shaderVoxel = m_device->createShader()
		.withLayout(voxelVertexLayout)
		.withGLSL<VERTEX_SHADER>("res/shaders/glsl/voxel.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/shaders/glsl/voxel.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/shaders/hlsl/voxel.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/shaders/hlsl/voxel.ps.hlsl", "main")
		.build();
	m_shaderVoxel->bind(0, "Camera", m_constantCamera);
	m_shaderVoxel->bind(1, "Chunk", m_constantChunk);

	m_shaderVoxel->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderVoxel->bind(0, "stoneFloor", m_textureStoneFloor, PIXEL_SHADER);
	m_shaderVoxel->bind(1, "dirtFloor", m_textureDirtFloor, PIXEL_SHADER);
	m_shaderVoxel->bind(2, "stoneWall", m_textureStoneWall, PIXEL_SHADER);
	m_shaderVoxel->bind(3, "stonePillarSmall", m_textureStonePillarSmall, PIXEL_SHADER);
}

void ChunkManager::initModels() {
	
	m_modelStoneFloor = std::make_shared<VoxelModel<MODEL_STONE_FLOOR>>(m_context);
	m_modelStoneFloor->loadModel("res/models/dungeon/model_stone_floor.png", VM_STONE_FLOOR);

	m_modelDirtFloor = std::make_shared<VoxelModel<MODEL_DIRT_FLOOR>>(m_context);
	m_modelDirtFloor->loadModel("res/models/dungeon/model_stone_floor.png", VM_DIRT_FLOOR);

	m_modelStoneWall = std::make_shared<VoxelModel<MODEL_STONE_WALL>>(m_context);
	m_modelStoneWall->loadModel("res/models/dungeon/model_stone_wall.png", VM_STONE_WALL_Z);

	m_modelStonePillarSmall = std::make_shared<VoxelModel<MODEL_STONE_PILLAR_SMALL>>(m_context);
	m_modelStonePillarSmall->loadModel("res/models/dungeon/model_stone_pillar_small.png", VM_STONE_PILLAR_SMALL);
}

void ChunkManager::update(const double deltaTime) {

}

void ChunkManager::render() {

	m_context->bind(m_shaderVoxel);
	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
		ChunkPos pos = it->first;
		ChunkStruct chunkStruct{ glm::vec4(pos.x, pos.y, pos.z, 0) * float(CHUNK_SIZE) };
		m_context->setData(m_constantChunk, &chunkStruct);
		it->second->render();
	}
}

const VoxelMaterialType ChunkManager::getVoxelAt(const ChunkPos chunkPos, const int x, const int y, const int z) const {
	ChunkPos newChunkPos{
		floor((chunkPos.x * CHUNK_SIZE + x) / CHUNK_SIZE),
		floor((chunkPos.y * CHUNK_SIZE + y) / CHUNK_SIZE),
		floor((chunkPos.z * CHUNK_SIZE + z) / CHUNK_SIZE) };
	auto find = m_chunks.find(newChunkPos);
	if (find != m_chunks.end()) {
		return find->second->getVoxelAt(UMOD(x, CHUNK_SIZE), UMOD(y, CHUNK_SIZE), UMOD(z, CHUNK_SIZE));
	}
	return VoxelMaterialType::VM_NULL;
}

void ChunkManager::setVoxelAt(const int x, const int y, const int z, const VoxelMaterialType material) {
	ChunkPos newChunkPos{
		floor(x / CHUNK_SIZE),
		floor(y / CHUNK_SIZE),
		floor(z / CHUNK_SIZE) };
	auto find = m_chunks.find(newChunkPos);
	if (find != m_chunks.end()) {
		find->second->setVoxelAt(UMOD(x, CHUNK_SIZE), UMOD(y, CHUNK_SIZE), UMOD(z, CHUNK_SIZE), material);
	}
}