#ifndef RENDERMESH_H
#define RENDERMESH_H

#include "Mesh.h"
#include "../Terrain.h"

#include <glm/glm.hpp>

class RenderMesh {
public:
	static Mesh* RenderTerrainMesh(Terrain* terrain);
};

#endif
