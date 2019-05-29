#pragma once

#include "NinthEngine/Scene/Node.h"

namespace NinthEngine {

enum class LightType {
	Ambient		= 0,
	Directional = 1,
	Point		= 2
};

enum class AttenuationType {
	Constant	= 0,
	Linear		= 1,
	Quadratic	= 2
};

struct LightData {
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	float type, range, attenuation, castShadow;
	LightData()
		: diffuse(glm::vec4()), specular(glm::vec4())
		, position(glm::vec4()), type(0.0f), range(0.0f)
		, attenuation(0.0f), castShadow(0.0f) {}
};

class NE_API Light : public Node {
public:
	Light(Node* node)
		: Node(node), m_data(LightData()) {};
	virtual ~Light() {};

	inline virtual void updateLocal() override {
		Node::updateLocal();
		updateLight();
	};

	inline virtual LightType getLightType() { return static_cast<LightType>((int)m_data.type); };
	inline virtual Color getDiffuse() { return Color(m_data.diffuse.r, m_data.diffuse.g, m_data.diffuse.b); };
	inline virtual Color getSpecular() { return Color(m_data.specular.r, m_data.specular.g, m_data.specular.b); };

	inline virtual void setDiffuse(const Color& diffuse) { m_data.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 0.0f); };
	inline virtual void setSpecular(const Color& specular) { m_data.specular = glm::vec4(specular.r, specular.g, specular.b, 0.0f); };
	
	inline virtual LightData getData() { return m_data; };

protected:
	virtual void updateLight() = 0;

protected:
	LightData m_data;
};

}