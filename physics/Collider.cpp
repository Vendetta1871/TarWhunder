#include "Collider.h"

#include <math.h>

#include "Math.h"

#include <iostream>

std::vector<glm::vec3> Collider::Collisions;

Collider::Collider(glm::vec3* vertixes, int n)
{
	VertexCount = n;
	p = new glm::vec3[n];
	Points = new glm::vec3[n];
	memmove_s(p, n * sizeof(glm::vec3), vertixes, n * sizeof(glm::vec3));
	delete[] vertixes;
}

Collider::Collider()
{
	VertexCount = 0;
	p = nullptr;
	Points = nullptr;
}

Collider::~Collider()
{
	delete[] p;
	delete[] Points;
	delete[] BoundRect;
}

void Collider::RecalculateBounds()
{
	BoundRect[0] = Points[0].x;
	BoundRect[1] = Points[0].y;
	BoundRect[2] = Points[0].z;
	BoundRect[3] = Points[0].x;
	BoundRect[4] = Points[0].y;
	BoundRect[5] = Points[0].z;

	for (int i = 1; i < VertexCount; ++i)
	{
		if (Points[i].x < BoundRect[0])
		{
			BoundRect[0] = Points[i].x;
		}
		if (Points[i].y < BoundRect[1])
		{
			BoundRect[1] = Points[i].y;
		}
		if (Points[i].z < BoundRect[2])
		{
			BoundRect[2] = Points[i].z;
		}
		if (Points[i].x > BoundRect[3])
		{
			BoundRect[3] = Points[i].x;
		}
		if (Points[i].y > BoundRect[4])
		{
			BoundRect[4] = Points[i].y;
		}
		if (Points[i].z > BoundRect[5])
		{
			BoundRect[5] = Points[i].z;
		}
	}
}

void Collider::RecalculatePoints(glm::vec3 origin, glm::mat3 rot)
{
	for (int i = 0; i < VertexCount; ++i)
	{
		Points[i] = origin + rot * p[i];
	}

	RecalculateBounds();
}

std::vector<std::vector<int>> faces{
	{0, 1, 3, 2},
	{0, 4, 5, 1},
	{2, 6, 4, 0},
	{2, 3, 7, 6},
	{3, 1, 5, 7},
	{4, 6, 7, 5},
};

int _getPolPos(float x, float z, int& ix, int& iz)
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

#define M_PI           3.14159265358979323846f

std::pair<glm::vec3, glm::vec3> Collider::GetCollisionPoints(float** h_map, int h_map_size, float& depth)
{
	int count = 0;
	glm::vec3 points[16];
	glm::vec3 normals[16];
	float depths[16];

	for (int i = 0; i < 8; ++i) {
		int ix, iz;
		int sign = _getPolPos(Points[i].x, Points[i].z, ix, iz);

		glm::vec3 n = Math::GetPolygonNormal(h_map, h_map_size, Points[i].x, Points[i].z);
		float h_map_s = Math::_getValFromArr(h_map, h_map_size, ix, iz);
		float d = -(n.x * ix + n.y * h_map_s + n.z * iz);
		float y = -(n.x * Points[i].x + n.z * Points[i].z + d) / n.y;
		float dy = y - Points[i].y;
		if (dy >= 0.0f)
		{
			points[count] = Points[i];
			normals[count] = Math::Normalize(n);
			depths[count] = dy;
			++count;
		}
	}

	// collision between terrain vertixes and OBB edges
	int x0 = std::floor(std::max(BoundRect[0], 0.0f));
	int z0 = std::floor(std::max(BoundRect[2], 0.0f));
	int x1 = std::ceil(std::min(BoundRect[3], h_map_size - 1.0f));
	int z1 = std::ceil(std::min(BoundRect[5], h_map_size - 1.0f));
	for (auto& i : faces)
	{
		glm::vec3 a = Points[i[0]];
		glm::vec3 b = Points[i[1]];
		glm::vec3 c = Points[i[2]];
		glm::vec3 d = Points[i[3]];

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
				float sum_angle = Math::_getAngle(a, b, o);
				sum_angle += Math::_getAngle(b, c, o);
				sum_angle += Math::_getAngle(c, d, o);
				sum_angle += Math::_getAngle(d, a, o);

				if (std::abs(2.0f * M_PI - sum_angle) < 0.01f && count < 16)
				{
					points[count] = o;
					normals[count] = n;
					depths[count] = dy;
					++count;
				}
			}
		}

		if (count >= 16) 
		{
			break;
		}
	}

	depth = 0.0f;
	glm::vec3 p = glm::vec3(0.0f);
	glm::vec3 n = glm::vec3(0.0f);
	if (count == 0) {
		depth = 0.0f;
		return std::make_pair(p, n);
	}

	for (int i = 0; i < count; ++i)
	{
		p += points[i];
		n += normals[i];
		depth += depths[i];
	}
	p /= count;
	depth = 1.0f / count * depth;
	Collisions.push_back(p);
	return std::make_pair(p, Math::Normalize(n));
}

