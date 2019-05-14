#pragma once

#include "Node.h"
#include "Scene.h"

namespace NinthEngine {

class PerspectiveCamera;

class NE_API SceneNode : public Node<SceneNode> {
public:
	SceneNode(const std::shared_ptr<SceneNode>& parent, const std::shared_ptr<Scene>& scene);
	~SceneNode();

	std::shared_ptr<SceneNode> createSceneNode();
	std::shared_ptr<PerspectiveCamera> createPerspectiveCamera();
	
protected:
	std::shared_ptr<Scene> m_scene;
};

}