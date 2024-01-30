#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vector>

#include <glm/glm.hpp>

class Mesh;
class Terrain;
class PhysicalObject;
/*================
Mesh factory class
================*/
class RenderMesh 
{
private:
	static float* GetObjectMesh(int n);
public:
	static Mesh* RenderTerrainMesh(Terrain* terrain);
	static Mesh* RenderPhysicalObjectMesh(const PhysicalObject* object);
	// render <objects> physical objects colliders
	static Mesh* RenderDebugPhysicsMesh(const std::vector<PhysicalObject*>& objects);
	// render <collisions> points of collision between physical objects
	static Mesh* RenderDebugPhysicsMesh(const std::vector<glm::vec3>& collisions);
};

#endif
