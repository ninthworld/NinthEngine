#pragma once

#include "Light.h"

namespace NinthEngine {

class NE_API AmbientLight : public Light {
protected:
	AmbientLight(Node* parent);

public:
	~AmbientLight();

	static AmbientLight* create(Node* parent);

protected:
	void updateLight() override;
};

}