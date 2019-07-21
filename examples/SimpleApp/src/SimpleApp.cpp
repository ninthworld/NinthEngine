#include <NinthEngine.h>
#include <NinthEngine/Scene/Camera/PerspectiveCamera.h>
#include <NinthEngine/Scene/Light/Light.h>
#include <NinthEngine/GLGraphics.h>

using namespace NinthEngine;

class SimpleApp : public Application, WindowListener {
public:
	SimpleApp(const ApplicationProps& props) : Application(props) {}
	~SimpleApp() {}

	void create() override {
		getWindow()->addWindowListener(this);

		Camera* camera = getSceneManager()->getRoot()->createPerspectiveCamera(PerspectiveCameraProps());
		camera->setLocalPosition(glm::vec3(100, 100, 100));
		camera->lookAt(glm::vec3(0, 0, 0));

		Light* ambientLight = getSceneManager()->getRoot()->createAmbientLight(LightProps(Color(1, 1, 1)));

		// Model* model = getResourceManager()->loadModel("resources/cube.obj");

		/*
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
		*/
	}

	bool onWindowClosed(const WindowClosedEvent& event) override {
		setIsRunning(false);
		return true;
	}
};

Application* NinthEngine::createApplication() {
	return new SimpleApp(ApplicationProps(GLGraphics::create, WindowProps("SimpleApp", 1280, 720)));
}