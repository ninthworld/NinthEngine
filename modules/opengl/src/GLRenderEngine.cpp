#include "Render\Win32\Win32GLContext.hpp"
#include "..\include\NinthEngineGL\GLRenderEngine.hpp"

namespace NinthEngine {
namespace GL {

GLRenderEngine::GLRenderEngine() {
}

GLRenderEngine::~GLRenderEngine() {
}

void GLRenderEngine::init(const std::shared_ptr<GameWindow>& window, void* hwnd) {
	
	if (hwnd) {
		auto glContext = std::make_unique<Win32::Win32GLContext>(hwnd);

		if (glewInit() != GLEW_OK) {
			LOG_ERROR << "Failed to initialize GLEW";
			throw std::exception();
		}

		glContext->makeCurrent();

		m_device = std::make_unique<GLGraphicsDevice>();
		m_context = std::make_unique<GLGraphicsContext>(std::move(glContext), window);
	}
}

} // namespace GL
} // namespace NinthEngine