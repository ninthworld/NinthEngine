#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class GLContext;
class GameWindow;

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(const std::shared_ptr<GLContext>&, const std::shared_ptr<GameWindow>&);
	~GLGraphicsContext();
	
private:
	std::shared_ptr<GLContext> glContext;
	std::shared_ptr<GameWindow> window;

};

} // namespace NinthEngine
