#pragma once

#ifdef NE_PLATFORM_WIN32

extern NinthEngine::Application* NinthEngine::createApplication();

int main(int argc, char** argv) {

	NinthEngine::Log::init();
	NinthEngine::Log::info("Initialized Log");

	auto app = NinthEngine::createApplication();
	app->create();
	while (app->isRunning()) {
		app->update();
		app->getGraphics()->swapBuffers();
		app->getWindow()->update();
	}
	delete app;

	return 0;
}

#endif