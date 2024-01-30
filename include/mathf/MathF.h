#ifndef MATHF_H
#define MATHF_H

#include <vector>

#include <glm/glm.hpp>
/*========================
Additional math operations
========================*/
class MathF
{
public:
	/*===================================================
	Returns smoothed array (averages values in area 3x3).
	params:
	array - source array (will be deleted after calling method!)
	size - size of square array
	===================================================*/
	static float** SmoothArray(float** array, int size);
	/*====================================================
	Diamond Square algorithm based heights map generation.  
	params:
	h_map[out] - designation heights map array (should have a [size+1][size+1] shape and zero values on edges)
	size - size of created array
	scale - heights amplitude factor
	f - roughness factor
	step - current square size (default: 1 as a start value)
	depth - current recursion depth (default: 1 as a start value)
	====================================================*/
	static void DiamondSquare(float** h_map, int size, int scale, float f, int step = 1, int depth = 1);
	// fast <v> vector normalization
	static glm::vec3 Normalize(const glm::vec3& v);
	// returns orthonormalized <m> matrix
	static glm::mat3 Orthonormalize(const glm::mat3& m);
	/*=====================================================================================
	Returns value by given indexes from square array or 0 indexes are beyond arrays bounds.
	params:
	array[in] - source array
	size - size of array
	x - first index
	y - second index
	=====================================================================================*/
	static float GetValueSafe(const float*const* array, int size, int x, int y);
	// returns <a, o, b> angle value in radians
	static float GetAngle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& o);
};

#endif
