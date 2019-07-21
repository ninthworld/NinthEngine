#pragma once

#include "NinthEngine/Scene/SceneNode.h"

namespace NinthEngine {

enum class NE_API LightType {
	Ambient,
	Directional,
	Point
};

enum class NE_API LightAttenuation {
	Constant,
	Linear,
	Quadratic
};

struct NE_API LightProps {
	Color diffuse;
	Color specular;
	LightProps(const Color& diffuse = Color(), const Color& specular = Color())
		: diffuse(diffuse), specular(specular) {}
};

class NE_API Light : public SceneNode {
	friend class SceneNode;
protected:
	Light(Node* parent, const LightProps& props);
public:
	virtual ~Light();

	virtual void updateLocal();

	virtual Color getDiffuse() { return m_diffuse; }
	virtual Color getSpecular() { return m_specular; }

	virtual void setDiffuse(const Color& diffuse);
	virtual void setSpecular(const Color& specular);

	virtual LightType getLightType() { return LightType::Ambient; }

protected:
	virtual void updateLight() {}

protected:
	Color m_diffuse;
	Color m_specular;
};

}