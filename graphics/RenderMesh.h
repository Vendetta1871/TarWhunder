#ifndef RENDER_MESH_H
#define RENDER_MESH_H

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
};

#endif
