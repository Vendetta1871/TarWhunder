#ifndef PHYSICS_PROCESSING_H
#define PHYSICS_PROCESSING_H

#include <glm/glm.hpp>
#include <vector>

class PhysicalObject;

class PhysicsProcessing
{
private:
	static float** h_map;
	static int h_map_size;

	static PhysicalObject* terrain;

	static bool Collide();
public:
	static std::vector<PhysicalObject*> object;

	static void InitPhysics(float** height_map, int n, std::vector<PhysicalObject*>* object);

	static void ComputePhysics(float time);
};

#endif
