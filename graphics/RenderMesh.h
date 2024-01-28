#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vector>
#include <glm/glm.hpp>

class Mesh;
class Terrain;
class PhysicalObject;

class RenderMesh 
{
private:
	static float* GetObjectMesh(int n);
public:
	static Mesh* RenderTerrainMesh(Terrain* terrain);
	static Mesh* RenderPhysicalObjectMesh(PhysicalObject* object);
	static Mesh* RenderDebugPhysicsMesh(const std::vector<PhysicalObject*>& object);
	static Mesh* RenderDebugPhysicsMesh(const std::vector<glm::vec3>& collisions);
};

#endif
