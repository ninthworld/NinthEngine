#pragma once

#include "NinthEngine/Core.h"
#include "NinthEngine/Window/Window.h"
#include "NinthEngine/Graphics/Graphics.h"
#include "NinthEngine/Event/Event.h"
#include "NinthEngine/Event/WindowEvent.h"
#include "NinthEngine/Event/KeyEvent.h"
#include "NinthEngine/Event/MouseEvent.h"
#include "NinthEngine/Input/InputCodes.h"

#include "NinthEngine/Graphics/Shader.h"
#include "NinthEngine/Graphics/VertexArray.h"

namespace NinthEngine {

class NE_API App {
public:
	App(const WindowProps& windowProps = WindowProps(), 
		const GraphicsProps& graphicsProps = GraphicsProps());
	virtual ~App();

	std::shared_ptr<Window> getWindow() { return m_window; }
	std::shared_ptr<Graphics> getGraphics() { return m_graphics; }

	void onEvent(Event& e);
	virtual bool onWindowClosed(WindowClosedEvent& e);
	virtual bool onKeyPressed(KeyPressedEvent& e);

	virtual void initialize() = 0;
	virtual void render() = 0;

	void run();

private:
	std::shared_ptr<Window> m_window;
	std::shared_ptr<Graphics> m_graphics;
	bool m_running;
};

App* createApp();

}
