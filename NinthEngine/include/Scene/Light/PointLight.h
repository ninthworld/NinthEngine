#pragma once

#include "Light.h"

namespace NinthEngine {

class NE_API PointLight : public Light {
protected:
	PointLight(Node* parent);

public:
	~PointLight();

	inline const float getRange() { return m_data.range; };
	inline AttenuationType getAttentationType() { return static_cast<AttenuationType>((int)m_data.attenuation); };

	inline void setRange(const float range) { m_data.range = range; };
	inline void setAttenuationType(const AttenuationType& attenuation) { m_data.attenuation = static_cast<float>((int)attenuation); };

	static PointLight* create(Node* parent);

protected:
	void updateLight() override;
};

}