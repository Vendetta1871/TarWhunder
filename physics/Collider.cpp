#include "Collider.h"

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
			BoundRect[0] = (int)glm::floor(Points[i].x);
		}
		if (Points[i].y < BoundRect[1])
		{
			BoundRect[1] = (int)glm::floor(Points[i].y);
		}
		if (Points[i].z < BoundRect[2])
		{
			BoundRect[2] = (int)glm::floor(Points[i].z);
		}
		if (Points[i].x > BoundRect[3])
		{
			BoundRect[3] = (int)glm::ceil(Points[i].x);
		}
		if (Points[i].y > BoundRect[4])
		{
			BoundRect[4] = (int)glm::ceil(Points[i].y);
		}
		if (Points[i].z > BoundRect[5])
		{
			BoundRect[5] = (int)glm::ceil(Points[i].z);
		}
	}
}


void Collider::RecalculatePoints(glm::vec3 origin, glm::mat3 rot)
{
	for (int i = 0; i < VertexCount; ++i)
	{
		Points[i] = rot * p[i] + origin;
	}

	RecalculateBounds();
}