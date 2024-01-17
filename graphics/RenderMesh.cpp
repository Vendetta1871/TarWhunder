#include "RenderMesh.h"
#include "../Math.h"

Mesh* RenderMesh::RenderTerrainMesh(Terrain* terrain)
{
	int size;
	float** h_map = terrain->GetHeightMap(&size);

	int attrs[] = { 3, 2, 3, 0 };
	float* vertixes = new float[(size - 2) * (size - 2) * 48];
		
	int k = 0;
	for (int x = 1; x < (size - 1); ++x)
	{
		for (int z = 1; z < (size - 1); ++z)
		{
			vertixes[k + 0] = x;
			vertixes[k + 1] = h_map[x][z];
			vertixes[k + 2] = z;
			vertixes[k + 3] = 0.0f;
			vertixes[k + 4] = 0.0f;
			Math::GetVertexNormal(h_map, size, x, z,
				vertixes + k + 5, vertixes + k + 6, vertixes + k + 7);

			vertixes[k + 8] = x;
			vertixes[k + 9] = h_map[x][z + 1];
			vertixes[k + 10] = z + 1;
			vertixes[k + 11] = 0.0f;
			vertixes[k + 12] = 1.0f;
			Math::GetVertexNormal(h_map, size, x, z + 1,
				vertixes + k + 13, vertixes + k + 14, vertixes + k + 15);

			vertixes[k + 16] = x + 1;
			vertixes[k + 17] = h_map[x + 1][z];
			vertixes[k + 18] = z;
			vertixes[k + 19] = 1.0f;
			vertixes[k + 20] = 0.0f;
			Math::GetVertexNormal(h_map, size, x + 1, z,
				vertixes + k + 21, vertixes + k + 22, vertixes + k + 23);

			vertixes[k + 24] = vertixes[k + 0];
			vertixes[k + 25] = vertixes[k + 1];
			vertixes[k + 26] = vertixes[k + 2];
			vertixes[k + 27] = 1.0f;
			vertixes[k + 28] = 0.0f;
			vertixes[k + 29] = vertixes[k + 5];
			vertixes[k + 30] = vertixes[k + 6];
			vertixes[k + 31] = vertixes[k + 7];

			vertixes[k + 32] = x - 1;
			vertixes[k + 33] = h_map[x - 1][z + 1];
			vertixes[k + 34] = z + 1;
			vertixes[k + 35] = 0.0f;
			vertixes[k + 36] = 1.0f;
			Math::GetVertexNormal(h_map, size, x - 1, z + 1,
				vertixes + k + 37, vertixes + k + 38, vertixes + k + 39);

			vertixes[k + 40] = x;
			vertixes[k + 41] = h_map[x][z + 1];
			vertixes[k + 42] = z + 1;
			vertixes[k + 43] = 1.0f;
			vertixes[k + 44] = 1.0f;
			Math::GetVertexNormal(h_map, size, x, z + 1,
				vertixes + k + 45, vertixes + k + 46, vertixes + k + 47);
			k += 48;
		}
	}

	Mesh* mesh = new Mesh(vertixes, (size - 2) * (size - 2) * 48 / 8, attrs);
	return mesh;
}
