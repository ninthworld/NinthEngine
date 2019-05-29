#include "pch.h"
#include "AmbientLight.h"

namespace NinthEngine {

AmbientLight* AmbientLight::create(Node* parent) {
	auto node = new AmbientLight(parent);
	if (parent != nullptr) parent->addChild(node);
	return node;
}

AmbientLight::AmbientLight(Node* parent) : Light(parent) {
	m_data.type = static_cast<float>((int)LightType::Ambient);
}

AmbientLight::~AmbientLight() {}

void AmbientLight::updateLight() {}

}