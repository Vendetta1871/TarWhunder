#include "Math.h"

#include <random>

#define GET_NORMAL_X(a, b, c, d, e, f, g, h, i) ((h-b)*(f-c) - (e-b)*(i-c))
#define GET_NORMAL_Y(a, b, c, d, e, f, g, h, i) ((d-a)*(i-c) - (g-a)*(f-c))
#define GET_NORMAL_Z(a, b, c, d, e, f, g, h, i) ((g-a)*(e-b) - (d-a)*(h-b))

float _getValFromArr(float** arr, int size, int x, int y)
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

glm::vec3 Math::GetPolygonNormal(float** h_map, int size, float x, float z)
{
	int ix = glm::floor(x);
	int iz = glm::floor(z);
	x = x - iz;
	z = z - iz;

	int sign = 1;
	if (x + z > 1)
	{
		sign = -1;
		ix += 1;
		iz += 1;
	}

	float y0 = _getValFromArr(h_map, size, ix, iz);
	float y1 = _getValFromArr(h_map, size, ix + sign, iz);
	float y2 = _getValFromArr(h_map, size, ix, iz + sign);

	return glm::vec3(GET_NORMAL_X(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign),
		GET_NORMAL_Y(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign),
		GET_NORMAL_Z(ix, y0, iz, ix + sign, y1, iz, ix, y2, iz + sign));
}

glm::vec3 Math::GetVertexSumNormal(float** h_map, int size, int x, int z)
{
	glm::vec3 n0 = GetPolygonNormal(h_map, size, x + 0.1f, z + 0.1f);
	glm::vec3 n1 = GetPolygonNormal(h_map, size, x - 0.1f, z + 0.2f);
	glm::vec3 n2 = GetPolygonNormal(h_map, size, x - 0.2f, z + 0.1f);
	glm::vec3 n3 = GetPolygonNormal(h_map, size, x - 0.1f, z - 0.1f);
	glm::vec3 n4 = GetPolygonNormal(h_map, size, x + 0.1f, z - 0.2f);
	glm::vec3 n5 = GetPolygonNormal(h_map, size, x + 0.2f, z + 0.1f);

	return n0 + n1 + n2 + n3 + n4 + n5;
}

float ds_random(int depth, float f)
{
	float pow = 1.0f;
	for (int i = 0; i < depth; ++i)
	{
		pow *= f;
	}
	return - pow + (rand() / (RAND_MAX / (2 * pow)));
}

void SquareStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)
{
	float medium = (
		_getValFromArr(p, n, x0, z0) +
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, x0, z3)) / 4.0f;

	p[(x0 + x3) / 2][(z0 + z3) / 2] = medium + ds_random(d, f) * s;
}

void DiamondStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)
{
	int ix = (x0 + x3) / 2;
	int iz = (z0 + z3) / 2;
	int ix0 = ix - (x3 - x0);
	int ix1 = ix + (x3 - x0);
	int iz0 = iz - (z3 - z0);
	int iz1 = iz + (z3 - z0);

	float medium = (
		_getValFromArr(p, n, ix, iz0) +
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x0, z0)) / 4.0f;
	p[ix][z0] = medium + ds_random(d, f) * s;

	medium = (
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, ix1, iz) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, ix, iz)) / 4.0f;
	p[x3][iz] = medium + ds_random(d, f) * s;

	medium = (
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, ix, iz1) +
		_getValFromArr(p, n, x0, z3)) / 4.0f;
	p[ix][z3] = medium + ds_random(d, f) * s;

	medium = (
		_getValFromArr(p, n, x0, z0) +
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x0, z3) +
		_getValFromArr(p, n, ix0, iz)) / 4.0f;
	p[x0][iz] = medium + ds_random(d, f) * s;
}

void Math::DiamondSquare(float** h_map, int size, int step, int depth, int scale, float f)
{
	int d = size / step;
	if (d < 1)
	{
		return;
	}

	for (int x = 0; x < size; x += d)
	{
		for (int z = 0; z < size; z += d)
		{
			SquareStep(h_map, size, x, z, x + d, z + d, depth, scale, f);
		}
	}

	for (int x = 0; x < size; x += d)
	{
		for (int z = 0; z < size; z += d)
		{
			DiamondStep(h_map, size, x, z, x + d, z + d, depth, scale, f);
		}
	}

	DiamondSquare(h_map, size, step * 2, depth + 1, scale, f);
}

float** Math::SmoothArray(float** array, int size) 
{
	float** buffer = new float*[size];

	for (int i = 0; i < size; ++i)
	{
		buffer[i] = new float[size];
		for (int j = 0; j < size; ++j)
		{
			float value = _getValFromArr(array, size, i, j);
			value += _getValFromArr(array, size, i + 1, j);
			value += _getValFromArr(array, size, i, j + 1);
			value += _getValFromArr(array, size, i + 1, j + 1);
			value += _getValFromArr(array, size, i - 1, j);
			value += _getValFromArr(array, size, i, j - 1);
			value += _getValFromArr(array, size, i - 1, j - 1);
			value += _getValFromArr(array, size, i - 1, j + 1);
			value += _getValFromArr(array, size, i + 1, j - 1);
			
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

glm::mat3 Math::GetRotationMatrix(glm::vec3 ox)
{
	float cos_a = ox.x;
	float sin_a = glm::sqrt(1.0f - cos_a * cos_a);
	float cos_b = ox.x == 0 ? glm::sqrt(1.0f - ox.y * ox.y) : (ox.z / ox.x);
	float sin_b = sin_a == 0 ? glm::sqrt(1.0f - ox.b * ox.b) : ( - ox.y / sin_a);

	return glm::mat3(cos_a, ox.y, sin_a * cos_b,
		0.0f, cos_b, sin_b,
		-sin_a, - cos_a * sin_b, ox.z);
}
