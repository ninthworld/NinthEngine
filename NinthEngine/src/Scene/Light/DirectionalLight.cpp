#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Light/DirectionalLight.h"

namespace NinthEngine {

DirectionalLight::DirectionalLight(Node* parent, const DirectionalLightProps& props)
	: Light(parent, props), m_castShadow(props.castShadow) {}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::setCastShadow(const bool castShadow) {
	m_castShadow = castShadow;
	onLocalModified();
}

}