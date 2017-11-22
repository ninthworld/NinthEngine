#include "Chunk.hpp"
#include <GL\glew.h>

Chunk::Chunk(const ChunkPos_t pos) :
	pos(pos),
	vao(new VertexArray()) {
}

Chunk::~Chunk() {
	delete vao;
}

void Chunk::init() {
	// Generate Chunk Data
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (y < 20) setBlockIDAt(x, y, z, BLOCKID_STONE);
				else if (y < 24) setBlockIDAt(x, y, z, BLOCKID_DIRT);
				else if (y < 25) setBlockIDAt(x, y, z, BLOCKID_GRASS);
				else setBlockIDAt(x, y, z, BLOCKID_AIR);
			}
		}
	}

	// Generate VAO data
	std::vector<BlockID_Faces_t> faceData = getVisibleFaces();

	ChunkVAO_t chunkVAOData;
	unsigned int offset = 0;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				BlockID_Faces_t blockIdFaces = GET_CHUNK_BLOCK(faceData, x, y, z);
				if (blockIdFaces.block != BLOCKID_AIR && blockIdFaces.faces > 0) {
					ChunkVAO_t cube = generateCube(x, y, z, blockIdFaces, &offset);
					chunkVAOData.vertices.insert(std::end(chunkVAOData.vertices), std::begin(cube.vertices), std::end(cube.vertices));
					chunkVAOData.indices.insert(std::end(chunkVAOData.indices), std::begin(cube.indices), std::end(cube.indices));
				}
			}
		}
	}

	vao->setData(chunkVAOData.vertices, chunkVAOData.indices);
}

void Chunk::render() {
	vao->render();
}

BlockID_t Chunk::getBlockIDAt(const int x, const int y, const int z) {
	if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_SIZE)
		return GET_CHUNK_BLOCK(data, x, y, z);
	else return BLOCKID_AIR;
}

void Chunk::setBlockIDAt(const int x, const int y, const int z, BlockID_t id) {
	GET_CHUNK_BLOCK(data, x, y, z) = id;
}

std::vector<BlockID_Faces_t> Chunk::getVisibleFaces() {
	std::vector<BlockID_Faces_t> blockIDFaces(CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE);

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				Faces_t faces = 0b000000;

				if (getBlockIDAt(x, y, z) != BLOCKID_AIR) {
					if (getBlockIDAt(x, y + 1, z) == BLOCKID_AIR) faces |= 0b100000; // Top
					if (getBlockIDAt(x, y - 1, z) == BLOCKID_AIR) faces |= 0b010000; // Bottom
					if (getBlockIDAt(x, y, z + 1) == BLOCKID_AIR) faces |= 0b001000; // Front
					if (getBlockIDAt(x, y, z - 1) == BLOCKID_AIR) faces |= 0b000100; // Back
					if (getBlockIDAt(x + 1, y, z) == BLOCKID_AIR) faces |= 0b000010; // Right
					if (getBlockIDAt(x - 1, y, z) == BLOCKID_AIR) faces |= 0b000001; // Left
				}

				GET_CHUNK_BLOCK(blockIDFaces, x, y, z) = { getBlockIDAt(x, y, z), faces };
			}
		}
	}

	return blockIDFaces;
}

