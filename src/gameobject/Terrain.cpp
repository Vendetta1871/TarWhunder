#include "../../include/gameobject/Terrain.h"
#include "../../include/mathf/MathF.h"
#include "../../include/physics/PhysicalObject.h"

#define GET_NORMAL_X(a, b, c, d, e, f, g, h, i) ((h-b)*(f-c) - (e-b)*(i-c))
#define GET_NORMAL_Y(a, b, c, d, e, f, g, h, i) ((d-a)*(i-c) - (g-a)*(f-c))
#define GET_NORMAL_Z(a, b, c, d, e, f, g, h, i) ((g-a)*(e-b) - (d-a)*(h-b))

Terrain::Terrain(const float*const* h_map, int size) : HeightsMap(h_map), HeightsMapSize(size)
{
	Object = new PhysicalObject(glm::vec3(0.0f), 0.0f, 0.0f, 0.0f);
}

Terrain::~Terrain()
{
	for (int i = 0; i < HeightsMapSize; ++i)
	{
		delete[]  HeightsMap[i];
	}
	delete[]  HeightsMap;
}

float Terrain::GetPointHeight(int x, int z) const
{
	return MathF::GetValueSafe(HeightsMap, HeightsMapSize, x, z);
}

glm::vec3 Terrain::GetTerrainNormal(float x, float z) const
{                                 
	int ix = (int)std::floor(x);  /*****************************************************/
	int iz = (int)std::floor(z);  /*  Search which polygon of square is this point in  */
	x = x - iz;                   /* (0, 0)                   (0, 0)                   */
	z = z - iz;                   /*   __________               __________             */
	                              /*   |       /|     /    \    |       /|     /    \  */
	int sign = 1;                 /*   |  (.) / |    |(0, 0)|   |      / |    |(1, 0)| */
	if (x + z > 1)                /*   |    /   | => |(1, 0)|   |    /   | => |(1, 1)| */
	{                             /*   |   /    |    |(0, 1)|   |   /    |    |(0, 1)| */
		sign = -1;                /*   | /      |     \    /    | / (.)  |     \    /  */
		ix += 1;                  /*   |/_______|               |/_______|             */
		iz += 1;                  /*          (1, 1)                   (1, 1)          */
	}							  /*****************************************************/

	float y0 = MathF::GetValueSafe(HeightsMap, HeightsMapSize, ix, iz);
	float y1 = MathF::GetValueSafe(HeightsMap, HeightsMapSize, ix + sign, iz);
	float y2 = MathF::GetValueSafe(HeightsMap, HeightsMapSize, ix, iz + sign);

	return MathF::Normalize(glm::vec3(
		GET_NORMAL_X(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign),
		GET_NORMAL_Y(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign),
		GET_NORMAL_Z(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign)));
}
