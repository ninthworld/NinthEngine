#include "BlockGame.hpp"
#include "Chunk.hpp"
#include "SimpleShader.hpp"
#include <NinthEngine\Window.hpp>
#include <NinthEngine\GameUtils.hpp>

namespace TestGame {

BlockGame::BlockGame() :
	camera(new Camera(glm::vec3(0), glm::vec3(0))) {
}

BlockGame::~BlockGame() {
	delete simpleShader;
	//delete simpleVAO;
	delete camera;
	delete simpleTexture;
	delete chunk;
}

void BlockGame::init(Window *window) {
	window->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	camera->init(window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	simpleTexture = GameUtils::loadBMP("blocks.bmp");

	chunk = new Chunk({ 0, 0 });
	chunk->init();

	/*struct UV_t {
		float u, v;
		UV_t operator +(UV_t b) { return { u + b.u, v + b.v }; }
		UV_t operator *(UV_t b) { return { u * b.u, v * b.v }; }
	};

	UV_t size{ 1 / 16.f, 1 / 16.f };
	UV_t grassTop{ 0, 13 };
	UV_t grassBot{ 0, 14 };
	UV_t grassSide{ 1, 14 };

	std::vector<VertexArray::VertexNT_t> vertices {
		// Top
		{ 0, 1, 0, 0, 1, 0, grassTop.u * size.u, grassTop.v * size.v },
		{ 0, 1, 1, 0, 1, 0, grassTop.u * size.u, (1 + grassTop.v) * size.v },
		{ 1, 1, 0, 0, 1, 0, (1 + grassTop.u) * size.u, grassTop.v * size.v },
		{ 1, 1, 1, 0, 1, 0, (1 + grassTop.u) * size.u, (1 + grassTop.v) * size.v },
		// Bottom
		{ 1, 0, 0, 0, -1, 0, grassBot.u * size.u, grassBot.v * size.v },
		{ 1, 0, 1, 0, -1, 0, grassBot.u * size.u, (1 + grassBot.v) * size.v },
		{ 0, 0, 0, 0, -1, 0, (1 + grassBot.u) * size.u, grassBot.v * size.v },
		{ 0, 0, 1, 0, -1, 0, (1 + grassBot.u) * size.u, (1 + grassBot.v) * size.v },
		// Front
		{ 1, 0, 1, 0, 0, 1, grassSide.u * size.u, grassSide.v * size.v },
		{ 1, 1, 1, 0, 0, 1, grassSide.u * size.u, (1 + grassSide.v) * size.v },
		{ 0, 0, 1, 0, 0, 1, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 0, 1, 1, 0, 0, 1, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Back
		{ 0, 0, 0, 0, 0, -1, grassSide.u * size.u, grassSide.v * size.v },
		{ 0, 1, 0, 0, 0, -1, grassSide.u * size.u, (1 + grassSide.v) * size.v  },
		{ 1, 0, 0, 0, 0, -1, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 1, 1, 0, 0, 0, -1, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Right
		{ 1, 0, 0, 1, 0, 0, grassSide.u * size.u, grassSide.v * size.v },
		{ 1, 1, 0, 1, 0, 0, grassSide.u * size.u, (1 + grassSide.v) * size.v  },
		{ 1, 0, 1, 1, 0, 0, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 1, 1, 1, 1, 0, 0, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Left
		{ 0, 0, 1, -1, 0, 0, grassSide.u * size.u, grassSide.v * size.v },
		{ 0, 1, 1, -1, 0, 0, grassSide.u * size.u, (1 + grassSide.v) * size.v },
		{ 0, 0, 0, -1, 0, 0, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 0, 1, 0, -1, 0, 0, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v }
	};

	std::vector<unsigned int> indices {
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9, 10, 10, 9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23
	};*/

	simpleShader = new SimpleShader();
	simpleShader->init();
	//simpleVAO = new VertexArray();
	//simpleVAO->setData(vertices, indices);
}

void BlockGame::input(Window *window) {
	camera->input(window);
}

void BlockGame::update(const float interval) {
	camera->update(interval);
}

void BlockGame::render(Window *window) {
	if (window->isResized()) {
		glViewport(0, 0, window->getWidth(), window->getHeight());
		camera->init(window);
		window->setResized(false);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	simpleShader->bind();
	simpleShader->loadViewProjMatrix(camera->getViewProjMatrix());
	simpleShader->loadModelMatrix(glm::translate(glm::mat4(), glm::vec3(0)));
	simpleShader->loadTexture(simpleTexture);
	//simpleVAO->render();
	chunk->render();
	simpleShader->unbind();
}

} // namespace TestGame