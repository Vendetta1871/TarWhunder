#ifndef PHYSICS_PROCESSING_H
#define PHYSICS_PROCESSING_H

#include <glm/glm.hpp>

class PhysicalObject;

class PhysicsProcessing
{
private:
	static float** h_map;
	static int h_map_size;

	static int count;

	static int Collide(glm::vec3* point, glm::vec3* n, float* len);
public:
	static PhysicalObject* object;

	static void InitPhysics(float** height_map, int n, PhysicalObject* object);

	static void ComputePhysics(float time);
};

#endif
