#pragma once

#include "pch.h"
#include "NinthEngine/Core.h"
#include "Node.h"
#include "Camera/PerspectiveCamera.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"

namespace NinthEngine {

class NE_API Scene {
	Scene();

public:
	~Scene();

	void update();

	inline Node* getRootNode() { return m_rootNode; };
	inline std::vector<Camera*>& getCameras() { return m_cameras; };
	inline std::vector<Light*>& getLights() { return m_lights; };

	Node* createNode(Node* parent);
	PerspectiveCamera* createPerspectiveCamera(Node* parent, const float aspectRatio);
	AmbientLight* createAmbientLight(Node* parent);
	DirectionalLight* createDirectionalLight(Node* parent);
	PointLight* createPointLight(Node* parent);

	static Scene* create();

private:
	Node* m_rootNode;
	std::vector<Node*> m_allNodes;
	std::vector<Camera*> m_cameras;
	std::vector<Light*> m_lights;
};

}