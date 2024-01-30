#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

class Terrain;
/*===================
Terrain factory class
===================*/
class TerrainGenerator
{
public:
	// returns a diamond square algorithm generated square <size>*<size> heights map
	static Terrain* GenerateHeightsMap(int size);
};

#endif