bool Collider::SAT_OBBOBB(glm::vec3* obj, glm::vec3& normal, float& depth)
{
	int count = 6;
	glm::vec3 axis[15] = {
		Math::Normalize(Points[1] - Points[0]),
		Math::Normalize(Points[2] - Points[0]),
		Math::Normalize(Points[4] - Points[0]),
		Math::Normalize(obj[1] - obj[0]),
		Math::Normalize(obj[2] - obj[0]),
		Math::Normalize(obj[4] - obj[0]),
	};

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j < 6; ++j)
		{
			glm::vec3 vec = glm::cross(axis[i], axis[j]);
			if (glm::length(vec) != 0.0f)
			{
				axis[count] = Math::Normalize(vec);
				++count;
			}
		}
	}

	depth = FLT_MAX;
	for (int i = 0; i < count; ++i)
	{
		float max_a = FLT_MIN, max_b = FLT_MIN;
		float min_a = FLT_MAX, min_b = FLT_MAX;
		for (int j = 0; j < 8; ++j)
		{
			float p = glm::dot(Points[j], axis[i]);
			max_a = std::max(max_a, p);
			min_a = std::min(min_a, p);
			p = glm::dot(obj[j], axis[i]);
			max_b = std::max(max_b, p);
			min_b = std::min(min_b, p);
		}

		float l = std::max(max_a, max_b) - std::min(min_a, min_b);
		float sum = (max_a - min_a) + (max_b - min_b);
		float d = sum - l;
		if (d < 0.0f)
		{
			return false;
		}

		if (d <= depth)
		{
			depth = d;
			normal = axis[i];
			if (max_a > max_b)
			{
				normal *= -1.0f;
			}
		}
	}

	depth += 0.001f;
	normal = Math::Normalize(normal);
	return true;
}

bool _lineBoxIntersect(glm::vec3 O, glm::vec3 M, glm::vec3* box, glm::vec3& point)
{
	for (auto& i : faces)
	{
		glm::vec3 a = box[i[0]];
		glm::vec3 b = box[i[1]];
		glm::vec3 c = box[i[2]];
		glm::vec3 d = box[i[3]];

		glm::vec3 n = Math::Normalize(glm::cross(c - a, b - a));

		glm::vec3 O1 = O - glm::dot(O - a, n) * n;
		glm::vec3 M1 = M - glm::dot(M - a, n) * n;

		float y0 = glm::dot(O - O1, n);
		float y1 = glm::dot(M - M1, n);
		if (y0 * y1 > 0.0f)
		{
			continue;
		}
		y0 = std::abs(y0);
		y1 = std::abs(y1);

		point = O1 + y0 / (y0 + y1) * (M1 - O1);

		float sum_angle = Math::_getAngle(a, b, point);
		sum_angle += Math::_getAngle(b, c, point);
		sum_angle += Math::_getAngle(c, d, point);
		sum_angle += Math::_getAngle(d, a, point);

		if (std::abs(2.0f * M_PI - sum_angle) < 0.01f)
		{
			return true;
		}
	}

	return false;
}

int Collider::_getFurthestPointIndex(glm::vec3 d)
{
	int ind_max = 0;
	float max = FLT_MIN;
	for (int i = 0; i < 8; ++i)
	{
		float p = glm::dot(Points[i], d);
		if (p >= max)
		{
			max = p;
			ind_max = i;
		}
	}
	return ind_max;
}

std::vector<std::vector<int>> _getCubeNormals = {
	{1, 2, 4},
	{0, 3, 5},
	{0, 3, 6},
	{1, 2, 7},
	{0, 5, 6},
	{1, 4, 7},
	{2, 4, 7},
	{3, 5, 6}
};

glm::vec3 Collider::GetCollisionPoint_OBBOBB(Collider* obj, glm::vec3 n)
{
	glm::mat3 A = glm::inverse(glm::mat3(
		Points[1] - Points[0], 
		Points[2] - Points[0], 
		Points[4] - Points[0]));

	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 p = A * (obj->Points[i] - Points[0]);
		if (p.x <= 1.0f && p.y <= 1.0f && p.z <= 1.0f &&
			p.x >= 0.0f && p.y >= 0.0f && p.z >= 0.0f)
		{
			Collisions.push_back(obj->Points[i]);
			return obj->Points[i];
		}
	}

	A = glm::inverse(glm::mat3(
		obj->Points[1] - obj->Points[0],
		obj->Points[2] - obj->Points[0],
		obj->Points[4] - obj->Points[0]));

	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 p = A * (Points[i] - obj->Points[0]);
		if (p.x <= 1.0f && p.y <= 1.0f && p.z <= 1.0f &&
			p.x >= 0.0f && p.y >= 0.0f && p.z >= 0.0f)
		{
			Collisions.push_back(Points[i]);
			return Points[i];
		}
	}

	glm::vec3 p = glm::vec3(0.0f);
	int index = _getFurthestPointIndex(n);
	for (auto& i : _getCubeNormals[index])
	{	
		if (_lineBoxIntersect(Points[index], Points[i], obj->Points, p))
		{
			Collisions.push_back(p);
			return p;
		}
	}

	index = obj->_getFurthestPointIndex(-n);
	for (auto& i : _getCubeNormals[index])
	{
		if (_lineBoxIntersect(obj->Points[index], obj->Points[i], Points, p))
		{
			Collisions.push_back(p);
			return p;
		}
	}

	return Math::Normalize(glm::vec3(0.0f));
}
