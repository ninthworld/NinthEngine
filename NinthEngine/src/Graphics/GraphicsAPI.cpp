#include "pch.h"

#include "Graphics\GraphicsAPI.h"

namespace NinthEngine {

void GraphicsAPI::addGraphics(const std::string& name, const std::function<Graphics*()>& creator) {
	s_graphicsCreators.insert(std::pair<std::string, std::function<Graphics*()>>(name, creator));
	s_graphicsNames.push_back(name);
}

Graphics* GraphicsAPI::getGraphics(const std::string& name) {
	auto it = s_graphicsCreators.find(name);
	if (it != s_graphicsCreators.end()) return it->second();
	return nullptr;
}

std::vector<std::string> GraphicsAPI::listGraphics() {
	return s_graphicsNames;
}

}