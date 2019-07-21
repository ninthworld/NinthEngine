#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "NinthEngine/Graphics/IndexBuffer.h"
#include "NinthEngine/Graphics/Rasterizer.h"
#include "NinthEngine/Graphics/Texture.h"

namespace NinthEngine {

struct NE_API Material {
	Color diffuse;
	Color emissive;
	Color specular;
	float glossiness;
	float metallic;
	float roughness;
	Texture* albedoMap;
	Texture* metallicRoughnessMap;
	Texture* normalMap;
	Texture* specularMap;
	Texture* ambientOcclusionMap;
	Texture* emissiveMap;
	Material(
		Color diffuse = Color(), Color emissive = Color(), Color specular = Color(),
		float glossiness = 0.0f, float metallic = 0.0f, float roughness = 0.0f,
		Texture* albedoMap = nullptr, Texture* metallicRoughnessMap = nullptr, Texture* normalMap = nullptr,
		Texture* specularMap = nullptr, Texture* ambientOcclusionMap = nullptr, Texture* emissiveMap = nullptr)
		: diffuse(diffuse), emissive(emissive), specular(specular)
		, glossiness(glossiness), metallic(metallic), roughness(roughness)
		, albedoMap(albedoMap), metallicRoughnessMap(metallicRoughnessMap), normalMap(normalMap)
		, specularMap(specularMap), ambientOcclusionMap(ambientOcclusionMap), emissiveMap(emissiveMap) {};
};

static const Material GOLD_MATERIAL = Material(Color(1.0f, 0.766f, 0.336f), Color(), Color(1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 0.1f);

struct NE_API MeshProps {
	Material material;
	IndexBuffer* indexBuffer;
	Rasterizer* rasterizer;
	PrimitiveType primitive;
	MeshProps(
		const Material& material = Material(), IndexBuffer* indexBuffer = nullptr, 
		Rasterizer* rasterizer = nullptr, const PrimitiveType& primitive = PrimitiveType::Triangles)
		: material(material), indexBuffer(indexBuffer), rasterizer(rasterizer) {};
};

class NE_API Mesh {
public:
	Mesh(const MeshProps& props)
		: m_material(props.material), m_indexBuffer(props.indexBuffer)
		, m_rasterizer(props.rasterizer), m_primitive(props.primitive) {}
	virtual ~Mesh() {}

	inline Material getMaterial() { return m_material; }
	inline PrimitiveType getPrimitiveType() { return m_primitive; }
	inline IndexBuffer* getIndexBuffer() { return m_indexBuffer; }
	inline Rasterizer* getRasterizer() { return m_rasterizer; }

	inline void setMaterial(const Material& material) { m_material = material; }
	inline void setPrimitiveType(const PrimitiveType& primitive) { m_primitive = primitive; }
	inline void setIndexBuffer(IndexBuffer* indexBuffer) { m_indexBuffer = indexBuffer; }
	inline void setRasterizer(Rasterizer* rasterizer) { m_rasterizer = rasterizer; }

private:
	Material m_material;
	PrimitiveType m_primitive;
	IndexBuffer* m_indexBuffer;
	Rasterizer* m_rasterizer;
};

}
