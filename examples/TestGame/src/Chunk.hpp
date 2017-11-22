#pragma once

#include <NinthEngine\GameUtils.hpp>
#include <NinthEngine\VertexArray.hpp>
#include <stdint.h>
#include <iostream>

using namespace NinthEngine;

namespace TestGame {

#define CHUNK_SIZE		16
#define CHUNK_HEIGHT	32

#define BLOCKID_AIR		0
#define BLOCKID_STONE	1
#define BLOCKID_DIRT	2
#define BLOCKID_GRASS	3

#define GET_CHUNK_BLOCK(a, x, y, z)	a[x + CHUNK_SIZE * (y + CHUNK_HEIGHT * z)]
#define TEXTURE_OFFSET(x)			((x) / 16.f) 

struct ChunkPos_t {
	int x, z;
};

struct ChunkVAO_t {
	std::vector<VertexArray::VertexNT_t> vertices;
	std::vector<unsigned int> indices;
};

typedef uint8_t BlockID_t;
typedef uint8_t Faces_t;
struct BlockID_Faces_t {
	BlockID_t block;
	Faces_t faces;
};
struct BlockFace_t {
	float u, v;
};
struct Block_t {
	BlockFace_t face[6]; // Top, Bottom, Front, Back, Right, Left
};

const Block_t blocks[]{
	{ },																	// Air
	{ { { 8, 14 },{ 8, 14 },{ 8, 14 },{ 8, 14 },{ 8, 14 },{ 8, 14 } } },	// Stone
	{ { { 0, 14 },{ 0, 14 },{ 0, 14 },{ 0, 14 },{ 0, 14 },{ 0, 14 } } },	// Dirt
	{ { { 0, 13 },{ 0, 14 },{ 1, 14 },{ 1, 14 },{ 1, 14 },{ 1, 14 } } }		// Grass
};

class Chunk {
public:
	Chunk(const ChunkPos_t pos);
	~Chunk();
	void init();
	void render();
	BlockID_t getBlockIDAt(const int x, const int y, const int z);
	void setBlockIDAt(const int x, const int y, const int z, const BlockID_t id);
protected:
	std::vector<BlockID_Faces_t> getVisibleFaces();
	static ChunkVAO_t Chunk::generateCube(const float x, const float y, const float z, const BlockID_Faces_t blockIdFaces, unsigned int *offset);
private:
	ChunkPos_t pos;
	BlockID_t data[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
	VertexArray *vao;
};

} // namespace TestGame