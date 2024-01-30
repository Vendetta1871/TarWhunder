#include "../../include/gameobject/TerrainGenerator.h"
#include "../../include/gameobject/Terrain.h"
#include "../../include/mathf/MathF.h"
#include "../../include/physics/PhysicalObject.h"

Terrain* TerrainGenerator::GenerateHeightsMap(int size)
{
	//TODO: add objects on map(building etc)
	float** h_map = new float* [size + 1];
	for (int i = 0; i <= size; ++i)
	{
		h_map[i] = new float[size + 1];
		memset(h_map[i], 0, (size + 1) * sizeof(float));
	}

	MathF::DiamondSquare(h_map, size, 35, 0.5f);
	h_map = MathF::SmoothArray(h_map, size);

	Terrain* terrain = new Terrain(h_map, size);
	return terrain;
}
