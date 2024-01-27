#include "Math.h"

#pragma GCC optimize ("03")

#include <random>

#define GET_NORMAL_X(a, b, c, d, e, f, g, h, i) ((h-b)*(f-c) - (e-b)*(i-c))
#define GET_NORMAL_Y(a, b, c, d, e, f, g, h, i) ((d-a)*(i-c) - (g-a)*(f-c))
#define GET_NORMAL_Z(a, b, c, d, e, f, g, h, i) ((g-a)*(e-b) - (d-a)*(h-b))

int Math::_antiLoop = 0;

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
		_getValFromArr(p, n, x0, z0) +
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, x0, z3)) / 4.0f;

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
		_getValFromArr(p, n, ix, iz0) +
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x0, z0)) / 4.0f;
	p[ix][z0] = medium + _random(d, f) * s;

	medium = (
		_getValFromArr(p, n, x3, z0) +
		_getValFromArr(p, n, ix1, iz) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, ix, iz)) / 4.0f;
	p[x3][iz] = medium + _random(d, f) * s;

	medium = (
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x3, z3) +
		_getValFromArr(p, n, ix, iz1) +
		_getValFromArr(p, n, x0, z3)) / 4.0f;
	p[ix][z3] = medium + _random(d, f) * s;

	medium = (
		_getValFromArr(p, n, x0, z0) +
		_getValFromArr(p, n, ix, iz) +
		_getValFromArr(p, n, x0, z3) +
		_getValFromArr(p, n, ix0, iz)) / 4.0f;
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

glm::vec3 _getNormal(glm::vec3* v)
{
	float x = GET_NORMAL_X(v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z);
	float y = GET_NORMAL_X(v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z);
	float z = GET_NORMAL_X(v[0].x, v[0].y, v[0].z, v[1].x, v[1].y, v[1].z, v[2].x, v[2].y, v[2].z);
	return glm::vec3(x, y, z);
}

bool Math::SAT_OBBOBB(glm::vec3* a, glm::vec3* b, glm::vec3& normal, float& depth)
{
	std::vector<glm::vec3> axis {
		Math::Normalize(a[1] - a[0]),
		Math::Normalize(a[2] - a[0]),
		Math::Normalize(a[4] - a[0]),
		Math::Normalize(b[1] - b[0]),
		Math::Normalize(b[2] - b[0]),
		Math::Normalize(b[4] - b[0]),
	};

	for (int i = 1; i < 4; ++i)
	{
		for (int j = 1; j < 4; ++j)
		{
			glm::vec3 vec = glm::cross(
				a[i + i / 3] - a[0], b[j + j / 3] - b[0]);
			if (glm::length(vec) != 0.0f)
			{
				axis.push_back(Math::Normalize(vec));
			}
		}
	}

	depth = FLT_MAX;
	for (auto& n : axis)
	{
		float max_a = 0, max_b = 0;
		float min_a = FLT_MAX, min_b = FLT_MAX;
		for (int i = 0; i < 8; ++i)
		{
			float p = glm::dot(a[i], n);
			if (p > max_a)
			{
				max_a = p;
			}
			if (p < min_a)
			{
				min_a = p;
			}
			p = glm::dot(b[i], n);
			if (p > max_b)
			{
				max_b = p;
			}
			if (p < min_b)
			{
				min_b = p;
			}
		}

		float l = std::max(max_a, max_b) - std::min(min_a, min_b);
		float sum = (max_a - min_a) + (max_b - min_b);
		float d = sum - l;
		if (d < 0.0f)
		{
			return false;
		}

		if (d > depth)
		{
			continue;
		}
		depth = d;
		normal = n;
		if (max_a > max_b)
		{
			normal *= -1.0f;
		}
	}

	depth += 0.001f;
	normal = Math::Normalize(normal);
	return true;
}

glm::vec3 _getFurthestPoint(glm::vec3* points, int n, glm::vec3 d)
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

std::vector<std::vector<int>> faces{
	{0, 1, 3, 2},
	{0, 4, 5, 1},
	{2, 6, 4, 0},
	{2, 3, 7, 6},
	{3, 1, 5, 7},
	{4, 6, 7, 5},
};

