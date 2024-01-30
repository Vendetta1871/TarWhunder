#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>

class PhysicalObject;
/*=================
Terrain game object
=================*/
class Terrain
{
public:
	//TODO: add objects on map (building etc)
	const float* const* HeightsMap;
	const int HeightsMapSize;

	PhysicalObject* Object; // physical object of terrain

	Terrain(const float*const* h_map, int size);
	~Terrain();
	// returns height of point in <x, y> position
	float GetPointHeight(int x, int z) const;
	// returns the normal vector of terrain polygon in <x, y> position 
	glm::vec3 GetTerrainNormal(float x, float z) const;
};

#endif
