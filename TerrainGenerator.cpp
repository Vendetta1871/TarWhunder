#include "TerrainGenerator.h"
#include "Terrain.h"
#include "physics/Math.h"
#include "physics/PhysicalObject.h"

Terrain* TerrainGenerator::CreateTerrain()
{
	int size = 64;
	float** h_map = new float* [size + 1];
	for (int i = 0; i <= size; ++i)
	{
		h_map[i] = new float[size + 1];
	}
	h_map[0][0] = 0;
	h_map[0][size] = 0;
	h_map[size][0] = 0;
	h_map[size][size] = 0;

	Math::DiamondSquare(h_map, size, 1, 1, 30, 0.5f);
	h_map = Math::SmoothArray(h_map, size);

	Terrain* terrain = new Terrain(h_map, size);
	return terrain;
}
