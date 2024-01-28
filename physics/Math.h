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

	static float _getValFromArr(float** arr, int size, int x, int y);
	static float _getAngle(glm::vec3 a, glm::vec3 b, glm::vec3 o);
	static glm::vec3 _getFurthestPoint(glm::vec3* points, int n, glm::vec3 d);
};

#endif
