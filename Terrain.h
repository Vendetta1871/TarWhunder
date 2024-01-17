#ifndef TERRAIN_H
#define TERRAIN_H

class Terrain
{
private:
	float** h_map;
	const int sz;
public:
	Terrain(float** h_map, int size);
	float** GetHeightMap(int* size);
};

class TerrainGenerator
{
public:
	static Terrain* CreateTerrain();
};

#endif
