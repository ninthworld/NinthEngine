#pragma once

#include "SimpleShader.hpp"
#include "Chunk.hpp"
#include <NinthEngine\IGameLogic.hpp>
#include <NinthEngine\VertexArray.hpp>
#include <NinthEngine\Camera.hpp>

class TestGame : public IGameLogic {
public:
	TestGame();
	~TestGame();
	void init(Window *window);
	void input(Window *window);
	void update(const float interval);
	void render(Window *window);
protected:
private:
	SimpleShader *simpleShader;
	//VertexArray *simpleVAO;
	Camera *camera;
	Texture *simpleTexture;
	Chunk *chunk;
};
