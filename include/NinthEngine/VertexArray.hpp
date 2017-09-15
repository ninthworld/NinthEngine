#pragma once

#define ATTRIB_POSITION 0
#define ATTRIB_NORMAL	1
#define ATTRIB_TEXCOORD 2

#include <GL\glew.h>
#include <vector>

class VertexArray {
public:
	struct Vertex_t {
	public:
		float x, y, z;
	};

	struct VertexNT_t {
	public:
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};

	VertexArray();
	~VertexArray();
	void setData(std::vector<Vertex_t> vertices, std::vector<unsigned int> indices);
	void setData(std::vector<VertexNT_t> vertices, std::vector<unsigned int> indices);
	void render();
protected:
private:
	GLuint vaoId;
	GLuint vboId;
	GLuint iboId;
	GLsizei numIndices;
	bool hasNormals;
	bool hasTexCoords;
};