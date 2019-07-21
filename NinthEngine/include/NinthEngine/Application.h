#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "NinthEngine/Window/Window.h"
#include "NinthEngine/Graphics/Graphics.h"
#include "NinthEngine/Resource/ResourceManager.h"
#include "NinthEngine/Scene/SceneManager.h"

namespace NinthEngine {

struct NE_API ApplicationProps {
	using GraphicsFunc = std::function<Graphics*(const GraphicsProps&, Window*)>;

	GraphicsFunc graphicsFunc;
	WindowProps windowProps;
	GraphicsProps graphicsProps;

	ApplicationProps(const GraphicsFunc& graphicsFunc, const WindowProps& windowProps = WindowProps(), const GraphicsProps& graphicsProps = GraphicsProps())
		: graphicsFunc(graphicsFunc), windowProps(windowProps), graphicsProps(graphicsProps) {}
};

class NE_API Application {
public:
	Application(const ApplicationProps& props);
	virtual ~Application();
		
	virtual void create();
	virtual void update();

	virtual const bool isRunning() const { return m_running; }
	virtual void setIsRunning(const bool running) { m_running = running; }

	Graphics* getGraphics();
	Window* getWindow();
	SceneManager* getSceneManager();
	ResourceManager* getResourceManager();

private:
	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<SceneManager> m_sceneManager;
	std::unique_ptr<ResourceManager> m_resourceManager;

	bool m_running;
};

static Application* createApplication();

}