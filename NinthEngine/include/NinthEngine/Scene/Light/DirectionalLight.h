#pragma once

#include "Light.h"

namespace NinthEngine {

struct NE_API DirectionalLightProps : LightProps {
	bool castShadow;
	DirectionalLightProps(const bool castShadow = true, const Color& diffuse = Color(), const Color& specular = Color())
		: LightProps(diffuse, specular), castShadow(castShadow) {}
};

class NE_API DirectionalLight : public Light {
	friend SceneNode;
protected:
	DirectionalLight(Node* parent, const DirectionalLightProps& props);
public:
	~DirectionalLight();

	inline virtual const bool isCastShadow() { return m_castShadow; }
	void setCastShadow(const bool castShadow);

	LightType getLightType() override { return LightType::Directional; }
	
protected:
	void updateLight() {}

private:
	bool m_castShadow;
};

}