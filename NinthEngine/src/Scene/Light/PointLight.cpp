#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Scene/Light/PointLight.h"

namespace NinthEngine {

PointLight::PointLight(Node* parent, const PointLightProps& props) 
	: Light(parent, props), m_range(props.range), m_attenuation(props.attenuation) {}

PointLight::~PointLight() {}

void PointLight::setRange(const float range) {
	m_range = range;
	onLocalModified();
}

void PointLight::setAttenuation(LightAttenuation attenuation) {
	m_attenuation = attenuation;
	onLocalModified();
}

}