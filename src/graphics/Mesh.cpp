#include "../../include/graphics/Mesh.h"

#include <GL/glew.h>

Mesh::Mesh(const float* buffer, size_t vertexes, const int* attrs) : vertexes(vertexes), attrs(attrs)
{
	// count of numbers representing one vertex
	int vertex_size = 0; 
	for (int i = 0; attrs[i] != 0; i++)
	{
		vertex_size += attrs[i];
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size * vertexes, buffer, GL_STATIC_DRAW);
	// send the array data according to the target shader layout
	int offset = 0;
	for (int i = 0; attrs[i] != 0; i++) 
	{
		int size = attrs[i];
		glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (GLvoid*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += size;
	}

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Mesh::Draw(unsigned int primitive)
{
	glBindVertexArray(vao);
	glDrawArrays(primitive, 0, vertexes);
	glBindVertexArray(0);
}

const int* Mesh::GetAttrs()
{
	return attrs;
}

int Mesh::GetVertexesCount() const
{
	return vertexes;
}
