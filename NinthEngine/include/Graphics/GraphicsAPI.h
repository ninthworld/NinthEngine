#pragma once

#include "NEPCH.h"

#include "Core.h"
#include "Graphics\Graphics.h"

namespace NinthEngine {

class NE_API GraphicsAPI {
public:
	static void addGraphics(const std::string& name, const std::function<Graphics*()>& creator);
	static Graphics* getGraphics(const std::string& name);
	static std::vector<std::string> listGraphics();
private:
	static std::vector<std::string> s_graphicsNames;
	static std::map<std::string, std::function<Graphics*()>> s_graphicsCreators;
};

}