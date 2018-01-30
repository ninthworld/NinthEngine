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

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ChunkPos pos{ i, 0, j };
			auto chunk = std::make_unique<Chunk>(shared_from_this(), pos);
			m_chunks.insert(std::make_pair(pos, std::move(chunk)));
		}
	}

	for (int i = 0; i < 4 * CHUNK_SIZE; ++i) {
		for (int j = 0; j < 4 * CHUNK_SIZE; ++j) {
			setVoxelAt(i, 0, j, VM_DIRT);

			float dist = glm::distance(glm::vec2(i, j), glm::vec2(CHUNK_SIZE * 2, CHUNK_SIZE * 2));

			if (dist > 12) {
				setVoxelAt(i, 1, j, VM_GRASS_1);
			}

			if (dist > 14) {
				setVoxelAt(i, 2, j, VM_GRASS_2);
			}
		}
	}
	
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

	m_textureDirt = m_device->createTexture()
		.withFile("res/textures/dirt.png")
		.build();
	m_textureDirt->setSampler(m_sampler);
}

void ChunkManager::initShaders() {

	m_shaderVoxel = m_device->createShader()
		.withLayout(voxelVertexLayout)
		.withGLSL<VERTEX_SHADER>("res/shaders/glsl/voxel.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/shaders/glsl/voxel.ps.glsl")
		.build();
	m_shaderVoxel->bind(0, "Camera", m_constantCamera);
	m_shaderVoxel->bind(1, "Chunk", m_constantChunk);

	m_shaderVoxel->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderVoxel->bind(0, "dirtTexture", m_textureDirt, PIXEL_SHADER);
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