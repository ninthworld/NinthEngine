#pragma once

#include <NinthEngine\Render\Blender.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLBlender : public Blender {
public:
	GLBlender(const BlenderStruct blender);
	~GLBlender();

	// Self
	const BlenderStruct getBlenderStruct() const { return m_blenderStruct; };

private:
	BlenderStruct m_blenderStruct;

};

} // namespace GL
} // namespace NinthEngine