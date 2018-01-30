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

	m_modelLink = std::make_unique<VoxelModel<20>>(m_context);

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
	
	for (unsigned i = 0; i < 20; ++i) {
		for (unsigned j = 0; j < 20; ++j) {
			for (unsigned k = 0; k < 20; ++k) {
				auto material = m_modelLink->getVoxelAt(i, j, k);
				setVoxelAt(i, j + 3, k, material);
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
	
	// Colors
	std::vector<unsigned char> colorsImage;
	unsigned width, height;
	lodepng::decode(colorsImage, width, height, "res/textures/colors.png", LodePNGColorType::LCT_RGB, 8);
	m_textureColors = m_device->createTexture()
		.withFormat(FORMAT_RGB_8_UNORM)
		.withSize(width, height)
		.build();
	m_textureColors->setSampler(m_sampler);

	std::vector<unsigned char> linkImage;
	unsigned w, h;
	lodepng::decode(linkImage, w, h, "res/textures/link.png", LodePNGColorType::LCT_RGBA, 8);

	std::vector<glm::vec3> linkColors;
	for (unsigned i = 0; i < linkImage.size(); i += 4) {
		if (linkImage[i + 3] > 0) {
			auto color = glm::vec3(linkImage[i], linkImage[i + 1], linkImage[i + 2]);

			unsigned x = i / 4 % 14;
			unsigned z = floor((i / 4) / (20 * 14));
			unsigned y = floor(((i / 4) % (20 * 14)) / 20);

			unsigned vIndex;
			for (vIndex = 0; vIndex < linkColors.size(); ++vIndex) {
				if (linkColors[vIndex] == color) break;
			}
			if (vIndex == linkColors.size()) linkColors.push_back(color);

			m_modelLink->setVoxelAt(x, y, z, VoxelMaterialType(vIndex + 64));
		}
	}

	for (unsigned i = 0; i < linkColors.size(); ++i) {
		colorsImage[32 * 3 + i * 3] = static_cast<unsigned char>(linkColors[i].r);
		colorsImage[32 * 3 + i * 3 + 1] = static_cast<unsigned char>(linkColors[i].g);
		colorsImage[32 * 3 + i * 3 + 2] = static_cast<unsigned char>(linkColors[i].b);
	}

	m_context->setData(m_textureColors, &colorsImage[0]);
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
	m_shaderVoxel->bind(0, "colorsTexture", m_textureColors, VERTEX_SHADER);
	m_shaderVoxel->bind(1, "dirtTexture", m_textureDirt, PIXEL_SHADER);
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