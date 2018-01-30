#pragma once

#include <NinthEngine\Render\Config\LayoutConfig.hpp>

using namespace NinthEngine;

enum VoxelMaterialType {
	VM_NONE,
	VM_DIRT,
	VM_GRASS_1,
	VM_GRASS_2,
	VM_NULL
};

typedef unsigned char VoxelFace;
enum VoxelFaceType : unsigned char {
	VF_TOP = 0b1,
	VF_BOTTOM = 0b10,
	VF_RIGHT = 0b100,
	VF_LEFT = 0b1000,
	VF_FRONT = 0b10000,
	VF_BACK = 0b100000
};

const static LayoutConfig voxelVertexLayout =
LayoutConfig().float3(POSITION).float3(NORMAL).float1(COLOR);
struct VoxelVertex {
	glm::vec3 position;
	glm::vec3 normal;
	float material;
};

static VoxelVertex packVoxelVertex(const glm::vec3 position, const glm::vec3 normal, const VoxelMaterialType material) {
	return { position, normal, (float)material };
};