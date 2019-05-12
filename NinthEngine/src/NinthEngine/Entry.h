#pragma once

#ifdef NE_PLATFORM_WIN32

extern NinthEngine::App* NinthEngine::createApp();

int main(int argc, char** argv) {

	NinthEngine::Log::init();
	NE_WARN("Initialized Log");

	auto app = NinthEngine::createApp();
	app->run();
	delete app;

	return 0;
}

#endif