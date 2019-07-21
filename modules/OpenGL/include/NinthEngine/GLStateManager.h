#pragma once

#include "GLPCH.h"
#include "GLCore.h"

namespace NinthEngine {

template <class T>
class NE_GL_API GLStateManager {
public:
	GLStateManager(T* default) : m_default(default), m_stack(std::vector<T*>(0)) {}
	~GLStateManager() {}

	void push(T* object) {
		if (object == nullptr) {
			m_stack.push_back(m_default);
			return;
		}
		m_stack.push_back(object);
	}

	T* pop() {
		if (m_stack.empty()) return m_default;
		T* object = m_stack.back();
		m_stack.pop_back();
		return object;
	}

	T* peak() {
		if (m_stack.empty()) return m_default;
		return m_stack.back();
	}

	void clear() {
		m_stack.clear();
	}

	void setDefault(T* default) { m_default = default; }
	
private:
	T* m_default;
	std::vector<T*> m_stack;
};

}