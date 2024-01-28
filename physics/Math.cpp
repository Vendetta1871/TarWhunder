#include "Math.h"

#pragma GCC optimize ("03")

#include <random>

#define GET_NORMAL_X(a, b, c, d, e, f, g, h, i) ((h-b)*(f-c) - (e-b)*(i-c))
#define GET_NORMAL_Y(a, b, c, d, e, f, g, h, i) ((d-a)*(i-c) - (g-a)*(f-c))
#define GET_NORMAL_Z(a, b, c, d, e, f, g, h, i) ((g-a)*(e-b) - (d-a)*(h-b))

int Math::_antiLoop = 0;

float Math::_getValFromArr(float** arr, int size, int x, int y)
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

float** Math::SmoothArray(float** array, int size)
{
	float** buffer = new float* [size];

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

float _random(int depth, float f)
{
	float pow = 1.0f;
	for (int i = 0; i < depth; ++i)
	{
		pow *= f;
	}
	return -pow + (rand() / (RAND_MAX / (2 * pow)));
}

void _squareStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)
{
	float medium = (
		Math::_getValFromArr(p, n, x0, z0) +
		Math::_getValFromArr(p, n, x3, z0) +
		Math::_getValFromArr(p, n, x3, z3) +
		Math::_getValFromArr(p, n, x0, z3)) / 4.0f;

	p[(x0 + x3) / 2][(z0 + z3) / 2] = medium + _random(d, f) * s;
}

void _diamondStep(float** p, int n, int x0, int z0, int x3, int z3, int d, int s, float f)
{
	int ix = (x0 + x3) / 2;
	int iz = (z0 + z3) / 2;
	int ix0 = ix - (x3 - x0);
	int ix1 = ix + (x3 - x0);
	int iz0 = iz - (z3 - z0);
	int iz1 = iz + (z3 - z0);

	float medium = (
		Math::_getValFromArr(p, n, ix, iz0) +
		Math::_getValFromArr(p, n, x3, z0) +
		Math::_getValFromArr(p, n, ix, iz) +
		Math::_getValFromArr(p, n, x0, z0)) / 4.0f;
	p[ix][z0] = medium + _random(d, f) * s;

	medium = (
		Math::_getValFromArr(p, n, x3, z0) +
		Math::_getValFromArr(p, n, ix1, iz) +
		Math::_getValFromArr(p, n, x3, z3) +
		Math::_getValFromArr(p, n, ix, iz)) / 4.0f;
	p[x3][iz] = medium + _random(d, f) * s;

	medium = (
		Math::_getValFromArr(p, n, ix, iz) +
		Math::_getValFromArr(p, n, x3, z3) +
		Math::_getValFromArr(p, n, ix, iz1) +
		Math::_getValFromArr(p, n, x0, z3)) / 4.0f;
	p[ix][z3] = medium + _random(d, f) * s;

	medium = (
		Math::_getValFromArr(p, n, x0, z0) +
		Math::_getValFromArr(p, n, ix, iz) +
		Math::_getValFromArr(p, n, x0, z3) +
		Math::_getValFromArr(p, n, ix0, iz)) / 4.0f;
	p[x0][iz] = medium + _random(d, f) * s;
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
			_squareStep(h_map, size, x, z, x + d, z + d, depth, scale, f);
		}
	}

	for (int x = 0; x < size; x += d)
	{
		for (int z = 0; z < size; z += d)
		{
			_diamondStep(h_map, size, x, z, x + d, z + d, depth, scale, f);
		}
	}

	DiamondSquare(h_map, size, step * 2, depth + 1, scale, f);
}

int _getPolygonPos(float x, float z, int& ix, int& iz)
{
	ix = std::floor(x);
	iz = std::floor(z);
	x = x - iz;
	z = z - iz;

	int sign = 1;
	if (x + z > 1)
	{
		sign = -1;
		ix += 1;
		iz += 1;
	}
	return sign;
}

glm::vec3 Math::GetPolygonNormal(float** h_map, int size, float x, float z)
{
	int ix, iz;
	int sign = _getPolygonPos(x, z, ix, iz);

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

glm::vec3 Math::Normalize(glm::vec3 vec)
{
	float t = vec.x * vec.x +
		vec.y * vec.y + vec.z * vec.z;
	return 1.0f / std::sqrtf(t) * vec;
}

glm::mat3 Math::Reorthogonalize(glm::mat3 m)
{
	glm::vec3 a = m[0];
	glm::vec3 b = m[1] - glm::dot(m[1], a) / glm::dot(a, a) * a;
	glm::vec3 c = m[2] - glm::dot(m[2], a) / glm::dot(a, a) * a - glm::dot(m[2], b) / glm::dot(b, b) * b;

	return glm::mat3(Normalize(a), Normalize(b), Normalize(c));
}

glm::vec3 Math::_getFurthestPoint(glm::vec3* points, int n, glm::vec3 d)
{
	int ind_max = 0;
	float max = 0.0f;
	for (int i = 0; i < n; ++i)
	{
		float p = glm::dot(points[i], d);
		if (p < max)
		{
			continue;
		}
		max = p;
		ind_max = i;
	}
	return points[ind_max];
}

float Math::_getAngle(glm::vec3 a, glm::vec3 b, glm::vec3 o)
{
	glm::vec3 ao = Math::Normalize(a - o);
	glm::vec3 bo = Math::Normalize(b - o);
	float cos = glm::dot(ao, bo);
	return std::acos(cos);
}