glm::vec3 Math::GetCollisionPoints_OBBOBB(glm::vec3* a, glm::vec3* b, glm::vec3 n)
{
	glm::vec3 p = _getFurthestPoint(a, 8, n);
	std::vector<glm::vec3> contactPointsA;
	//std::vector<int> verA;
	for (int i = 0; i < 8; ++i)
	{
		float d = 0 - n.x * p.x - n.y * p.y - n.z * p.z;
		float distance = n.x * a[i].x + n.y * a[i].y + n.z * a[i].z + d;

		if (std::abs(distance) < 0.005f)
		{
			contactPointsA.emplace_back(a[i]);
			//verA.push_back(i);
		}
	}

	glm::vec3 q = _getFurthestPoint(b, 8, -n);
	std::vector<glm::vec3> contactPointsB;
	//std::vector<int> verB;
	for (int i = 0; i < 8; ++i)
	{
		float d = 0 - n.x * q.x - n.y * q.y - n.z * q.z;
		float distance = n.x * b[i].x + n.y * b[i].y + n.z * b[i].z + d;

		if (std::abs(distance) < 0.005f)
		{
			contactPointsB.emplace_back(b[i]);
			//verB.push_back(i);
		}
	}

	if (contactPointsA.size() == 1)
	{
		return contactPointsA[0];
	}

	if (contactPointsB.size() == 1)
	{
		return contactPointsB[0];
	}

	//

	if (contactPointsA.size() == 4)
	{
		contactPointsA[0] = (contactPointsA[0] + contactPointsA[1]) / 2.0f;
		contactPointsA[1] = (contactPointsA[2] + contactPointsA[3]) / 2.0f;

		if (glm::length(contactPointsA[0] - contactPointsA[1]) < 0.005f)
		{
			glm::vec3 a_dir = Math::Normalize(contactPointsA[2] - contactPointsA[3]);
			contactPointsA[0] += 1.0f * a_dir;
			contactPointsA[1] -= 1.0f * a_dir;
		}
	}

	if (contactPointsB.size() == 4)
	{
		contactPointsB[0] = (contactPointsB[0] + contactPointsB[1]) / 2.0f;
		contactPointsB[1] = (contactPointsB[2] + contactPointsB[3]) / 2.0f;

		if (glm::length(contactPointsB[0] - contactPointsB[1]) < 0.005f)
		{
			glm::vec3 b_dir = Math::Normalize(contactPointsB[2] - contactPointsB[3]);
			contactPointsB[0] += 1.0f * b_dir;
			contactPointsB[1] -= 1.0f * b_dir;
		}
	}

	//if (contactPointsA.size() == 2 && contactPointsB.size() == 2)
	glm::vec3 c0 = (contactPointsA[0] + contactPointsA[1]) / 2.0f;
	glm::vec3 c1 = (contactPointsB[0] + contactPointsB[1]) / 2.0f;

	glm::vec3 a_dir = Math::Normalize(contactPointsA[1] - contactPointsA[0]);
	float d = glm::dot(c1 - c0, a_dir);

	glm::vec3 point = c0 + d * a_dir;

	return point;
}

float _getAngle(glm::vec3 a, glm::vec3 b, glm::vec3 o)
{
	glm::vec3 ao = Math::Normalize(a - o);
	glm::vec3 bo = Math::Normalize(b - o);
	float cos = glm::dot(ao, bo);
	return std::acos(cos);
}

std::pair<glm::vec3, glm::vec3> Math::GetCollisionPoints(float** h_map, int h_map_size,
	glm::vec3* obj, int x0, int x1, int z0, int z1, float& depth)
{
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<float> depths;

	for (int i = 0; i < 8; ++i) {
		int ix, iz;
		int sign = _getPolygonPos(obj[i].x, obj[i].z, ix, iz);

		glm::vec3 n = Math::GetPolygonNormal(h_map, h_map_size, obj[i].x, obj[i].z);
		float h_map_s = _getValFromArr(h_map, h_map_size, ix, iz);
		float d = -(n.x * ix + n.y * h_map_s + n.z * iz);
		float y = -(n.x * obj[i].x + n.z * obj[i].z + d) / n.y;
		float dy = y - obj[i].y;
		if (dy >= 0.0f)
		{
			points.emplace_back(obj[i]);
			normals.push_back(Math::Normalize(n));
			depths.push_back(dy);
		}
	}

	// collision between terrain vertixes and OBB edges
	for (auto& i : faces)
	{
		glm::vec3 a = obj[i[0]];
		glm::vec3 b = obj[i[1]];
		glm::vec3 c = obj[i[2]];
		glm::vec3 d = obj[i[3]];

		glm::vec3 n = Math::Normalize(glm::cross(c - a, b - a));
		if (glm::dot(n, glm::vec3(0.0f, -1.0f, 0.0f)) >= 0.0f)
		{
			continue;
		}

		float D = -(n.x * a.x + n.y * a.y + n.z * a.z);
		for (int x = x0; x < x1; ++x)
		{
			for (int z = z0; z < z1; ++z)
			{
				float y = -(n.x * x + n.z * z + D) / n.y;
				float dy = h_map[x][z] - y;

				if (dy < 0.0f)
				{
					continue;
				}
				glm::vec3 o = glm::vec3(x, h_map[x][z], z);
				float sum_angle = _getAngle(a, b, o);
				sum_angle += _getAngle(b, c, o);
				sum_angle += _getAngle(c, d, o);
				sum_angle += _getAngle(d, a, o);

				if (std::abs(2.0f * std::_Pi_val - sum_angle) < 0.01f)
				{
					points.emplace_back(o);
					normals.push_back(Math::Normalize(n));
					depths.push_back(dy);
				}
			}
		}
	}

	depth = 0.0f;
	glm::vec3 p = glm::vec3(0.0f);
	glm::vec3 n = glm::vec3(0.0f);
	int count = std::min(std::min(points.size(), normals.size()), depths.size());
	for (int i = 0; i < count; ++i)
	{
		p += 1.0f / count * points[i];
		n += 1.0f / count * normals[i];
		depth += 1.0f / count * depths[i];
	}
	n = Math::Normalize(n);
	return std::make_pair(p, n);
}