ChunkVAO_t Chunk::generateCube(const float x, const float y, const float z, const BlockID_Faces_t blockIdFaces, unsigned int *offset) {
	Block_t block = blocks[blockIdFaces.block];

	std::vector<VertexArray::VertexNT_t> verticesTop{
		{ x + 0, y + 1, z + 0, 0, 1, 0, TEXTURE_OFFSET(block.face[0].u + 0), TEXTURE_OFFSET(block.face[0].v + 0) },
		{ x + 0, y + 1, z + 1, 0, 1, 0, TEXTURE_OFFSET(block.face[0].u + 0), TEXTURE_OFFSET(block.face[0].v + 1) },
		{ x + 1, y + 1, z + 0, 0, 1, 0, TEXTURE_OFFSET(block.face[0].u + 1), TEXTURE_OFFSET(block.face[0].v + 0) },
		{ x + 1, y + 1, z + 1, 0, 1, 0, TEXTURE_OFFSET(block.face[0].u + 1), TEXTURE_OFFSET(block.face[0].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> verticesBottom{
		{ x + 1, y + 0, z + 0, 0, -1, 0, TEXTURE_OFFSET(block.face[1].u + 0), TEXTURE_OFFSET(block.face[1].v + 0) },
		{ x + 1, y + 0, z + 1, 0, -1, 0, TEXTURE_OFFSET(block.face[1].u + 0), TEXTURE_OFFSET(block.face[1].v + 1) },
		{ x + 0, y + 0, z + 0, 0, -1, 0, TEXTURE_OFFSET(block.face[1].u + 1), TEXTURE_OFFSET(block.face[1].v + 0) },
		{ x + 0, y + 0, z + 1, 0, -1, 0, TEXTURE_OFFSET(block.face[1].u + 1), TEXTURE_OFFSET(block.face[1].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> verticesFront{
		{ x + 1, y + 0, z + 1, 0, 0, 1, TEXTURE_OFFSET(block.face[2].u + 0), TEXTURE_OFFSET(block.face[2].v + 0) },
		{ x + 1, y + 1, z + 1, 0, 0, 1, TEXTURE_OFFSET(block.face[2].u + 0), TEXTURE_OFFSET(block.face[2].v + 1) },
		{ x + 0, y + 0, z + 1, 0, 0, 1, TEXTURE_OFFSET(block.face[2].u + 1), TEXTURE_OFFSET(block.face[2].v + 0) },
		{ x + 0, y + 1, z + 1, 0, 0, 1, TEXTURE_OFFSET(block.face[2].u + 1), TEXTURE_OFFSET(block.face[2].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> verticesBack{
		{ x + 0, y + 0, z + 0, 0, 0, -1, TEXTURE_OFFSET(block.face[3].u + 0), TEXTURE_OFFSET(block.face[3].v + 0) },
		{ x + 0, y + 1, z + 0, 0, 0, -1, TEXTURE_OFFSET(block.face[3].u + 0), TEXTURE_OFFSET(block.face[3].v + 1) },
		{ x + 1, y + 0, z + 0, 0, 0, -1, TEXTURE_OFFSET(block.face[3].u + 1), TEXTURE_OFFSET(block.face[3].v + 0) },
		{ x + 1, y + 1, z + 0, 0, 0, -1, TEXTURE_OFFSET(block.face[3].u + 1), TEXTURE_OFFSET(block.face[3].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> verticesRight{
		{ x + 1, y + 0, z + 0, 1, 0, 0, TEXTURE_OFFSET(block.face[4].u + 0), TEXTURE_OFFSET(block.face[4].v + 0) },
		{ x + 1, y + 1, z + 0, 1, 0, 0, TEXTURE_OFFSET(block.face[4].u + 0), TEXTURE_OFFSET(block.face[4].v + 1) },
		{ x + 1, y + 0, z + 1, 1, 0, 0, TEXTURE_OFFSET(block.face[4].u + 1), TEXTURE_OFFSET(block.face[4].v + 0) },
		{ x + 1, y + 1, z + 1, 1, 0, 0, TEXTURE_OFFSET(block.face[4].u + 1), TEXTURE_OFFSET(block.face[4].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> verticesLeft{
		{ x + 0, y + 0, z + 1, -1, 0, 0, TEXTURE_OFFSET(block.face[5].u + 0), TEXTURE_OFFSET(block.face[5].v + 0) },
		{ x + 0, y + 1, z + 1, -1, 0, 0, TEXTURE_OFFSET(block.face[5].u + 0), TEXTURE_OFFSET(block.face[5].v + 1) },
		{ x + 0, y + 0, z + 0, -1, 0, 0, TEXTURE_OFFSET(block.face[5].u + 1), TEXTURE_OFFSET(block.face[5].v + 0) },
		{ x + 0, y + 1, z + 0, -1, 0, 0, TEXTURE_OFFSET(block.face[5].u + 1), TEXTURE_OFFSET(block.face[5].v + 1) }
	};

	std::vector<VertexArray::VertexNT_t> vertices;
	std::vector<unsigned int> indices;
	if ((blockIdFaces.faces & 0b100000) == 0b100000) {
		vertices.insert(std::end(vertices), std::begin(verticesTop), std::end(verticesTop));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	if ((blockIdFaces.faces & 0b010000) == 0b010000) {
		vertices.insert(std::end(vertices), std::begin(verticesBottom), std::end(verticesBottom));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	if ((blockIdFaces.faces & 0b001000) == 0b001000) {
		vertices.insert(std::end(vertices), std::begin(verticesFront), std::end(verticesFront));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	if ((blockIdFaces.faces & 0b000100) == 0b000100) {
		vertices.insert(std::end(vertices), std::begin(verticesBack), std::end(verticesBack));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	if ((blockIdFaces.faces & 0b000010) == 0b000010) {
		vertices.insert(std::end(vertices), std::begin(verticesRight), std::end(verticesRight));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	if ((blockIdFaces.faces & 0b000001) == 0b000001) {
		vertices.insert(std::end(vertices), std::begin(verticesLeft), std::end(verticesLeft));
		unsigned int i = *offset;
		unsigned int ind[]{ i, i + 1, i + 2, i + 2, i + 1, i + 3 };
		indices.insert(std::end(indices), std::begin(ind), std::end(ind));
		*offset += 4;
	}

	return { vertices, indices };
}