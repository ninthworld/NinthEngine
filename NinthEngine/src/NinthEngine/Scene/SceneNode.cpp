#include "pch.h"
#include "SceneNode.h"
#include "Camera/PerspectiveCamera.h"

namespace NinthEngine {

SceneNode::SceneNode(const std::shared_ptr<SceneNode>& parent, const std::shared_ptr<Scene>& scene)
	: Node<SceneNode>(parent), m_scene(scene) {
}

SceneNode::~SceneNode() {
}

std::shared_ptr<SceneNode> SceneNode::createSceneNode() {
	return std::shared_ptr<SceneNode>(new SceneNode(shared_from_this(), m_scene));
}

std::shared_ptr<PerspectiveCamera> SceneNode::createPerspectiveCamera() {
	auto camera = std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(shared_from_this(), m_scene));
	return camera;
}

}