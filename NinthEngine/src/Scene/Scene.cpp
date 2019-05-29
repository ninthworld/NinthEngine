#include "pch.h"
#include "Scene.h"
#include "Node.h"
#include "Camera/Camera.h"

namespace NinthEngine {

Scene* Scene::create() {
	return new Scene();
}

Scene::Scene() {
	m_rootNode = createNode(nullptr);
}

Scene::~Scene() {
	for (auto it = m_allNodes.begin(); it != m_allNodes.end(); ++it) {
		delete *it;
	}
}

void Scene::update() {
	m_rootNode->update();
}

Node* Scene::createNode(Node* parent) {
	auto node = Node::create(parent);
	m_allNodes.push_back(node);
	return node;
}

PerspectiveCamera* Scene::createPerspectiveCamera(Node* parent, const float aspectRatio) {
	auto camera = PerspectiveCamera::create(parent,	aspectRatio);
	m_allNodes.push_back(camera);
	m_cameras.push_back(camera);
	return camera;
}

AmbientLight* Scene::createAmbientLight(Node* parent) {
	auto light = AmbientLight::create(parent);
	m_allNodes.push_back(light);
	m_lights.push_back(light);
	return light;
}

DirectionalLight* Scene::createDirectionalLight(Node* parent) {
	auto light = DirectionalLight::create(parent);
	m_allNodes.push_back(light);
	m_lights.push_back(light);
	return light;
}

PointLight* Scene::createPointLight(Node* parent) {
	auto light = PointLight::create(parent);
	m_allNodes.push_back(light);
	m_lights.push_back(light);
	return light;
}


}