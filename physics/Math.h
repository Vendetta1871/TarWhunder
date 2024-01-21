#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>

class Math
{
public:
	static float** SmoothArray(float** array, int size);
	static void DiamondSquare(float** h_map, int size, int step, int depth, int scale, float f);
	static glm::vec3 GetPolygonNormal(float** h_map, int size, float x, float z);
	static glm::vec3 GetVertexSumNormal(float** h_map, int size, int x, int z);
	static glm::mat3 GetRotationMatrix(glm::vec3 ox);
};

#endif
