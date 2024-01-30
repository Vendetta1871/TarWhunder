#ifndef PHYSICS_PROCESSING_H
#define PHYSICS_PROCESSING_H

#include <vector>

#include <glm/glm.hpp>

class Terrain;
class PhysicalObject;
/*======================================
Simulate physics processes in game world
======================================*/
class PhysicsProcessing
{
private:
	static const Terrain* terrain;
	static std::vector<PhysicalObject*> objects;
public:
	// set the <terrain> terrain and <objects> physical objects
	static void InitPhysics(const Terrain* terrain, const std::vector<PhysicalObject*>& objects);
	// integrate forces, moving and collisions in <time> time
	static void SimulatePhysics(float time);
};

#endif
