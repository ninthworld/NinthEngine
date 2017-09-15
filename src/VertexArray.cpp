#include "../include/NinthEngine/VertexArray.hpp"

VertexArray::VertexArray() :
	numIndices(0),
	hasNormals(false),
	hasTexCoords(false) {
	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &vboId);
	glGenBuffers(1, &iboId);
};

VertexArray::~VertexArray() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}

void VertexArray::setData(std::vector<Vertex_t> vertices, std::vector<unsigned int> indices) {
	numIndices = indices.size();

	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_t), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, false, sizeof(Vertex_t), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void VertexArray::setData(std::vector<VertexNT_t> vertices, std::vector<unsigned int> indices) {
	numIndices = indices.size();
	hasNormals = true;
	hasTexCoords = true;

	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexNT_t), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, false, sizeof(VertexNT_t), 0);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, false, sizeof(VertexNT_t), (int*)(3 * sizeof(float)));
	glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, sizeof(VertexNT_t), (int*)(6 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexArray::render() {
	glBindVertexArray(vaoId);
	glEnableVertexAttribArray(ATTRIB_POSITION);
	if (hasNormals) glEnableVertexAttribArray(ATTRIB_NORMAL);
	if (hasTexCoords) glEnableVertexAttribArray(ATTRIB_TEXCOORD);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(ATTRIB_POSITION);
	if (hasNormals) glDisableVertexAttribArray(ATTRIB_NORMAL);
	if (hasTexCoords) glDisableVertexAttribArray(ATTRIB_TEXCOORD);
	glBindVertexArray(0);
}