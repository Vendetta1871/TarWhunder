#include "../../include/mathf/MathF.h"

#include <random>

//#pragma GCC optimize ("03")

float** MathF::SmoothArray(float** array, int size)
{
	float** buffer = new float* [size];

	for (int i = 0; i < size; ++i)
	{
		buffer[i] = new float[size];
		for (int j = 0; j < size; ++j)
		{
			float value = GetValueSafe(array, size, i, j);
			value += GetValueSafe(array, size, i + 1, j);
			value += GetValueSafe(array, size, i, j + 1);
			value += GetValueSafe(array, size, i + 1, j + 1);
			value += GetValueSafe(array, size, i - 1, j);
			value += GetValueSafe(array, size, i, j - 1);
			value += GetValueSafe(array, size, i - 1, j - 1);
			value += GetValueSafe(array, size, i - 1, j + 1);
			value += GetValueSafe(array, size, i + 1, j - 1);

			buffer[i][j] = value / 9.0f;
		}
	}

	for (int i = 0; i < size; ++i)
	{
		delete[] array[i];
	}
	delete[] array;
	return buffer;
}
// get random number in { -[r]^n; [r]^n } range (0 <= r <= 1)
float RandomF(int n, float r)
{
	float pow = 1.0f;
	for (int i = 0; i < n; ++i)
	{
		pow *= r;
	}
	return -pow + (rand() / (RAND_MAX / (2 * pow)));
}

void SquareStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)     /***************************************************/       
{                                                                                          	 /*           Diamond-Square algorithm              */
	p[(x0 + x3) / 2][(z0 + z3) / 2] = RandomF(d, f) * s + (									 /* p___________________q                           */
		MathF::GetValueSafe(p, n, x0, z0) +													 /* |    |    |    |    |                           */
		MathF::GetValueSafe(p, n, x3, z0) +													 /* |    |    |    |    |                           */
		MathF::GetValueSafe(p, n, x3, z3) +													 /* |----|----|----|----|   p = rnd                 */
		MathF::GetValueSafe(p, n, x0, z3)) / 4.0f;											 /* |    |    |    |    |   q = rnd				    */
}																							 /* |____|____|____|____| 						    */
																							 /* |    |    |    |    |   r = rnd				    */
void DiamondStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)	 /* |    |    |    |    |   s = rnd				    */
{																							 /* |----|----|----|----| 						    */
	int ix = (x0 + x3) / 2;																	 /* |    |    |    |    | 						    */
	int iz = (z0 + z3) / 2;																	 /* r____|____|____|____s 						    */
	int ix0 = ix - (x3 - x0);																 /* 												*/
	int ix1 = ix + (x3 - x0);																 /* Square step:								    */
	int iz0 = iz - (z3 - z0);																 /* a___________________b						    */
	int iz1 = iz + (z3 - z0);																 /* |    |    |    |    |						    */
																							 /* |    |    |    |    |                           */
	p[ix][z0] = RandomF(d, f) * s + (														 /* |----|----|----|----|                           */
		MathF::GetValueSafe(p, n, ix, iz0) +												 /* |    |    |    |    |                           */
		MathF::GetValueSafe(p, n, x3, z0) +													 /* |____|____p____|____|   p = (a+b+c+d) / 4 + rnd */
		MathF::GetValueSafe(p, n, ix, iz) +													 /* |    |    |    |    |						    */
		MathF::GetValueSafe(p, n, x0, z0)) / 4.0f;											 /* |    |    |    |    |						    */
	p[x3][iz] = RandomF(d, f) * s + (														 /* |----|----|----|----|						    */
		MathF::GetValueSafe(p, n, x3, z0) +													 /* |    |    |    |    |						    */
		MathF::GetValueSafe(p, n, ix1, iz) +												 /* c____|____|____|____d						    */
		MathF::GetValueSafe(p, n, x3, z3) +													 /* 												*/
		MathF::GetValueSafe(p, n, ix, iz)) / 4.0f;											 /* Diamond Step:								    */
	p[ix][z3] = RandomF(d, f) * s + (														 /* a_________q_________b						    */
		MathF::GetValueSafe(p, n, ix, iz) +													 /* |    |    |    |    |						    */
		MathF::GetValueSafe(p, n, x3, z3) +													 /* |    |    |    |    |						    */
		MathF::GetValueSafe(p, n, ix, iz1) +												 /* |----|----|----|----|    p = (a+c+d) / 3 + rnd  */
		MathF::GetValueSafe(p, n, x0, z3)) / 4.0f;											 /* |    |    |    |    |    q = (a+b+c) / 3 + rnd  */
	p[x0][iz] = RandomF(d, f) * s + (														 /* p____|____c____|____r						    */
		MathF::GetValueSafe(p, n, x0, z0) +													 /* |    |    |    |    |    r = (c+b+e) / 3 + rnd  */
		MathF::GetValueSafe(p, n, ix, iz) +													 /* |    |    |    |    |    s = (d+c+e) / 3 + rnd  */
		MathF::GetValueSafe(p, n, x0, z3) +													 /* |----|----|----|----|						    */
		MathF::GetValueSafe(p, n, ix0, iz)) / 4.0f;											 /* |    |    |    |    |						    */
}																							 /* d____|____s____|____e						    */
																							 /*												    */
