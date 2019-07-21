#pragma once

#include "Node.h"
#include "SceneManager.h"

namespace NinthEngine {

class PerspectiveCamera;
struct PerspectiveCameraProps;

class Light;
struct LightProps;

class PointLight;
struct PointLightProps;

class DirectionalLight;
struct DirectionalLightProps;

class NE_API SceneNode : public Node {
	friend SceneManager;
protected:
	SceneNode(Node* parent) : Node(parent) {}
public:
	virtual ~SceneNode() {}
	
	SceneNode* createSceneNode();

	PerspectiveCamera* createPerspectiveCamera(const PerspectiveCameraProps& props);

	Light* createAmbientLight(const LightProps& props);
	PointLight* createPointLight(const PointLightProps& props);
	DirectionalLight* createDirectionalLight(const DirectionalLightProps& props);
	
private:
	SceneManager* m_sceneManager;
};

}