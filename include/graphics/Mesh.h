#ifndef MESH_H
#define MESH_H

#include <stdlib.h>
/*====================
Handling meshes class
====================*/
class Mesh
{
private:
	const int* attrs; // mesh layout attributes
	unsigned int vao;
	unsigned int vbo;
	size_t vertexes;
public:
	Mesh(const float* buffer, size_t vertixes, const int* attrs);
	~Mesh();
	// draw mesh using current shader in <primitive> primitives
	void Draw(unsigned int primitive);

	const int* GetAttrs();
	int GetVertexesCount() const;
};

#endif
