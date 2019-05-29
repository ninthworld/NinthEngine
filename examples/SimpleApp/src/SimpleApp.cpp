#include <NinthEngine.h>

using namespace NinthEngine;

class SimpleApp : public App {
public:
	SimpleApp();
	virtual ~SimpleApp();

	void initialize();
};

App* NinthEngine::createApp() {
	return new SimpleApp();
}

SimpleApp::SimpleApp() : App("SimpleApp", 1280, 720) {}	
SimpleApp::~SimpleApp() {}

void SimpleApp::initialize() {
	App::initialize();

	Camera* camera = getScene()->createCamera("mainCamera");
	camera->setType(CameraType::Perspective);
	camera->setPosition(200, 300, 400);
	camera->lookAt(glm::vec3(0, 0, 0));
	getScene()->getRoot()->addChild(camera);

	Viewport* viewport = getWindow()->addViewport(camera);
	camera->setAspectRatio((float)viewport->getWidth() / (float)viewport->getHeight());

	Model* cubeModel = getResources()->loadModel("res/cube.obj");
	Entity* cubeEntity = getScene()->createEntity("cube", cubeModel);
	getScene()->getRoot()->addChild(cubeEntity);

	Light* light = getScene()->createLight("sun");
	light->setType(LightType::Directional);
	light->setDirection(0, -1, 1);
	getScene()->getRoot()->addChild(light);
}