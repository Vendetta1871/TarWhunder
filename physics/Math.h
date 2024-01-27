#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include <vector>

class Math
{
private:
	static int _antiLoop;
public:
	static float** SmoothArray(float** array, int size);
	static void DiamondSquare(float** h_map, int size, int step, int depth, int scale, float f);

	static glm::vec3 GetPolygonNormal(float** h_map, int size, float x, float z);
	static glm::vec3 GetVertexSumNormal(float** h_map, int size, int x, int z);

	static glm::vec3 Normalize(glm::vec3 vec);
	static glm::mat3 Reorthogonalize(glm::mat3 m);
	
	static bool SAT_OBBOBB(glm::vec3* a, glm::vec3* b, glm::vec3& normal, float& depth);
	static glm::vec3 GetCollisionPoints_OBBOBB(glm::vec3* a, glm::vec3* b, glm::vec3 n);

	static std::pair<glm::vec3, glm::vec3> GetCollisionPoints(float** h_map, int h_map_size, glm::vec3* obj, int x0, int x1, int z0, int z1, float& depth);
};

#endif
