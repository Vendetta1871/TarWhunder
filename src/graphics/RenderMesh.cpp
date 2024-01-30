#include "../../include/graphics/RenderMesh.h"
#include "../../include/graphics/Mesh.h"
#include "../../include/mathf/MathF.h"
#include "../../include/mathf/Collider.h"
#include "../../include/physics/PhysicalObject.h"
#include "../../include/gameobject/Terrain.h"
#include "../../resources/models/ObjectMeshArrays.h"

#include <iostream>

#include <glm/ext.hpp>

float* RenderMesh::GetObjectMesh(int n)
{
	//TODO: implement
	return new float[1];
}

glm::vec3 GetVertexAverageNormal(const Terrain* terrain, int x, int z)  /***************************************************/
{																		/* Return average normal of all connected polygons */
	glm::vec3 n0 = terrain->GetTerrainNormal(x + 0.1f, z + 0.1f);		/* _________________                               */
	glm::vec3 n1 = terrain->GetTerrainNormal(x - 0.1f, z + 0.2f);		/* |      /|      /|                               */
	glm::vec3 n2 = terrain->GetTerrainNormal(x - 0.2f, z + 0.1f);		/* |    /  |  4 /  |                               */
	glm::vec3 n3 = terrain->GetTerrainNormal(x - 0.1f, z - 0.1f);		/* |  /  3 |  /  5 |                               */
	glm::vec3 n4 = terrain->GetTerrainNormal(x + 0.1f, z - 0.2f);		/* |/______p/______| p_n = (n0+n1+n2+n3+n4+n5) / 6 */
	glm::vec3 n5 = terrain->GetTerrainNormal(x + 0.2f, z + 0.1f);		/* |      /|      /|                               */
																		/* |  2 /  | 0  /  |                               */
	return MathF::Normalize(n0 + n1 + n2 + n3 + n4 + n5);				/* |  /  1 |  /    |                               */
}																		/* |/______|/______|                               */
																		/***************************************************/
Mesh* RenderMesh::RenderTerrainMesh(Terrain* terrain)
{
	int size = terrain->HeightsMapSize;
	const int attrs[] = { 3, 2, 3, 0 }; // 3 - position, 2 - uv position, 3 - normals
	float* vertexes = new float[(size - 2) * (size - 2) * 48];
		
	int k = 0;
	glm::vec3 n = glm::vec3(0);
	for (int x = 1; x < (size - 1); ++x)
	{
		for (int z = 1; z < (size - 1); ++z)
		{
			vertexes[k + 0] = vertexes[k + 24] = (float)x;
			vertexes[k + 1] = vertexes[k + 25] = terrain->HeightsMap[x][z];
			vertexes[k + 2] = vertexes[k + 26] = (float)z;
			vertexes[k + 3] = 0.0f;
			vertexes[k + 4] = 0.0f;
			n = GetVertexAverageNormal(terrain, x, z);
			vertexes[k + 5] = vertexes[k + 29] = n.x;
			vertexes[k + 6] = vertexes[k + 30] = n.y;
			vertexes[k + 7] = vertexes[k + 31] = n.z;
			vertexes[k + 8] = vertexes[k + 40] = (float)x;
			vertexes[k + 9] = vertexes[k + 41] = terrain->HeightsMap[x][z + 1];
			vertexes[k + 10] = vertexes[k + 42] = (float)(z + 1);
			vertexes[k + 11] = 0.0f;
			vertexes[k + 12] = 1.0f;
			n = GetVertexAverageNormal(terrain, x, z + 1);
			vertexes[k + 13] = vertexes[k + 45] = n.x;
			vertexes[k + 14] = vertexes[k + 46] = n.y;
			vertexes[k + 15] = vertexes[k + 47] = n.z;
			vertexes[k + 16] = (float)(x + 1);
			vertexes[k + 17] = terrain->HeightsMap[x + 1][z];
			vertexes[k + 18] = (float)z;
			vertexes[k + 19] = 1.0f;
			vertexes[k + 20] = 0.0f;
			n = GetVertexAverageNormal(terrain, x + 1, z);
			vertexes[k + 21] = n.x;
			vertexes[k + 22] = n.y;
			vertexes[k + 23] = n.z;
			vertexes[k + 27] = 1.0f;
			vertexes[k + 28] = 0.0f;
			vertexes[k + 32] = (float)(x - 1);
			vertexes[k + 33] = terrain->HeightsMap[x - 1][z + 1];
			vertexes[k + 34] = (float)(z + 1);
			vertexes[k + 35] = 0.0f;
			vertexes[k + 36] = 1.0f;
			n = GetVertexAverageNormal(terrain, x - 1, z + 1);
			vertexes[k + 37] = n.x;
			vertexes[k + 38] = n.y;
			vertexes[k + 39] = n.z;
			vertexes[k + 43] = 1.0f;
			vertexes[k + 44] = 1.0f;
			k += 48;
		}
	}

	Mesh* mesh = new Mesh(vertexes, (size - 2) * (size - 2) * 6, attrs);
	delete[] vertexes;
	return mesh;
}

Mesh* RenderMesh::RenderPhysicalObjectMesh(const PhysicalObject* object)
{
	//TODO: different meshes depends of object type
	int type = object->type;
	const int attrs[] = { 3, 2, 3, 0 }; // 3 - position, 2 - uv position, 3 - normals
	Mesh* mesh = new Mesh(vertexesCube, sizeCube, attrs);
	return mesh;
}

Mesh* RenderMesh::RenderDebugPhysicsMesh(const std::vector<PhysicalObject*>& object)
{
	int count = object.size() * 36;
	const int attrs[] = { 3, 0 };
	float* vertexes = new float[count * 3];
	int k = 0;
	for (auto obj : object)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = i + 1; j < 8; ++j)
			{
				if (i + j == 7 || (j - i) % 3 == 0)
				{
					continue;
				}

				vertexes[k + 0] = obj->BoxCollider->OBB[i].x;
				vertexes[k + 1] = obj->BoxCollider->OBB[i].y;
				vertexes[k + 2] = obj->BoxCollider->OBB[i].z;
				vertexes[k + 3] = obj->BoxCollider->OBB[j].x;
				vertexes[k + 4] = obj->BoxCollider->OBB[j].y;
				vertexes[k + 5] = obj->BoxCollider->OBB[j].z;
				k += 6;
			}
		}
	}
	Mesh* mesh = new Mesh(vertexes, count, attrs);
	delete[] vertexes;
	return mesh;
}

Mesh* RenderMesh::RenderDebugPhysicsMesh(const std::vector<glm::vec3>& collisions)
{
	int count = collisions.size();
	const int attrs[] = { 3, 0 };
	float* vertexes = new float[count * 3];
	int i = 0;
	for (auto& col : collisions)
	{
		vertexes[i + 0] = col.x;
		vertexes[i + 1] = col.y;
		vertexes[i + 2] = col.z;
		i += 3;
	}
	Mesh* mesh = new Mesh(vertexes, count, attrs);
	delete[] vertexes;
	return mesh;
}
