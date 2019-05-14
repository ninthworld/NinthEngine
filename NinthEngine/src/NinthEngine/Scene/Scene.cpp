#include "pch.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Camera/Camera.h"

namespace NinthEngine {

Scene::Scene() {
	m_rootNode = std::shared_ptr<SceneNode>(new SceneNode(nullptr, shared_from_this()));
}

Scene::~Scene() {
}

void Scene::update() {
	m_rootNode->update(false);
}

void Scene::addCamera(const std::shared_ptr<Camera>& camera) { 
	m_cameras.push_back(camera); 
};

void Scene::removeCamera(const unsigned int index) { 
	m_cameras.erase(m_cameras.begin() + index); 
};

void Scene::removeCamera(const std::shared_ptr<Camera>& camera) { 
	m_cameras.erase(std::find(m_cameras.begin(), m_cameras.end(), camera)); 
};

}