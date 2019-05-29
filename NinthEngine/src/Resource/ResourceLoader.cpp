#include "pch.h"
#include "NinthEngine/Resource/ResourceLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace NinthEngine {

ResourceLoader* ResourceLoader::create(Graphics* graphics) {
	return new ResourceLoader(graphics);
}

ResourceLoader::ResourceLoader(Graphics* graphics)
	: m_graphics(graphics) {
}

ResourceLoader::~ResourceLoader() {
	for (auto it = m_models.begin(); it != m_models.end(); ++it) delete *it;
}

Model* ResourceLoader::loadModel(const std::string& file) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str());
	if (!warn.empty()) NE_WARN(warn);
	if (!err.empty()) NE_ERROR(err);
	if (!success) return nullptr;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	ModelProps modelProps;
	modelProps.vertexArray = m_graphics->createVertexArray();

	for (size_t s = 0; s < shapes.size(); ++s) {
		size_t indexOffset = 0;
		std::vector<int> indices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			for (size_t v = 0; v < fv; ++v) {
				tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

				positions.push_back(glm::vec3(vx, vy, vz));
				normals.push_back(glm::vec3(nx, ny, nz));
				texCoords.push_back(glm::vec2(tx, ty));
			}
			indexOffset += fv;
			indices.push_back(f);
		}

		BufferProps positionsProps;
		positionsProps.layout = BufferLayout({ LayoutType::Float3 });
		positionsProps.unitCount = positions.size();
		positionsProps.data = positions.data();
		VertexBuffer* positionsBuffer = m_graphics->createVertexBuffer(positionsProps);

		BufferProps normalsProps;
		normalsProps.layout = BufferLayout({ LayoutType::Float3 });
		normalsProps.unitCount = normals.size();
		normalsProps.data = normals.data();
		VertexBuffer* normalsBuffer = m_graphics->createVertexBuffer(normalsProps);

		BufferProps texCoordsProps;
		texCoordsProps.layout = BufferLayout({ LayoutType::Float2 });
		texCoordsProps.unitCount = texCoords.size();
		texCoordsProps.data = texCoords.data();
		VertexBuffer* texCoordsBuffer = m_graphics->createVertexBuffer(texCoordsProps);

		BufferProps indexProps;
		indexProps.layout = BufferLayout({ LayoutType::Integer });
		indexProps.unitCount = indices.size();
		indexProps.data = indices.data();
		IndexBuffer* indexBuffer = m_graphics->createIndexBuffer(indexProps);

		MeshProps meshProps;
		meshProps.indexBuffer = indexBuffer;
		meshProps.material = GOLD_MATERIAL;
		Mesh* mesh = Mesh::create(meshProps);

		modelProps.vertexArray->addVertexBuffer(positionsBuffer);
		modelProps.vertexArray->addVertexBuffer(normalsBuffer);
		modelProps.vertexArray->addVertexBuffer(texCoordsBuffer);
		modelProps.meshes.push_back(mesh);
	}

	Model* model = Model::createModel(modelProps);
	m_models.push_back(model);

	return model;
}

Texture* ResourceLoader::loadTexture(const std::string& file) {
	int width, height, components;
	unsigned char* data = stbi_load(file.c_str(), &width, &height, &components, 0);

	TextureProps props;
	props.width = width;
	props.height = height;
	props.components = 
		( components == 1 ? TextureComponentType::R 
		: components == 2 ? TextureComponentType::RG
		: components == 3 ? TextureComponentType::RGB
		: TextureComponentType::RGBA);
	props.data = data;
	Texture* texture = m_graphics->createTexture(props);

	stbi_image_free(data);

	return texture;
}

Shader* ResourceLoader::loadShader(const std::string& vertexShader, const std::string& pixelShader) {
	ShaderProps props;
	props.vertexShaderSrc = readFile(vertexShader);
	props.pixelShaderSrc = readFile(pixelShader);
	return m_graphics->createShader(props);
}

// Read file to string with upfront allocation
// Source: https://stackoverflow.com/a/2602060
// Author: Tyler McHenry, resueman, KeithB
std::string ResourceLoader::readFile(const std::string& file) {
	std::string out;
	std::ifstream fs(file);
	if (fs) {
		fs.seekg(0, std::ios::end);
		out.reserve(fs.tellg());
		fs.seekg(0, std::ios::beg);
		out.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
		fs.close();
	}
	return out;
}

}