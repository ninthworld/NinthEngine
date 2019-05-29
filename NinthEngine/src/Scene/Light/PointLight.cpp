#include "pch.h"
#include "PointLight.h"

namespace NinthEngine {

PointLight* PointLight::create(Node* parent) {
	auto node = new PointLight(parent);
	if (parent != nullptr) parent->addChild(node);
	return node;
}

PointLight::PointLight(Node* parent) : Light(parent) {
	m_data.type = static_cast<float>((int)LightType::Point);
}

PointLight::~PointLight() {}

void PointLight::updateLight() {
	m_data.position = glm::vec4(getWorldPosition(), 0.0f);
}

}