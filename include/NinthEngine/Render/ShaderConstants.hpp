#pragma once

#include <string>
#include <map>

namespace NinthEngine {

class ShaderConstants {
public:
	ShaderConstants();
	~ShaderConstants();

	ShaderConstants& add(const std::string name, const unsigned typeSize);

	std::map<std::string, unsigned> getConstants() { return constants; };

private:
	std::map<std::string, unsigned> constants;

};

} // namespace NinthEngine