#pragma once

#include <string>
#include <exception>
#include <fstream>
#include <streambuf>

namespace NinthEngine {

class Texture;

namespace GameUtils {

#define PI			3.141592654
#define toRads(deg) (deg * (PI / 180.0))
#define toDegs(rad) (rad * (180.0 / PI))

class Exception : public std::exception {
public:
	Exception(const std::string msg);
	const char *what();
protected:
private:
	std::string msg;
};

const std::string readFile(const std::string file);
Texture *loadBMP(const std::string file);

} // namespace GameUtils
} // namespace NinthEngine