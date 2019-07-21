#pragma once

#include "Light.h"

namespace NinthEngine {

struct NE_API PointLightProps : LightProps {
	float range;
	LightAttenuation attenuation;
	PointLightProps(
		const float range, LightAttenuation attenuation = LightAttenuation::Linear,
		const Color& diffuse = Color(), const Color& specular = Color())
		: LightProps(diffuse, specular), range(range), attenuation(attenuation) {}
};

class NE_API PointLight : public Light {
	friend SceneNode;
protected:
	PointLight(Node* parent, const PointLightProps& props);
public:
	~PointLight();

	inline const float getRange() const { return m_range; }
	inline LightAttenuation getAttentationType() const { return m_attenuation; }

	void setRange(const float range);
	void setAttenuation(LightAttenuation attenuation);

	LightType getLightType() override { return LightType::Point; }

protected:
	void updateLight() {}

private:
	float m_range;
	LightAttenuation m_attenuation;

};

}