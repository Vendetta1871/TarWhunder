#include "Terrain.h"

Terrain::Terrain(float** h_map, int size) : h_map(h_map), sz(size)
{

}

Terrain::~Terrain()
{
	for (int i = 0; i < sz; ++i)
	{
		delete[] h_map[i];
	}
	delete[] h_map;
}

float** Terrain::GetHeightMap(int* size)
{
	*size = sz;
	return h_map;
}
