#pragma once

#define PI			3.141592654
#define toRads(deg) (deg * (PI / 180.0))
#define toDegs(rad) (rad * (180.0 / PI))

#include "Texture.hpp"
#include <string>
#include <exception>
#include <fstream>
#include <streambuf>

namespace GameUtils {
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
}