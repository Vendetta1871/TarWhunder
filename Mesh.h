#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

class Mesh
{
private:
	unsigned int vao;
	unsigned int vbo;
	size_t vertixes;
public:
	Mesh(const float* buffer, size_t vertixes, const int* attrs);
	~Mesh();

	void Draw(unsigned int primitive);
};

#endif
