#pragma once

#include "NEPCH.h"

#include "Core.h"

namespace NinthEngine {

class NE_API Window {
public: 
	Window();
	virtual ~Window();

	virtual void initialize(const std::string& title, const unsigned int width, const unsigned int height) = 0;
	virtual void destroy() = 0;

	virtual std::string getTitle();
	virtual const unsigned int getWidth();
	virtual const unsigned int getHeight();

protected:
	std::string m_title;
	unsigned int m_width, m_height;
};

}