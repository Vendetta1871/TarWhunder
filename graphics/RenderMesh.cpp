#include "RenderMesh.h"

#include <glm/ext.hpp>

#include "Mesh.h"
#include "../Terrain.h"
#include "../physics/Math.h"
#include "../physics/PhysicalObject.h"
#include "../physics/Collider.h"

#include "../ObjectMeshArrays.h"

#include <iostream>

int attrs[] = { 3, 2, 3, 0 };

Mesh* RenderMesh::RenderTerrainMesh(Terrain* terrain)
{
	int size;
	float** h_map = terrain->GetHeightMap(&size);
	float* vertixes = new float[(size - 2) * (size - 2) * 48];
		
	int k = 0;
	glm::vec3 n = glm::vec3(0);
	for (int x = 1; x < (size - 1); ++x)
	{
		for (int z = 1; z < (size - 1); ++z)
		{
			vertixes[k + 0] = vertixes[k + 24] = (float)x;
			vertixes[k + 1] = vertixes[k + 25] = h_map[x][z];
			vertixes[k + 2] = vertixes[k + 26] = (float)z;
			vertixes[k + 3] = 0.0f;
			vertixes[k + 4] = 0.0f;
			n = Math::Normalize(Math::GetVertexSumNormal(h_map, size, x, z));
			vertixes[k + 5] = vertixes[k + 29] = n.x;
			vertixes[k + 6] = vertixes[k + 30] = n.y;
			vertixes[k + 7] = vertixes[k + 31] = n.z;
			vertixes[k + 8] = vertixes[k + 40] = (float)x;
			vertixes[k + 9] = vertixes[k + 41] = h_map[x][z + 1];
			vertixes[k + 10] = vertixes[k + 42] = (float)(z + 1);
			vertixes[k + 11] = 0.0f;
			vertixes[k + 12] = 1.0f;
			n = Math::Normalize(Math::GetVertexSumNormal(h_map, size, x, z + 1));
			vertixes[k + 13] = vertixes[k + 45] = n.x;
			vertixes[k + 14] = vertixes[k + 46] = n.y;
			vertixes[k + 15] = vertixes[k + 47] = n.z;
			vertixes[k + 16] = (float)(x + 1);
			vertixes[k + 17] = h_map[x + 1][z];
			vertixes[k + 18] = (float)z;
			vertixes[k + 19] = 1.0f;
			vertixes[k + 20] = 0.0f;
			n = Math::Normalize(Math::GetVertexSumNormal(h_map, size, x + 1, z));
			vertixes[k + 21] = n.x;
			vertixes[k + 22] = n.y;
			vertixes[k + 23] = n.z;
			vertixes[k + 27] = 1.0f;
			vertixes[k + 28] = 0.0f;
			vertixes[k + 32] = (float)(x - 1);
			vertixes[k + 33] = h_map[x - 1][z + 1];
			vertixes[k + 34] = (float)(z + 1);
			vertixes[k + 35] = 0.0f;
			vertixes[k + 36] = 1.0f;
			n = Math::Normalize(Math::GetVertexSumNormal(h_map, size, x - 1, z + 1));
			vertixes[k + 37] = n.x;
			vertixes[k + 38] = n.y;
			vertixes[k + 39] = n.z;
			vertixes[k + 43] = 1.0f;
			vertixes[k + 44] = 1.0f;
			k += 48;
		}
	}

	Mesh* mesh = new Mesh(vertixes, (size - 2) * (size - 2) * 48 / 8, attrs);
	delete[] vertixes;
	return mesh;
}

float* RenderMesh::GetObjectMesh(int n)
{
	return new float[1];
}

Mesh* RenderMesh::RenderPhysicalObjectMesh(PhysicalObject* object)
{
	int type = object->type;
	Mesh* mesh = new Mesh(vertixesCube, sizeCube, attrs);
	return mesh;
}

Mesh* RenderMesh::RenderDebugPhysicsMesh(const std::vector<PhysicalObject*>& object)
{
	int a[] = { 3, 0 };
	int count = object.size() * 36;
	float* vertixes = new float[count * 3];
	int j = 0;
	for (auto obj : object)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int k = i + 1; k < 8; ++k)
			{
				if (i + k == 7 || (k - i) % 3 == 0)
				{
					continue;
				}

				vertixes[j + 0] = obj->collider->Points[i].x;
				vertixes[j + 1] = obj->collider->Points[i].y;
				vertixes[j + 2] = obj->collider->Points[i].z;
				vertixes[j + 3] = obj->collider->Points[k].x;
				vertixes[j + 4] = obj->collider->Points[k].y;
				vertixes[j + 5] = obj->collider->Points[k].z;
				j += 6;
			}
		}
	}
	Mesh* mesh = new Mesh(vertixes, count, a);
	delete[] vertixes;
	return mesh;
}

Mesh* RenderMesh::RenderDebugPhysicsMesh(const std::vector<glm::vec3>& collisions)
{
	int a[] = { 3, 0 };
	int count = collisions.size();
	float* vertixes = new float[count * 3];
	int i = 0;
	for (auto& col : collisions)
	{
		vertixes[i + 0] = col.x;
		vertixes[i + 1] = col.y;
		vertixes[i + 2] = col.z;
		i += 3;
	}
	Mesh* mesh = new Mesh(vertixes, count, a);
	delete[] vertixes;
	return mesh;
}
