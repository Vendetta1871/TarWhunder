#include "Terrain.h"
#include "Math.h"

float** Terrain::GetHeightMap(int* size) 
{
	*size = sz;
	return h_map;
}

Terrain::Terrain(float** h_map, int size) : h_map(h_map), sz(size)
{

}

Terrain* TerrainGenerator::CreateTerrain() 
{
	int size = 1024;
	float** h_map = new float*[size + 1];
	for (int i = 0; i <= size; ++i)
	{
		h_map[i] = new float[size + 1];
	}
	h_map[0][0] = 0;
	h_map[0][size] = 0;
	h_map[size][0] = 0;
	h_map[size][size] = 0;

	Math::DiamondSquare(h_map, size, 1, 1, 35, 0.6f);
	h_map = Math::SmoothArray(h_map, size);

	Terrain* terrain = new Terrain(h_map, size);
	return terrain;
}
