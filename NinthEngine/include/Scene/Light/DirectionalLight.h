#pragma once

#include "Light.h"

namespace NinthEngine {

class NE_API DirectionalLight : public Light {
protected:
	DirectionalLight(Node* parent);

public:
	~DirectionalLight();

	inline virtual const bool isCastShadow() { return (bool)m_data.castShadow; };
	inline virtual void setCastShadow(const bool castShadow) { m_data.castShadow = (float)castShadow; };

	static DirectionalLight* create(Node* parent);

protected:
	void updateLight() override;
};

}