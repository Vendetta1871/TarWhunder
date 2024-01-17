#include "Math.h"

#include <random>

#include <glm/glm.hpp>

#define GET_NORMAL_X(a, b, c, d, e, f, g, h, i) ((e-b)*(i-c) - (h-b)*(f-c))
#define GET_NORMAL_Y(a, b, c, d, e, f, g, h, i) ((g-a)*(f-c) - (d-a)*(i-c))
#define GET_NORMAL_Z(a, b, c, d, e, f, g, h, i) ((d-a)*(h-b) - (g-a)*(e-b))

float _p(float** arr, int size, int x, int y)
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

void Math::GetVertexNormal(float** h_map, int size, int x, int z, float* n_x, float* n_y, float* n_z)
{
	float y0 = _p(h_map, size, x, z);
	float y1 = _p(h_map, size, x + 1, z);
	float y2 = _p(h_map, size, x, z - 1);
	float y3 = _p(h_map, size, x - 1, z - 1);
	float y4 = _p(h_map, size, x - 1, z);
	float y5 = _p(h_map, size, x, z + 1);
	float y6 = _p(h_map, size, x + 1, z + 1);

	glm::vec3 n0 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x + 1, y1, z, x, y2, z - 1),
		GET_NORMAL_Y(x, y0, z, x + 1, y1, z, x, y2, z - 1),
		GET_NORMAL_Z(x, y0, z, x + 1, y1, z, x, y2, z - 1));
	glm::vec3 n1 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x, y2, z - 1, x - 1, y3, z - 1),
		GET_NORMAL_Y(x, y0, z, x, y2, z - 1, x - 1, y3, z - 1),
		GET_NORMAL_Z(x, y0, z, x, y2, z - 1, x - 1, y3, z - 1));
	glm::vec3 n2 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x - 1, y3, z - 1, x - 1, y4, z),
		GET_NORMAL_Y(x, y0, z, x - 1, y3, z - 1, x - 1, y4, z),
		GET_NORMAL_Z(x, y0, z, x - 1, y3, z - 1, x - 1, y4, z));
	glm::vec3 n3 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x - 1, y4, z, x, y5, z + 1),
		GET_NORMAL_Y(x, y0, z, x - 1, y4, z, x, y5, z + 1),
		GET_NORMAL_Z(x, y0, z, x - 1, y4, z, x, y5, z + 1));
	glm::vec3 n4 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x, y5, z + 1, x + 1, y6, z + 1),
		GET_NORMAL_Y(x, y0, z, x, y5, z + 1, x + 1, y6, z + 1),
		GET_NORMAL_Z(x, y0, z, x, y5, z + 1, x + 1, y6, z + 1));
	glm::vec3 n5 = glm::vec3(
		GET_NORMAL_X(x, y0, z, x + 1, y6, z + 1, x + 1, y1, z),
		GET_NORMAL_Y(x, y0, z, x + 1, y6, z + 1, x + 1, y1, z),
		GET_NORMAL_Z(x, y0, z, x + 1, y6, z + 1, x + 1, y1, z));

	glm::vec3 n = glm::normalize(n0 + n1 + n2 + n3 + n4 + n5);
	*n_x = n.x;
	*n_y = n.y;
	*n_z = n.z;
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
		_p(p, n, x0, z0) +
		_p(p, n, x3, z0) +
		_p(p, n, x3, z3) +
		_p(p, n, x0, z3)) / 4.0f;

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
		_p(p, n, ix, iz0) +
		_p(p, n, x3, z0) +
		_p(p, n, ix, iz) +
		_p(p, n, x0, z0)) / 4.0f;
	p[ix][z0] = medium + ds_random(d, f) * s;

	medium = (
		_p(p, n, x3, z0) +
		_p(p, n, ix1, iz) +
		_p(p, n, x3, z3) +
		_p(p, n, ix, iz)) / 4.0f;
	p[x3][iz] = medium + ds_random(d, f) * s;

	medium = (
		_p(p, n, ix, iz) +
		_p(p, n, x3, z3) +
		_p(p, n, ix, iz1) +
		_p(p, n, x0, z3)) / 4.0f;
	p[ix][z3] = medium + ds_random(d, f) * s;

	medium = (
		_p(p, n, x0, z0) +
		_p(p, n, ix, iz) +
		_p(p, n, x0, z3) +
		_p(p, n, ix0, iz)) / 4.0f;
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
			float value = _p(array, size, i, j);
			value += _p(array, size, i + 1, j);
			value += _p(array, size, i, j + 1);
			value += _p(array, size, i + 1, j + 1);
			value += _p(array, size, i - 1, j);
			value += _p(array, size, i, j - 1);
			value += _p(array, size, i - 1, j - 1);
			value += _p(array, size, i - 1, j + 1);
			value += _p(array, size, i + 1, j - 1);
			
			buffer[i][j] = value / 9.0f;
		}
	}

	delete array;
	return buffer;
}