void MathF::DiamondSquare(float** h_map, int size, int scale, float f, int step, int depth)	 /* Square step:									*/
{																							 /* a_________b_________c						    */
	int d = size / step;																	 /* |    |    |    |    |                           */
	if (d < 1)																				 /* |    |    |    |    | 						    */
	{																						 /* |----p----|----q----|   p = (a+b+d+e) / 4 + rnd */
		return;																				 /* |    |    |    |    |   q = (b+c+e+f) / 4 + rnd */
	}																						 /* d____|____e____|____f						    */
																							 /* |    |    |    |    |   r = (d+e+g+h) / 4 + rnd */
	for (int x = 0; x < size; x += d)														 /* |    |    |    |    |   s = (e+f+h+i) / 4 + rnd */
	{																						 /* |----r----|----s----|						    */
		for (int z = 0; z < size; z += d)													 /* |    |    |    |    |						    */
		{																					 /* g____|____h____|____i						    */
			SquareStep(h_map, size, x, z, x + d, z + d, depth, scale, f);					 /*												    */
		}																					 /* Diamond step (final)    o = (a+b+d) / 3 + rnd   */
	}																						 /* a____o____b____p____c   p = (b+c+e) / 3 + rnd   */
																							 /* |    |    |    |    |   q = (a+d+f) / 3 + rnd   */
	for (int x = 0; x < size; x += d)														 /* |    |    |    |    |   r = (d+b+e+g) / 4 + rnd */
	{																						 /* q----d----r----e----s   s = (c+e+h) / 3 + rnd   */
		for (int z = 0; z < size; z += d)													 /* |    |    |    |    |   t = (f+d+g+i) / 4 + rnd */
		{																					 /* f____t____g____u____h   u = (g+e+h+j) / 4 + rnd */
			DiamondStep(h_map, size, x, z, x + d, z + d, depth, scale, f);					 /* |    |    |    |    |   v = (f+i+k) / 3 + rnd   */
		}																					 /* |    |    |    |    |   w = (i+g+j+l) / 4 + rnd */
	}																						 /* v----i----w----j----x   x = (j+h+m) / 3 + rnd   */
																							 /* |    |    |    |    |   y = (k+i+l) / 3 + rnd   */
	DiamondSquare(h_map, size, scale, f, step * 2, depth + 1);								 /* k____y____l____z____m   z = (l+j+m) / 3 + rnd   */
}																							 /***************************************************/

glm::vec3 MathF::Normalize(const glm::vec3& v)
{
	float t = v.x * v.x + v.y * v.y + v.z * v.z;
	return 1.0f / std::sqrtf(t) * v;
}

glm::mat3 MathF::Orthonormalize(const glm::mat3& m)
{
	// Gram-Schmidt process
	glm::vec3 a = m[0];
	glm::vec3 b = m[1] - glm::dot(m[1], a) / glm::dot(a, a) * a;
	glm::vec3 c = m[2] - glm::dot(m[2], a) / glm::dot(a, a) * a - glm::dot(m[2], b) / glm::dot(b, b) * b;

	return glm::mat3(Normalize(a), Normalize(b), Normalize(c));
}

float MathF::GetValueSafe(const float* const* arr, int size, int x, int y)
{
	if (x < 0 || x >= size)
	{
		return 0;
	}
	if (y < 0 || y >= size)
	{
		return 0;
	}
	return arr[x][y];
}

float MathF::GetAngle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& o)
{
	glm::vec3 ao = Normalize(a - o);
	glm::vec3 bo = Normalize(b - o);
	float cos = glm::dot(ao, bo);
	return std::acos(cos);
}
